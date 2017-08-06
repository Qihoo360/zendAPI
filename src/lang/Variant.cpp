// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by zzu_softboy on 06/06/2017.

#include "php/Zend/zend_compile.h"
#include "php/Zend/zend_closures.h"
#include "php/Zend/zend_string.h"
#include "php/Zend/zend_alloc.h"

#include "zapi/kernel/FatalError.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/lang/Variant.h"
#include "zapi/lang/internal/VariantPrivate.h"
#include "zapi/utils/LowerCase.h"
#include "zapi/ds/String.h"

#include <cstring>
#include <iostream>

namespace zapi
{
namespace lang
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

using zapi::utils::LowerCase;
using zapi::ds::String;
using internal::VariantPrivate;

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
   : m_implPtr(new VariantPrivate)
{
   ZVAL_NULL(*m_implPtr);
}

/**
 * Constructor for null ptr
 *
 * @param value
 */
Variant::Variant(std::nullptr_t value) : Variant()
{}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::int16_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, value);
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::int32_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, value);
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::int64_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, value);
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::uint16_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, static_cast<std::int16_t>(value));
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::uint32_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, static_cast<std::int32_t>(value));
}

/**
 * Constructor based on integer value
 * 
 * @param  value
 */
Variant::Variant(std::uint64_t value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_LONG(*m_implPtr, static_cast<std::int64_t>(value));
}

/**
 * Constructor based on boolean value
 * 
 * @param  value
 */
Variant::Variant(bool value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_BOOL(*m_implPtr, value);
}

/**
 * Constructor based on single character
 * 
 * @param  value
 */
Variant::Variant(char value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_STRINGL(*m_implPtr, &value, 1);
}

/**
 * Constructor based on string value
 * 
 * @param  value
 */
Variant::Variant(const std::string &value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_STRINGL(*m_implPtr, value.c_str(), value.size());
}

/**
 * Constructor based on a byte array
 * 
 * @param  value
 * @param  size
 */
Variant::Variant(const char *value, size_t size)
   : m_implPtr(new VariantPrivate)
{
   if (value != nullptr) {
      ZVAL_STRINGL(*m_implPtr, value, size);
   } else {
      ZVAL_NULL(*m_implPtr);
   }
}

Variant::Variant(const char *value)
   : Variant(value, std::strlen(value))
{
}


/**
 * Constructor based on decimal value
 * 
 * @param  value
 */
Variant::Variant(double value)
   : m_implPtr(new VariantPrivate)
{
   ZVAL_DOUBLE(*m_implPtr, value);
}

/**
 * Wrap object around zval
 * 
 * @param  zval        Value to wrap
 * @param  ref         Force this to be a reference
 */
Variant::Variant(zval *value, bool isRef)
   : m_implPtr(new VariantPrivate)
{
   if (!isRef) {
      ZVAL_DUP(*m_implPtr, value);
   } else {
      ZVAL_MAKE_REF(value);
      zend_reference *ref = Z_REF_P(value);
      ++GC_REFCOUNT(ref);
      ZVAL_REF(*m_implPtr, ref);
   }
}

/**
 * Copy constructor
 * 
 * @param  value
 */
Variant::Variant(const Variant &other)
   : m_implPtr(new VariantPrivate)
{
   zval *from = const_cast<zval *>(static_cast<const zval *>(*other.m_implPtr));
   // make sure what we are copied is not a reference
   ZVAL_DEREF(from);
   // copy the value
   ZVAL_COPY(*m_implPtr, from);
}

/**
 * Move constructor
 * 
 * @param  value
 */
Variant::Variant(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   m_implPtr = std::move(other.m_implPtr);
}

Variant::~Variant()
{
   if (m_implPtr) {
      zval_ptr_dtor(*m_implPtr);
   }
}

/**
 * Move operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(Variant &&value) ZAPI_DECL_NOEXCEPT
{
   if (this == &value) {
      return *this;
   }
   m_implPtr = std::move(value.m_implPtr);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const Variant &value)
{
   return operator=(const_cast<zval *>(static_cast<const zval *>(*(value.m_implPtr))));
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::nullptr_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_NULL(*m_implPtr);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int16_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_LONG(*m_implPtr, value);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int32_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_LONG(*m_implPtr, value);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int64_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_LONG(*m_implPtr, value);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::uint16_t value)
{
   return operator=(static_cast<std::int16_t>(value));
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::uint32_t value)
{
   return operator=(static_cast<std::int32_t>(value));
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::uint64_t value)
{
   return operator=(static_cast<std::int64_t>(value));
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(bool value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_BOOL(*m_implPtr, value);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(char value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_STRINGL(*m_implPtr, &value, 1);
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const std::string &value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_STRINGL(*m_implPtr, value.c_str(), value.size());
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const char *value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_STRINGL(*m_implPtr, value, std::strlen(value));
   return *this;
}

/**
 * Assignment operator
 * 
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(double value)
{
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   zval_dtor(*m_implPtr);
   ZVAL_DOUBLE(*m_implPtr, value);
   return *this;
}

Variant &Variant::operator=(struct _zval_struct *value)
{
   zval *to = static_cast<zval *>(*m_implPtr);
   if (Z_ISREF_P(value)) {
      value = Z_REFVAL_P(value);
   }
   if (Z_ISREF_P(to)) {
      to = Z_REFVAL_P(to);
   }
   if (Z_IMMUTABLE_P(to)) {
      // throw exception here ?
      return *this;
   }
   // setup about to zval *
   if (Z_REFCOUNTED_P(to)) {
      // objects can have their own assignment handler
      if (Z_TYPE_P(to) == IS_OBJECT && Z_OBJ_HANDLER_P(to, set)) {
         Z_OBJ_HANDLER_P(to, set)(to, value);
         return *this;
      }
      // If to and from are the same, there is nothing left to do
      if (to == value) {
         return *this;
      }
      // It is possible to make IS_REF point to another IS_REF, but that's a bug
      ZAPI_ASSERT(Z_TYPE_P(to) != IS_REFERENCE);
      if (Z_REFCOUNT_P(to) > 1) {
         // If reference count is greater than 1, we need to separate zval
         // This is the optimized version of SEPARATE_ZVAL_NOREF()
         if (Z_COPYABLE_P(to)) {
            // this will decrement the reference count and invoke GC_ZVAL_CHECK_FOR_POSSIBLE_ROOT()
            zval_ptr_dtor(to);
            zval_copy_ctor_func(to);
         }
      } else {
         zval_dtor(to);
      }
   }
   // Copy the value of b to a and increment the reference count if necessary
   ZVAL_COPY(to, value);
   return *this;
}

zval &Variant::getZval()
{
   return *static_cast<zval *>(*m_implPtr);
}

Variant::operator zval * () const
{
   return const_cast<zval *>(static_cast<const zval *>(*m_implPtr));
}

uint32_t Variant::refcount() const
{
   if (!Z_REFCOUNTED_P(const_cast<zval *>(static_cast<const zval *>(*m_implPtr)))) {
      return 0;
   }
   return Z_REFCOUNT_P(const_cast<zval *>(static_cast<const zval *>(*m_implPtr)));
}

zval Variant::detach(bool keeprefcount)
{
   zval result;
   // copy the value
   ZVAL_COPY_VALUE(&result, *m_implPtr);
   if (!keeprefcount) {
      Z_TRY_DELREF_P(*m_implPtr);
   }
   // we no longer represent a valid value
   ZVAL_UNDEF(*(m_implPtr.get()));
   // done
   return result;
}

/**
 * The type of object
 * 
 * @return Type
 */
Type Variant::getType() const
{
   zval *ptr = const_cast<zval *>(static_cast<const zval *>(*m_implPtr));
   if (Z_ISREF_P(ptr)) {
      ptr = Z_REFVAL_P(ptr);
   }
   return static_cast<Type>(Z_TYPE_P(ptr));
}

/**
 * Are we null? This will also check if we're a reference to a null value
 * 
 * @return bool
 */
bool Variant::isNull() const
{
   if (getType() == Type::Null) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::Null;
}

/**
 * Are we a number? This will also check if we're a reference to a number
 * 
 * @return bool
 */
bool Variant::isLong() const
{
   if (getType() == Type::Long) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::Long;
}

/**
 * Are we a boolean? This will also check if we're a reference to a boolean
 * 
 * @return bool
 */
bool Variant::isBool() const
{
   if (getType() == Type::False || getType() == Type::True) {
      return true;
   }
   Type type = static_cast<Type>(Z_TYPE_P(*m_implPtr));
   return getType() == Type::False || getType() == Type::True;
}

/**
 * Are we a string? This will also check if we're a reference to a string
 * 
 * @return bool
 */
bool Variant::isString() const
{
   if (getType() == Type::String) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::String;
}

/**
 * Are we a float? This will also check if we're a reference to a float
 * 
 * @return bool
 */
bool Variant::isDouble() const
{
   if (getType() == Type::Double) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::Double;
}

/**
 * Are we an object? This will also check if we're a reference to an object
 * 
 * @return bool
 */
bool Variant::isObject() const
{
   if (getType() == Type::Object) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::Object;
}

/**
 * Are we an array? This will also check if we're a reference to an array
 * @return bool
 */
bool Variant::isArray() const
{
   if (getType() == Type::Array) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(*m_implPtr)) == Type::Array;
}

/**
 * Returns true if the variant can be converted to the type targetType, otherwise false.
 * 
 * @return bool
 */
bool Variant::canConvert(Type targetType) const
{
   switch (targetType) {
   case Type::Undefined:
   case Type::Resource:
   case Type::Constant:
   case Type::ConstantAST:
   case Type::Reference:
   case Type::Callable:
      return false;
   default:
      return true;
   }
}

/**
 * convert current variant type to target type
 * If the cast cannot be done, the variant is cleared. Returns true if the current type of the 
 * variant was successfully cast; otherwise returns false.
 *
 * @param Type typeValue
 * @return bool
 */
bool Variant::convert(Type targetType)
{
   if (this->getType() == targetType) {
      return true;
   } else if (!canConvert(targetType)) {
      return false;
   }
   SEPARATE_ZVAL_IF_NOT_REF(*m_implPtr);
   switch (targetType) {
   case Type::Null:
      convert_to_null(*m_implPtr);
      break;
   case Type::Long:
      convert_to_long(*m_implPtr);
      break;
   case Type::Double:
      convert_to_double(*m_implPtr);
      break;
   case Type::Boolean:
      convert_to_boolean(*m_implPtr);
      break;
   case Type::False:
      convert_to_boolean(*m_implPtr);
      ZVAL_FALSE(*m_implPtr);
      break;
   case Type::True:
      convert_to_boolean(*m_implPtr);
      ZVAL_TRUE(*m_implPtr);
      break;
   case Type::Array:
      convert_to_array(*m_implPtr);
      break;
   case Type::Object:
      convert_to_object(*m_implPtr);
      break;
   case Type::String:
      convert_to_string(*m_implPtr);
      break;
   default:
      return false;
   }
   return true;
}

/**
 * Make a clone of the type
 * @return Variant
 */
Variant Variant::clone() const
{
   Variant output;
   ZVAL_DUP(*output.m_implPtr, *m_implPtr);
   return output;
}

/**
 * Clone the zval to a different type
 * @param  type
 * @return Variant
 */
Variant Variant::clone(Type targetType) const
{
   Variant cloned = clone();
   if (this->getType() != targetType) {
      cloned.convert(targetType);
   }
   return cloned;
}

/**
 * Retrieve the value as integer
 * @return long
 */
std::int64_t Variant::toLong() const
{
   return zval_get_long(*m_implPtr);
}

/**
 * Retrieve the value as boolean
 * @return bool
 */
bool Variant::toBool() const
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
      return toLong();
   case Type::Double:
      return toDouble();
   case Type::String:
      return Z_STRLEN_P(*m_implPtr);
   case Type::Array:
      return zend_hash_num_elements(Z_ARRVAL_P(*m_implPtr));
   default:
      return clone(Type::Boolean).toBool();
   }
}

/**
 * Retrieve the value as string
 * @return string
 */
std::string Variant::toString() const
{
   zend_string *s  = zval_get_string(*m_implPtr);
   std::string ret(ZSTR_VAL(s), ZSTR_LEN(s));
   zend_string_release(s);
   return ret;
}

/**
 * Retrieve the value as decimal
 * @return double
 */
double Variant::toDouble() const
{
   return zval_get_double(*m_implPtr);
}

bool Variant::operator==(const Variant &value) const
{
   return operator==(const_cast<zval *>(static_cast<const zval *>(*m_implPtr)));
}

bool Variant::operator==(zval *value) const
{
   zval result;
   // run the comparison
   if (SUCCESS != compare_function(&result, const_cast<zval *>(static_cast<const zval *>(*m_implPtr)), value)) {
      return false;
   }
   // convert to boolean
   return result.value.lval == 0; 
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

} // lang
} // zapi
