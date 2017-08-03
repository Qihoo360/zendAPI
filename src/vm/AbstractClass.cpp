// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by zzu_softboy on 2017/08/01.

#include <iostream>
#include "zapi/vm/AbstractClass.h"
#include "zapi/vm/internal/AbstractClassPrivate.h"
#include "zapi/lang/Method.h"

namespace zapi
{
namespace vm
{

namespace internal
{

AbstractClassPrivate::AbstractClassPrivate(const char *className, lang::ClassType type)
   : m_name(className),
     m_type(type)
{}

zend_object *AbstractClassPrivate::createObject(zend_class_entry *entry)
{
   
}

zend_object *AbstractClassPrivate::cloneObject(zval *value)
{
   
}

void AbstractClassPrivate::destructObject(zend_object *object)
{
   
}

void AbstractClassPrivate::freeObject(zend_object *object)
{
   
}

zend_function *AbstractClassPrivate::getStaticMethod(zend_class_entry *entry, zend_string *methodName)
{
   
}

zend_class_entry *AbstractClassPrivate::initialize(const std::string &ns)
{
   zend_class_entry entry;
   if (ns.size() > 0 && ns != "\\") {
      m_name = ns + "\\" + m_name;
   }
   // initialize the class entry
   INIT_CLASS_ENTRY_EX(entry, m_name.c_str(), m_name.size(), getMethodEntries().get());
   entry.create_object = &AbstractClassPrivate::createObject;
   entry.get_static_method = &AbstractClassPrivate::getStaticMethod;
   // check if traversable
   // check if serializable
   if (m_parent) {
      if (m_parent->m_classEntry) {
         m_classEntry = zend_register_internal_class_ex(&entry, m_parent->m_classEntry);
      } else {
         std::cerr << "Derived class " << m_name << " is initialized before base class " << m_parent->m_name
                   << ": base class is ignored" << std::endl;
         // ignore base class
         m_classEntry = zend_register_internal_class(&entry);
      }
   } else {
      m_classEntry = zend_register_internal_class(&entry);
   }
   // register the interfaces of the class
   for (std::shared_ptr<AbstractClassPrivate> &interface : m_interfaces) {
      if (interface->m_classEntry) {
         zend_class_implements(m_classEntry, 1, interface->m_classEntry);
      } else {
         // interface that want to implement is not initialized
         std::cerr << "Derived class " << m_name << " is initialized before base class "
                   << interface->m_name << ": interface is ignored"
                   << std::endl;
         
      }
   }
   
   // TODO we really save self pointer ?
   m_classEntry->ce_flags = static_cast<int>(m_type);
   // TODO all member variables
   return m_classEntry;
}

std::unique_ptr<zend_function_entry[]>& AbstractClassPrivate::getMethodEntries()
{
   if (m_methodEntries) {
      return m_methodEntries;
   }
   m_methodEntries.reset(new zend_function_entry[m_methods.size() + 1]);
   int i = 0;
   for (std::shared_ptr<Method> &method : m_methods) {
      zend_function_entry *entry = &m_methodEntries[i++];
      method->initialize(entry, m_name.c_str());
   }
   zend_function_entry *last = &m_methodEntries[i];
   memset(last, 0, sizeof(*last));
   return m_methodEntries;
}

} // internal


AbstractClass::AbstractClass(const char *className, lang::ClassType type)
   : m_implPtr(std::make_shared<AbstractClassPrivate>(className, type))
{}

AbstractClass::AbstractClass(const AbstractClass &other)
   : m_implPtr(other.m_implPtr)
{}

AbstractClass::AbstractClass(AbstractClass &&other) ZAPI_DECL_NOEXCEPT
   : m_implPtr(std::move(other.m_implPtr))
{}

AbstractClass &AbstractClass::operator=(const AbstractClass &other)
{
   return *this;
}

AbstractClass &AbstractClass::operator=(AbstractClass &&other) ZAPI_DECL_NOEXCEPT
{
   return *this;
}

StdClass *AbstractClass::construct() const
{
   return nullptr;
}

StdClass *AbstractClass::clone() const
{
   return nullptr;
}

AbstractClass::~AbstractClass()
{}

zend_class_entry *AbstractClass::initialize(const std::string &prefix)
{
   return getImplPtr()->initialize(prefix);
}

zend_class_entry *AbstractClass::initialize()
{
   return initialize("");
}

} // vm
} // zapi