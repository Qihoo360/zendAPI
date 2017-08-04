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
// Created by zzu_softboy on 2017/08/03.

#include "zapi/vm/ObjectBinder.h"
#include "zapi/lang/StdClass.h"
#include "zapi/lang/internal/StdClassPrivate.h"

namespace zapi
{
namespace vm
{

using zapi::lang::StdClass;
using zapi::lang::internal::StdClassPrivate;

ObjectBinder::ObjectBinder(zend_class_entry *entry, StdClass *nativeObject, 
                           const zend_object_handlers *objectHandlers, uint32_t refCount)
   : m_nativeObject(nativeObject)
{
   // here can be negative ？ 
   // return sizeof(zval) *
   // (ce->default_properties_count -
   // ((ce->ce_flags & ZEND_ACC_USE_GUARDS) ? 0 : 1));
   // maybe -16
   // or we may not set something not correct
   ssize_t propertiesSize = static_cast<ssize_t>(zend_object_properties_size(entry));
   propertiesSize = propertiesSize < 0 ? 0 : propertiesSize;
   m_container = static_cast<Container *>(ecalloc(1, sizeof(Container) + static_cast<size_t>(propertiesSize)));
   m_container->m_zendObject.ce = entry;
   m_container->m_self = this;
   zend_object_std_init(&m_container->m_zendObject, entry);
   object_properties_init(&m_container->m_zendObject, entry);
   m_container->m_zendObject.handlers = objectHandlers;
   if (refCount != 1) {
      GC_REFCOUNT(&m_container->m_zendObject) = refCount;
   }
}

zend_object *ObjectBinder::getZendObject() const
{
   return &m_container->m_zendObject;
}

ObjectBinder::~ObjectBinder()
{
   zend_object_std_dtor(&m_container->m_zendObject);
   // maybe be free twice, but no error, very strange
   // if you delete this statement, zend memory manager 
   // complain memory leak
   efree(m_container);
}

void ObjectBinder::destroy()
{
   delete this;
}

StdClass *ObjectBinder::getNativeObject() const
{
   return m_nativeObject.get();
}

ObjectBinder *ObjectBinder::retrieveSelfPtr(const zend_object *object)
{
   Container *container = reinterpret_cast<Container *>(const_cast<char *>(reinterpret_cast<const char *>(object) - calculateZendObjectOffset()));
   return container->m_self;
}

ObjectBinder *ObjectBinder::retrieveSelfPtr(zval *object)
{
   return retrieveSelfPtr(Z_OBJ_P(object));
}

} // vm
} // zapi