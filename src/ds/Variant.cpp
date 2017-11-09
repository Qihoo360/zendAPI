// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by zzu_softboy on 2017/06/06.

#include "zapi/kernel/FatalError.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/ds/Variant.h"
#include "zapi/ds/internal/VariantPrivate.h"
#include "zapi/ds/ArrayVariant.h"
#include "zapi/ds/StringVariant.h"
#include "zapi/ds/BoolVariant.h"
#include "zapi/ds/DoubleVariant.h"
#include "zapi/ds/NumericVariant.h"
#include "zapi/ds/ObjectVariant.h"
#include "zapi/ds/CallableVariant.h"
#include "zapi/lang/StdClass.h"
#include "zapi/lang/internal/StdClassPrivate.h"
#include <cstring>
#include <iostream>

namespace zapi
{
namespace ds
{

namespace internal
{

_zval_struct & VariantPrivate::operator*() const &
{
   return *const_cast<_zval_struct *>(reinterpret_cast<const _zval_struct *>(&m_buffer));
}

VariantPrivate::operator _zval_struct*() const &
{
   return const_cast<_zval_struct *>(reinterpret_cast<const _zval_struct *>(&m_buffer));
}

VariantPrivate::operator const _zval_struct*() const &
{
   return reinterpret_cast<const _zval_struct *>(&m_buffer);
}

_zval_struct * VariantPrivate::dereference() const
{
   _zval_struct *self = const_cast<_zval_struct *>(reinterpret_cast<const _zval_struct*>(&m_buffer));
   if (Z_TYPE_P(self) != IS_REFERENCE) {
      return self;
   }
   return Z_REFVAL_P(self);
}

} // internal

using internal::VariantPrivate;
using zapi::lang::StdClass;
using zapi::kernel::FatalError;

namespace
{
void std_zval_deleter(VariantPrivate *ptr)
{
   zval *v = *ptr;
   if (Z_COUNTED_P(v)) {
      zval_ptr_dtor(v);
   }
   delete ptr;
}
}

/**
 * Implementation for the Value class, which wraps a PHP userspace
 * value (a 'zval' in Zend's terminology) into a C++ object
 *
 * Reminder for the implementer:
 *
 *     A 'zval' is an object that represents a _value_ in the PHP user space,
 *     and thus not a variable. A 'value' or 'zval' can be used by many
 *     different variables at the same time. The 'refcount' property of the
 *     zval holds the number of variables ($a, $b, $c, et cetera) that are
 *     all linked to the same value. With this system, PHP can implement copy
 *     on write behavior.
 *
 *     Next to the refcount, the zval also holds a is_ref property, which is
 *     set to true if all variables linked to the value are references of each
 *     other. Thus is $a, $b and $c all point to the same variable, and is_ref
 *     is set to true, changing the value means that the $a, $b and $c value
 *     are all updated. If is_res was false, a change to $a would not mean a
 *     change to $b, and the zval should have been copied first.
 */

/**
 * Constructor (value = NULL)
 */
Variant::Variant()
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_NULL(getUnDerefZvalPtr());
}

/**
 * Constructor for null ptr
 *
 * @param value
 */
Variant::Variant(const std::nullptr_t value) : Variant()
{}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(const std::int8_t value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_LONG(getUnDerefZvalPtr(), value);
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(const std::int16_t value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_LONG(getUnDerefZvalPtr(), value);
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(const std::int32_t value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_LONG(getUnDerefZvalPtr(), value);
}

#if SIZEOF_ZEND_LONG == 8
/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(const std::int64_t value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_LONG(getUnDerefZvalPtr(), value);
}
#endif

/**
 * Constructor based on boolean value
 * 
 * @param  value
 */
Variant::Variant(const bool value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_BOOL(getUnDerefZvalPtr(), value);
}

/**
 * Constructor based on single character
 * 
 * @param  value
 */
Variant::Variant(const char value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_STRINGL(getUnDerefZvalPtr(), &value, 1);
}

/**
 * Constructor based on decimal value
 * 
 * @param  value
 */
Variant::Variant(const double value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_DOUBLE(getUnDerefZvalPtr(), value);
}

/**
 * Constructor based on string value
 * 
 * @param  value
 */
Variant::Variant(const std::string &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_STRINGL(getUnDerefZvalPtr(), value.c_str(), value.size());
}

/**
 * Constructor based on a byte array
 * 
 * @param  value
 * @param  size
 */
Variant::Variant(const char *value, size_t size)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   if (value != nullptr) {
      ZVAL_STRINGL(getUnDerefZvalPtr(), value, size);
   } else {
      ZVAL_NULL(getUnDerefZvalPtr());
   }
}

Variant::Variant(const char *value)
   : Variant(value, std::strlen(value))
{}

Variant::Variant(const StdClass &nativeObject)
{
   zend_object *zobject = nativeObject.m_implPtr->m_zendObject;
   if (nullptr == zobject) {
      throw FatalError("Can't construct from an unregister StdClass Object");
   }
   zval *self = getUnDerefZvalPtr();
   ZVAL_OBJ(self, zobject);
   Z_ADDREF_P(self);
}

Variant::Variant(const Variant &other)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   stdCopyZval(getUnDerefZvalPtr(), const_cast<zval *>(other.getZvalPtr()));
}

/**
 * Wrap object around zval
 * 
 * @param  zval        Variant to wrap
 * @param  ref         Force this to be a reference
 */
Variant::Variant(zval *value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (nullptr != value) {
      if (!isRef) {
         ZVAL_COPY(self, value);
      } else {
         ZVAL_MAKE_REF(value);
         zend_reference *ref = Z_REF_P(value);
         ++GC_REFCOUNT(ref);
         ZVAL_REF(self, ref);
      }
   } else {
      ZVAL_NULL(self);
   }
}

Variant::Variant(zval &value, bool isRef)
   :Variant(&value, isRef)
{}

Variant::Variant(zval &&value, bool isRef)
   :Variant(&value, isRef)
{}

Variant::Variant(Variant &other, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      stdCopyZval(self, const_cast<zval *>(other.getZvalPtr()));
   } else {
      zval *source = other.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(BoolVariant &value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      ZVAL_BOOL(self, value.toBool());
   } else {
      zval *source = value.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(NumericVariant &value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      ZVAL_LONG(self, value.toLong());
   } else {
      zval *source = value.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(DoubleVariant &value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      ZVAL_DOUBLE(self, value.toDouble());
   } else {
      zval *source = value.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(StringVariant &value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      stdCopyZval(self, const_cast<zval *>(value.getZvalPtr()));
   } else {
      zval *source = value.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(ArrayVariant &value, bool isRef)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      stdCopyZval(self, const_cast<zval *>(value.getZvalPtr()));
   } else {
      zval *source = value.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

Variant::Variant(const BoolVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_BOOL(getUnDerefZvalPtr(), value.toBool());
}

Variant::Variant(const NumericVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_LONG(getUnDerefZvalPtr(), value.toLong());
}

Variant::Variant(const DoubleVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   ZVAL_DOUBLE(getUnDerefZvalPtr(), value.toDouble());
}

Variant::Variant(const StringVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   stdCopyZval(getUnDerefZvalPtr(), const_cast<zval *>(value.getZvalPtr()));
}

Variant::Variant(const ArrayVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   stdCopyZval(getUnDerefZvalPtr(), const_cast<zval *>(value.getZvalPtr()));
}

Variant::Variant(const ObjectVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   stdCopyZval(getUnDerefZvalPtr(), const_cast<zval *>(value.getZvalPtr()));
}

Variant::Variant(const CallableVariant &value)
   : m_implPtr(new VariantPrivate, std_zval_deleter)
{
   stdCopyZval(getUnDerefZvalPtr(), const_cast<zval *>(value.getZvalPtr()));
}

/**
 * Move constructor
 * 
 * @param  value
 */
Variant::Variant(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   // maybe this is the only way to implement pass argument by ref
   m_implPtr = std::move(other.m_implPtr);
}

Variant::Variant(BoolVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(NumericVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(StringVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(DoubleVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(ArrayVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(ObjectVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::Variant(CallableVariant &&value)
{
   m_implPtr = std::move(value.m_implPtr);
}

Variant::~Variant() ZAPI_DECL_NOEXCEPT
{}

Variant &Variant::operator =(zval *value)
{
   stdAssignZval(getZvalPtr(), value);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(const Variant &value)
{
   if (this != &value) {
      return operator =(const_cast<zval *>(value.getZvalPtr()));
   }
   return *this;
}

Variant &Variant::operator =(Variant &&value) ZAPI_DECL_NOEXCEPT
{
   assert(this != &value);
   if (getUnDerefType() != Type::Reference) {
      m_implPtr = std::move(value.m_implPtr);
      if (getUnDerefType() == Type::Reference) {
         selfDeref(getUnDerefZvalPtr());
      }
   } else {
      operator =(const_cast<zval *>(value.getZvalPtr()));
   }
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(std::nullptr_t value)
{
   zval temp;
   ZVAL_NULL(&temp);
   return operator =(&temp);
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(std::int8_t value)
{
   zval temp;
   ZVAL_LONG(&temp, value);
   return operator =(&temp);
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(std::int16_t value)
{
   zval temp;
   ZVAL_LONG(&temp, value);
   return operator =(&temp);
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(std::int32_t value)
{
   zval temp;
   ZVAL_LONG(&temp, value);
   return operator =(&temp);
}

#if SIZEOF_ZEND_LONG == 8

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(std::int64_t value)
{
   zval temp;
   ZVAL_LONG(&temp, value);
   return operator =(&temp);
}
#endif

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(bool value)
{
   zval temp;
   ZVAL_BOOL(&temp, value);
   return operator =(&temp);
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(char value)
{
   zval temp;
   ZVAL_STRINGL(&temp, &value, 1);
   operator =(&temp);
   zval_dtor(&temp); // standard free, no cycle check
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(const std::string &value)
{
   zval temp;
   if (value.size() > 0) {
      ZVAL_STRINGL(&temp, value.c_str(), value.size());
   } else {
      ZVAL_EMPTY_STRING(&temp);
   }
   operator =(&temp);
   zval_dtor(&temp); // standard free, no cycle check
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(const char *value)
{
   zval temp;
   if (value) {
      ZVAL_STRINGL(&temp, value, std::strlen(value));
   } else {
      ZVAL_EMPTY_STRING(&temp);
   }
   operator =(&temp);
   zval_dtor(&temp); // standard free, no cycle check
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator =(double value)
{
   zval temp;
   ZVAL_DOUBLE(&temp, value);
   return operator =(&temp);
}

bool Variant::operator ==(const Variant &other) const
{
   return fast_equal_check_function(const_cast<zval *>(getZvalPtr()), const_cast<zval *>(other.getZvalPtr()));
}

bool Variant::operator !=(const Variant &other) const
{
   return !fast_equal_check_function(const_cast<zval *>(getZvalPtr()), const_cast<zval *>(other.getZvalPtr()));
}

bool Variant::strictEqual(const Variant &other) const
{
   return fast_is_identical_function(const_cast<zval *>(getZvalPtr()), const_cast<zval *>(other.getZvalPtr()));
}

bool Variant::strictNotEqual(const Variant &other) const
{
   return fast_is_not_identical_function(const_cast<zval *>(getZvalPtr()), const_cast<zval *>(other.getZvalPtr()));
}

void Variant::stdCopyZval(zval *dest, zval *source)
{
   // make sure what we are copied is not a reference
   ZVAL_DEREF(source);
   // copy the value
   ZVAL_COPY(dest, source);
}

void Variant::stdAssignZval(zval *dest, zval *source)
{
   if (Z_ISREF_P(source)) {
      source = Z_REFVAL_P(source);
   }
   if (Z_ISREF_P(dest)) {
      dest = Z_REFVAL_P(dest);
   }
   if (Z_IMMUTABLE_P(dest)) {
      // throw exception here ?
      return;
   }
   // setup about dest zval *
   if (Z_REFCOUNTED_P(dest)) {
      // objects can have their own assignment handler
      if (Z_TYPE_P(dest) == IS_OBJECT && Z_OBJ_HANDLER_P(dest, set)) {
         Z_OBJ_HANDLER_P(dest, set)(dest, source);
         return;
      }
      // If dest and from are the same, there is nothing left dest do
      if (dest == source) {
         return;
      }
      // It is possible dest make IS_REF point dest another IS_REF, but that's a bug
      ZAPI_ASSERT(Z_TYPE_P(dest) != IS_REFERENCE);
      if (Z_COUNTED_P(dest)) {
         if (Z_REFCOUNT_P(dest) > 1) {
            // If reference count is greater than 1, we need dest separate zval
            // This is the optimized version of SEPARATE_ZVAL_NOREF()
            if (Z_COPYABLE_P(dest)) {
               // this will decrement the reference count and invoke GC_ZVAL_CHECK_FOR_POSSIBLE_ROOT()
               zval_ptr_dtor(dest);
               zval_copy_ctor_func(dest);
            } else {
               Z_DELREF_P(dest);
            }
         } else {
            zval_dtor(dest);
         }
      }
   }
   // Copy the source of b dest a and increment the reference count if necessary
   ZVAL_COPY(dest, source);
}

void Variant::selfDeref(zval *self)
{
   zval *orig = self;
   zval *realPtr = orig;
   ZVAL_DEREF(realPtr);
   zval temp;
   ZVAL_COPY(&temp, realPtr);
   zval_dtor(orig);
   ZVAL_COPY_VALUE(orig, &temp);
}

zval &Variant::getZval() const ZAPI_DECL_NOEXCEPT
{
   zval *ret = static_cast<zval *>(*m_implPtr);
   ZVAL_DEREF(ret);
   return *ret;
}

zval *Variant::getZvalPtr() ZAPI_DECL_NOEXCEPT
{
   zval *ret = static_cast<zval *>(*m_implPtr);
   ZVAL_DEREF(ret);
   return ret;
}

const zval *Variant::getZvalPtr() const ZAPI_DECL_NOEXCEPT
{
   zval *ret = static_cast<zval *>(*m_implPtr);
   ZVAL_DEREF(ret);
   return ret;
}

zval &Variant::getUnDerefZval() const ZAPI_DECL_NOEXCEPT
{
   return *static_cast<zval *>(*m_implPtr);
}

zval *Variant::getUnDerefZvalPtr() ZAPI_DECL_NOEXCEPT
{
   return static_cast<zval *>(*m_implPtr);
}

const zval *Variant::getUnDerefZvalPtr() const ZAPI_DECL_NOEXCEPT
{
   return static_cast<zval *>(*m_implPtr);
}

Variant::operator zval * () const
{
   return const_cast<zval *>(getZvalPtr());
}

uint32_t Variant::getRefCount() const ZAPI_DECL_NOEXCEPT
{
   if (!Z_REFCOUNTED_P(const_cast<zval *>(getUnDerefZvalPtr()))) {
      return 0;
   }
   return Z_REFCOUNT_P(const_cast<zval *>(getUnDerefZvalPtr()));
}

zval Variant::detach(bool keeprefcount)
{
   zval result;
   zval *self = getUnDerefZvalPtr();
   // copy the value
   ZVAL_COPY_VALUE(&result, self);
   if (!keeprefcount) {
      Z_TRY_DELREF_P(self);
   }
   // we no longer represent a valid value
   ZVAL_UNDEF(self);
   // done
   return result;
}

Variant Variant::makeReferenceByZval()
{
   return Variant(getUnDerefZvalPtr(), true);
}

/**
 * The type of object
 * 
 * @return Type
 */
Type Variant::getType() const ZAPI_DECL_NOEXCEPT
{
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr())));
}

Type Variant::getUnDerefType() const ZAPI_DECL_NOEXCEPT
{
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getUnDerefZvalPtr())));
}

std::string Variant::getTypeStr() const ZAPI_DECL_NOEXCEPT
{
   Type type = getType();
   switch (type) {
   case Type::Array:
      return "Array";
   case Type::False:
   case Type::True:
   case Type::Boolean:
      return "Boolean";
   case Type::Callable:
      return "Callable";
   case Type::Constant:
      return "Constant";
   case Type::Double:
      return "Double";
   case Type::Numeric:
      return "Numeric";
   case Type::Object:
      return "Object";
   case Type::Reference:
      return "Reference";
   case Type::Resource:
      return "Resource";
   case Type::String:
      return "String";
   case Type::Ptr:
      return "Pointer";
   default:
      return "Unknow";
   }
}

/**
 * Are we null? This will also check if we're a reference to a null value
 * 
 * @return bool
 */
bool Variant::isNull() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::Null) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::Null;
}

/**
 * Are we a number? This will also check if we're a reference to a number
 * 
 * @return bool
 */
bool Variant::isLong() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::Long) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::Long;
}

/**
 * Are we a boolean? This will also check if we're a reference to a boolean
 * 
 * @return bool
 */
bool Variant::isBool() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::False || getType() == Type::True) {
      return true;
   }
   Type type = getType();
   return type == Type::False || type == Type::True;
}

/**
 * Are we a string? This will also check if we're a reference to a string
 * 
 * @return bool
 */
bool Variant::isString() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::String) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::String;
}

/**
 * Are we a float? This will also check if we're a reference to a float
 * 
 * @return bool
 */
bool Variant::isDouble() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::Double) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::Double;
}

/**
 * Are we an object? This will also check if we're a reference to an object
 * 
 * @return bool
 */
bool Variant::isObject() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::Object) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::Object;
}

/**
 * Are we an array? This will also check if we're a reference to an array
 * @return bool
 */
bool Variant::isArray() const ZAPI_DECL_NOEXCEPT
{
   if (getType() == Type::Array) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(const_cast<zval *>(getZvalPtr()))) == Type::Array;
}

bool Variant::isReference() const ZAPI_DECL_NOEXCEPT
{
   return Z_TYPE_P(getUnDerefZvalPtr()) == IS_REFERENCE;
}

void Variant::invalidate() ZAPI_DECL_NOEXCEPT
{
   zval *self = getUnDerefZvalPtr();
   if (Z_TYPE_P(self) == IS_UNDEF) {
      return;
   }
   zval_ptr_dtor(self);
   ZVAL_UNDEF(self);
}

/**
 * Make a clone of the type
 * @return Variant
 */
Variant Variant::clone() const
{
   Variant output;
   ZVAL_DUP(output.getZvalPtr(), const_cast<zval *>(getZvalPtr()));
   return output;
}

/**
 * Retrieve the value as boolean
 * @return bool
 */
bool Variant::toBool() const ZAPI_DECL_NOEXCEPT
{
   switch (getType()) {
   case Type::Undefined:
      return false;
   case Type::Null:
      return false;
   case Type::False:
      return false;
   case Type::True:
      return true;
   case Type::Long:
      return zval_get_long(const_cast<zval *>(getZvalPtr()));
   case Type::Double:
      return zval_get_double(const_cast<zval *>(getZvalPtr()));
   case Type::String:
      return Z_STRLEN_P(getZvalPtr());
   case Type::Array:
      return zend_hash_num_elements(Z_ARRVAL_P(getZvalPtr()));
   default:
      return false;
   }
}

/**
 * Retrieve the value as string
 * @return string
 */
std::string Variant::toString() const ZAPI_DECL_NOEXCEPT
{
   zend_string *s  = zval_get_string(const_cast<zval *>(getZvalPtr()));
   std::string ret(ZSTR_VAL(s));
   zend_string_release(s);
   return ret;
}

/**
 * Custom output stream operator
 * @param  stream
 * @param  value
 * @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const Variant &value)
{
   return stream << value.toString();
}

} // ds
} // zapi
