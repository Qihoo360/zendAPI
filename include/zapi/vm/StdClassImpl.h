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
// Created by zzu_softboy on 08/06/2017.

#ifndef ZAPI_VM_STDCLASSIMPL_H
#define ZAPI_VM_STDCLASSIMPL_H

#include <memory>

#include "zapi/Global.h"
#include "zapi/lang/StdClass.h"

#include "php/Zend/zend_types.h"
#include "php/Zend/zend_alloc.h"
#include "php/Zend/zend_objects_API.h"

namespace zapi
{
namespace vm
{

using zapi::lang::StdClass;

class StdClassImpl
{
private:
   /**
    * Structure with a first element which is a zend_object, so that
    * it can be casted to a zend_object
    * @var    MixedObject
    */
   struct MixedObject
   {
      /**
       * Pointer to ourselves
       * @var ObjectImpl
        */
      StdClassImpl *self;
      /**
       * The actual object MUST be the last member, because PHP uses hackish
       * tricks for optimization (we allocate more memory than sizeof(MixedObject))
       * @var    zend_object
       */
      zend_object object;
   } *m_mixed;

   /**
    * Pointer to the C++ implementation
    * @var    std::unique_ptr<Base>
    */
   std::unique_ptr<StdClass> m_object;

public:
   /**
    * Constructor
    *
    * This will create a new object in the Zend engine.
    *
    * @param  entry       Zend class entry
    * @param  handler     Zend object handlers
    * @param  base        C++ object that already exists
    * @param  refcount    The initial refcount for the object
    */
   StdClassImpl(zend_class_entry *entry, StdClass *object, zend_object_handlers *handlers, int refCount)
      : m_object(object)
   {
      // allocate a mixed object (for some reason this does not have to be deallocated)
      m_mixed = static_cast<MixedObject *>(ecalloc(1, sizeof(MixedObject) + zend_object_properties_size(entry)));
      m_mixed->object.ce = entry;
      m_mixed->self = this;
      // initialize the object and its properties
      zend_object_std_init(&m_mixed->object, entry);
      object_properties_init(&m_mixed->object, entry);

      // install the handlers
      m_mixed->object.handlers = handlers;

      // set the initial refcount (if it is different than one, because one is the default)
      if (1 != refCount) {
         GC_REFCOUNT(getZendObject()) = refCount;
      }
      object->m_implPtr = this;
   }

   virtual ~StdClassImpl()
   {
      zend_object_std_dtor(&m_mixed->object);
   }

   void destruct()
   {
      delete this;
   }

   /**
    * The offset between the zend_object and the ObjectImpl
    * in bytes. This can be used to find the other when only
    * a pointer to one is available.
    *
    * @return The offset in bytes
    */
   static constexpr size_t getOffset()
   {
      return offsetof(MixedObject, object);
   }

   /**
    * Find the object based on a zval
    * @param  val         Zval object
    * @return ObjectImpl
    */
   static StdClassImpl *find(zval *val)
   {
      return find(Z_OBJ_P(val));
   }

   /**
    * Find the object based on a zend_object
    * @param  object      Zend object pointer
    * @return ObjectImpl
    */
   static StdClassImpl *find(const zend_object *object)
   {
      // the zend_object is the last pointer in the struct so we have to subtract the
      // correct number of bytes from the pointer to get at the address at which the
      // actual StdClassImpl starts. to be able to actually perform this pointer arithmetic
      // we must first cast the pointer to a char (void pointer arithmetic is not allowed!)
      auto *mixed = reinterpret_cast<const MixedObject *>(reinterpret_cast<const char *>(object) - getOffset());
      return mixed->self;
   }

   /**
    * Retrieve the base class of the original C++ object
    * @return Base
    */
   StdClass *getObject() const
   {
      return m_object.get();
   }

   /**
    * Pointer to the PHP object
    * @return zend_object
    */
   zend_object *getZendObject() const
   {
      return &m_mixed->object;
   }
};

} // vm
} // zapi

#endif // ZAPI_VM_STDCLASSIMPL_H
