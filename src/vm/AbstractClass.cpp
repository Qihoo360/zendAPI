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
#include "zapi/vm/ObjectBinder.h"
#include "zapi/lang/Method.h"
#include "zapi/lang/StdClass.h"
#include "zapi/kernel/NotImplemented.h"
#include "zapi/kernel/OrigException.h"

namespace zapi
{
namespace vm
{

namespace internal
{
namespace
{
AbstractClassPrivate *retrieve_acp_ptr_from_cls_entry(zend_class_entry *entry)
{
   // we hide the pointer in entry->info.user.doc_comment
   // the format is \0 + pointer_address
   // if entry->info.user.doc_comment length > 0 or nullptr no pointer hide in it
   while (entry->parent && (nullptr == entry->info.user.doc_comment ||
                            ZSTR_LEN(entry->info.user.doc_comment) > 0))
   {
      // we find the pointer in parent classs
      entry = entry->parent;
   }
   const char *comment = ZSTR_VAL(entry->info.user.doc_comment);
   // here we retrieve the second byte, it have the pointer infomation
   return *reinterpret_cast<AbstractClassPrivate **>(const_cast<char *>(comment + 1));
}

void acp_ptr_deleter(zend_string *ptr)
{
   zend_string_release(ptr);
}
} // anonymous namespace

using zapi::lang::StdClass;
using zapi::vm::ObjectBinder;
using zapi::kernel::NotImplemented;
using zapi::kernel::Exception;

AbstractClassPrivate::AbstractClassPrivate(const char *className, lang::ClassType type)
   : m_name(className),
     m_type(type),
     m_self(nullptr, acp_ptr_deleter)
{}

zend_object *AbstractClassPrivate::createObject(zend_class_entry *entry)
{
   // here we lose everything from AbstractClass object
   // of course we are in static method of AbstractClass
   // but we need get pointer to it, we need some meta info in it and we must
   // instantiate native c++ class associated with the meta class
   AbstractClassPrivate *abstractClsPrivatePtr = retrieve_acp_ptr_from_cls_entry(entry);
   // note: here we use StdClass type to store Derived class
   StdClass *nativeObject = abstractClsPrivatePtr->m_apiPtr->construct();
   if (!nativeObject) {
      // report error on failure, because this function is called directly from the
      // Zend engine, we can call zend_error() here (which does a longjmp() back to
      // the Zend engine)
      zend_error(E_ERROR, "Unable to instantiate %s", entry->name->val);
   }
   // here we assocaited a native object with an ObjectBinder object
   // ObjectBinder can make an relationship on nativeObject and zend_object
   // don't warry about memory, we do relase
   ObjectBinder *binder = new ObjectBinder(entry, nativeObject, abstractClsPrivatePtr->getObjectHandlers(), 1);
   return binder->getZendObject();
}

zend_object *AbstractClassPrivate::cloneObject(zval *value)
{
   return nullptr;
}

int AbstractClassPrivate::countElements(zval *objecy, zend_long *count)
{
   
}

zval *AbstractClassPrivate::readDimension(zval *object, zval *offset, int type, zval *rv)
{
   
}

void AbstractClassPrivate::writeDimension(zval *object, zval *offset, zval *value)
{
   
}

int AbstractClassPrivate::hasDimension(zval *object, zval *offset, int checkEmpty)
{
   
}

void AbstractClassPrivate::unsetDimension(zval *object, zval *offset)
{
   
}

zval *AbstractClassPrivate::readProperty(zval *object, zval *name, int type, void **cacheSlot, zval *rv)
{
   
}

void AbstractClassPrivate::writeProperty(zval *object, zval *name, zval *value, void **cacheSlot)
{
   
}

int AbstractClassPrivate::hasProperty(zval *object, zval *name, int hasSetExists, void **cacheSlot)
{
   
}

void AbstractClassPrivate::unsetProperty(zval *object, zval *member, void **cacheSlot)
{
   
}

zend_function *AbstractClassPrivate::getMethod(zend_object **object, zend_string *method, const zval *key)
{
   
}

int AbstractClassPrivate::getClosure(zval *object, zend_class_entry **entry, zend_function **func, 
                                     zend_object **objectPtr)
{
   
}

int AbstractClassPrivate::cast(zval *object, zval *retValue, int type)
{
   
}

int AbstractClassPrivate::compare(zval *left, zval *right)
{
   
}

void AbstractClassPrivate::destructObject(zend_object *object)
{
   ObjectBinder *binder = ObjectBinder::retrieveSelfFromZendObject(object);
   AbstractClassPrivate *selfPtr = retrieve_acp_ptr_from_cls_entry(object->ce);
   try {
      if (binder->getNativeObject()) {
         selfPtr->m_apiPtr->callDestruct(binder->getNativeObject());
      }
   } catch (const NotImplemented &exception) {
      zend_objects_destroy_object(object);
   } catch (Exception &exception) {
      // a regular zapi::kernel::Exception was thrown by the extension, pass it on
      // to PHP user space
      kernel::process_exception(exception);
   }
}

void AbstractClassPrivate::freeObject(zend_object *object)
{
   ObjectBinder *binder = ObjectBinder::retrieveSelfFromZendObject(object);
   binder->destroy();
}

zend_function *AbstractClassPrivate::getStaticMethod(zend_class_entry *entry, zend_string *methodName)
{
   return nullptr;
}

zend_class_entry *AbstractClassPrivate::initialize(AbstractClass *cls, const std::string &ns)
{
   m_apiPtr = cls;
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
   
   // save AbstractClassPrivate instance pointer into the info.user.doc_comment of zend_class_entry
   // we need save the address of this pointer
   AbstractClassPrivate *selfPtr = this;
   m_self.reset(zend_string_alloc(sizeof(this), 1));
   m_classEntry->ce_flags = static_cast<uint32_t>(m_type);
   // make the string look like empty
   ZSTR_VAL(m_self)[0] = '\0';
   ZSTR_LEN(m_self) = 0;
   std::memcpy(ZSTR_VAL(m_self.get()) + 1, &selfPtr, sizeof(selfPtr));
   // save into the doc_comment
   m_classEntry->info.user.doc_comment = m_self.get();
   // TODO all member variables
   return m_classEntry;
}

std::unique_ptr<zend_function_entry[]>& AbstractClassPrivate::getMethodEntries()
{
   if (m_methodEntries) {
      return m_methodEntries;
   }
   m_methodEntries.reset(new zend_function_entry[m_methods.size() + 1]);
   size_t i = 0;
   for (std::shared_ptr<Method> &method : m_methods) {
      zend_function_entry *entry = &m_methodEntries[i++];
      method->initialize(entry, m_name.c_str());
   }
   zend_function_entry *last = &m_methodEntries[i];
   memset(last, 0, sizeof(*last));
   return m_methodEntries;
}

zend_object_handlers *AbstractClassPrivate::getObjectHandlers()
{
   if (m_intialized) {
      return &m_handlers;
   }
   memcpy(&m_handlers, &std_object_handlers, sizeof(zend_object_handlers));
   if (!m_apiPtr->clonable()) {
      m_handlers.clone_obj = nullptr;
   } else {
      m_handlers.clone_obj = &AbstractClassPrivate::cloneObject;
   }
   // function for array access interface
   m_handlers.count_elements = &AbstractClassPrivate::countElements;
   m_handlers.write_dimension = &AbstractClassPrivate::writeDimension;
   m_handlers.read_dimension = &AbstractClassPrivate::readDimension;
   m_handlers.has_dimension = &AbstractClassPrivate::hasDimension;
   m_handlers.unset_dimension = &AbstractClassPrivate::unsetDimension;
   
   // functions for magic properties handlers __get, __set, __isset and __unset
   m_handlers.write_property = &AbstractClassPrivate::writeProperty;
   m_handlers.read_property = &AbstractClassPrivate::readProperty;
   m_handlers.has_property = &AbstractClassPrivate::hasProperty;
   m_handlers.unset_property = &AbstractClassPrivate::unsetProperty;
   
   // functions for method is called
   m_handlers.get_method = &AbstractClassPrivate::getMethod;
   m_handlers.get_closure = &AbstractClassPrivate::getClosure;
   
   // functions for object destruct
   m_handlers.dtor_obj = &AbstractClassPrivate::destructObject;
   m_handlers.free_obj = &AbstractClassPrivate::freeObject;
   
   // functions for type cast
   m_handlers.cast_object = &AbstractClassPrivate::cast;
   m_handlers.compare_objects = &AbstractClassPrivate::compare;
   m_intialized = true;
   return &m_handlers;
}

} // internal


AbstractClass::AbstractClass(const char *className, lang::ClassType type)
   : m_implPtr(std::make_shared<AbstractClassPrivate>(className, type))
{
}

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

void AbstractClass::callClone(StdClass *nativeObject) const
{}

void AbstractClass::callDestruct(StdClass *nativeObject) const
{}

bool AbstractClass::clonable() const
{
   return false;
}

bool AbstractClass::serializable() const
{
   return false;
}

bool AbstractClass::traversable() const
{
   return false;
}

AbstractClass::~AbstractClass()
{}

zend_class_entry *AbstractClass::initialize(const std::string &prefix)
{
   return getImplPtr()->initialize(this, prefix);
}

zend_class_entry *AbstractClass::initialize()
{
   return initialize("");
}

void AbstractClass::registerProperty(const char *name, std::nullptr_t value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, int16_t value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, int32_t value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, int64_t value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, char value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, const std::string &value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, const char *value, Modifier)
{
   
}

void AbstractClass::registerProperty(const char *name, bool value, Modifier flags)
{
   
}

void AbstractClass::registerProperty(const char *name, double value, Modifier flags)
{
   
}

} // vm
} // zapi