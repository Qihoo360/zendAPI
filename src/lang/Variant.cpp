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

#include "zapi/lang/FatalError.h"
#include "zapi/lang/Variant.h"
#include "zapi/lang/StdClass.h"
#include "zapi/lang/OrigException.h"
#include "zapi/vm/StdClassImpl.h"
#include "zapi/utils/Arithmetic.h"
#include "zapi/utils/LowerCase.h"
#include "zapi/ds/String.h"

#include <cstring>
#include <iostream>

namespace zapi
{
namespace lang
{

using zapi::vm::StdClassImpl;
using zapi::utils::Arithmetic;
using zapi::utils::LowerCase;
using zapi::ds::String;

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
{
   ZVAL_NULL(&m_val);
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
 * @param  value
 */
Variant::Variant(std::int16_t value)
{
   ZVAL_LONG(&m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::int32_t value)
{
   ZVAL_LONG(&m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::int64_t value)
{
   ZVAL_LONG(&m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::uint16_t value)
{
   ZVAL_LONG(&m_val, static_cast<std::int16_t>(value));
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::uint32_t value)
{
   ZVAL_LONG(&m_val, static_cast<std::int32_t>(value));
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::uint64_t value)
{
   ZVAL_LONG(&m_val, static_cast<std::int64_t>(value));
}

/**
 * Constructor based on boolean value
 * @param  value
 */
Variant::Variant(bool value)
{
   ZVAL_BOOL(&m_val, value);
}

/**
 * Constructor based on single character
 * @param  value
 */
Variant::Variant(char value)
{
   ZVAL_STRINGL(&m_val, &value, 1);
}

/**
 * Constructor based on string value
 * @param  value
 */
Variant::Variant(const std::string &value)
{
   ZVAL_STRINGL(&m_val, value.c_str(), value.size());
}

/**
 * Constructor based on a byte array
 * @param  value
 * @param  size
 */
Variant::Variant(const char *value, int size)
{
   if (value != nullptr) {
      if (size < 0) {
         size = std::strlen(value);
      }
      ZVAL_STRINGL(&m_val, value, size);
   } else {
      ZVAL_NULL(&m_val);
   }
}

/**
 * Constructor based on decimal value
 * @param  value
 */
Variant::Variant(double value)
{
   ZVAL_DOUBLE(&m_val, value);
}

/**
 * Wrap object around zval
 * @param  zval        Value to wrap
 * @param  ref         Force this to be a reference
 */
Variant::Variant(_zval_struct *value, bool isRef)
{
   ZVAL_DUP(&m_val, value);
   // if value is the primitive type return
   if (!Z_REFCOUNTED_P(&m_val)) {
      return;
   }
   if (isRef && Z_REFCOUNT_P(&m_val) > 1) {
      SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   }
   Z_ADDREF_P(&m_val);
   if (!isRef || Z_ISREF(m_val)) {
      return;
   }
   ZVAL_MAKE_REF(&m_val);
}

/**   
 * Wrap around an object
 * @param  object
 */
Variant::Variant(const StdClass *object)
{
   // there are two options: the object was constructed from user space,
   // and is already linked to a handle, or it was constructed from C++
   // space, and no handle does yet exist. But if it was constructed from
   // C++ space and not yet wrapped, this Value constructor should not be
   // called directly, but first via the derived Php::Object class.
   const StdClassImpl *impl = object->implementation();
   if (nullptr == impl) {
      throw FatalError("Assigning an unassigned object to a variable");
   }
   ZVAL_OBJ(&m_val, impl->getZendObject());
   Z_ADDREF_P(&m_val);
}

/**
 * Copy constructor
 * @param  value
 */
Variant::Variant(const Variant &other)
{
   zval *from = const_cast<zval *>(&other.m_val);
   ZVAL_DEREF(from);
   // copy the value
   ZVAL_COPY(&m_val, from);
}

/**
 * Move constructor
 * @param  value
 */
Variant::Variant(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   ZVAL_UNDEF(&m_val);
   std::swap(m_val, other.m_val);
}

Variant::~Variant()
{
   zval_ptr_dtor(&m_val);
}

/**
 * Detach the zval
 *
 * This will unlink the zval internal structure from the Value object,
 * so that the destructor will not reduce the number of references and/or
 * deallocate the zval structure. This is used for functions that have to
 * return a zval pointer, that would otherwise be deallocated the moment
 * the function returns.
 *
 * @param  keeprefcount
 * @return zval
 */
Zval Variant::detach(bool keepRefCount)
{
   Zval result;
   ZVAL_COPY_VALUE(result, &m_val);
   // should we keep the reference count?
   /// FIXME: what if reference count becomes 0?
   /// Maybe we should call zval_ptr_dtor()?
   if (!keepRefCount) {
      Z_TRY_DELREF_P(&m_val);
   }
   ZVAL_UNDEF(&m_val);
   return result;
}

/**
 * Retrieve the refcount
 * @return int
 */
int Variant::getRefCount()
{
   if (!Z_REFCOUNTED(m_val))
   {
      return 0;
   }
   return Z_REFCOUNT(m_val);
}

/**
 * Move operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(Variant &&value) ZAPI_DECL_NOEXCEPT
{
   if (this == &value) {
      return *this;
   }
   // if neither value is a reference we can simply swap the values
   // the other value will then destruct and reduce the refcount
   if (!Z_ISREF(value.m_val) && (!Z_ISREF(m_val))) {
      std::swap(m_val, value.m_val);
   }else if (Z_ISREF(value.m_val)){
      std::swap(m_val, value.m_val);
      ZVAL_UNREF(&m_val);
   } else {
      ZVAL_COPY_VALUE(&m_val, &value.m_val);
   } 
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const Variant &value)
{
   if (this == &value) {
      return *this;
   }
   if (Z_ISREF(m_val)) {
      int refcount = Z_REFCOUNT(m_val);
      zval_dtor(&m_val);
      m_val = value.m_val;
      zval_copy_ctor(&m_val);
      ZVAL_MAKE_REF(&m_val);
      Z_SET_REFCOUNT(m_val, refcount);
   } else {
      zval_ptr_dtor(&m_val);
      m_val = value.m_val;
      Z_TRY_ADDREF(m_val);
   }
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::nullptr_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_NULL(&m_val);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int16_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_LONG(&m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int32_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_LONG(&m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int64_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_LONG(&m_val, value);
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
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(bool value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_BOOL(&m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(char value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_STRINGL(&m_val, &value, 1);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const std::string &value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_STRINGL(&m_val, value.c_str(), value.size());
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const char *value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_STRINGL(&m_val, value, std::strlen(value));
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(double value)
{
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   zval_dtor(&m_val);
   ZVAL_DOUBLE(&m_val, value);
   return *this;
}

/**
 * The type of object
 * @return Type
 */
Type Variant::getType() const
{
   return static_cast<Type>(Z_TYPE(m_val));
}

/**
 * Are we null? This will also check if we're a reference to a null value
 * @return bool
 */
bool Variant::isNull() const
{
   if (getType() == Type::Null) {
      return true;
   }
   return static_cast<Type>(Z_TYPE(m_val)) == Type::Null;
}

/**
 * Are we a number? This will also check if we're a reference to a number
 * @return bool
 */
bool Variant::isLong() const
{
   if (getType() == Type::Long) {
      return true;
   }
   return static_cast<Type>(Z_TYPE(m_val)) == Type::Long;
}

/**
 * Are we a boolean? This will also check if we're a reference to a boolean
 * @return bool
 */
bool Variant::isBool() const
{
   if (getType() == Type::False || getType() == Type::True) {
      return true;
   }
   Type type = static_cast<Type>(Z_TYPE(m_val));
   return getType() == Type::False || getType() == Type::True;
}

/**
 * Are we a string? This will also check if we're a reference to a string
 * @return bool
 */
bool Variant::isString() const
{
   if (getType() == Type::String) {
      return true;
   }
   return static_cast<Type>(Z_TYPE(m_val)) == Type::String;
}

/**
 * Are we a float? This will also check if we're a reference to a float
 * @return bool
 */
bool Variant::isDouble() const
{
   if (getType() == Type::Double) {
      return true;
   }
   return static_cast<Type>(Z_TYPE(m_val)) == Type::Double;
}

/**
 * Are we an object? This will also check if we're a reference to an object
 * @return bool
 */
bool Variant::isObject() const
{
   if (getType() == Type::Object) {
      return true;
   }
   return static_cast<Type>(Z_TYPE(m_val)) == Type::Object;
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
   return static_cast<Type>(Z_TYPE(m_val)) == Type::Array;
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
   SEPARATE_ZVAL_IF_NOT_REF(&m_val);
   switch (targetType) {
   case Type::Null:
      convert_to_null(&m_val);
      break;
   case Type::Long:
      convert_to_long(&m_val);
      break;
   case Type::Double:
      convert_to_double(&m_val);
      break;
   case Type::Boolean:
      convert_to_boolean(&m_val);
      break;
   case Type::False:
      convert_to_boolean(&m_val);
      ZVAL_FALSE(&m_val);
      break;
   case Type::True:
      convert_to_boolean(&m_val);
      ZVAL_TRUE(&m_val);
      break;
   case Type::Array:
      convert_to_array(&m_val);
      break;
   case Type::Object:
      convert_to_object(&m_val);
      break;
   case Type::String:
      convert_to_string(&m_val);
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
   ZVAL_DUP(&output.m_val, &m_val);
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
std::int64_t  Variant::toLong() const
{
   if (isLong()) {
      return Z_LVAL(m_val);
   }
   return clone(Type::Long).toLong();
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
      return Z_STRLEN(m_val);
   case Type::Array:
      return zend_hash_num_elements(Z_ARRVAL(m_val));
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
   zend_string *tempStr = zval_get_string(const_cast<zval *>(&m_val));
   std::string ret(ZSTR_VAL(tempStr), ZSTR_LEN(tempStr));
   zend_string_release(tempStr);
   return ret;
   switch (getType()) {
   case Type::Null:
      return {};
   case Type::False:
      return "0";
   case Type::True:
      return "1";
   case Type::Long:
      return std::to_string(toLong());
   case Type::Double:
      return std::to_string(toDouble());
   case Type::String:
      return {Z_STRVAL(m_val), Z_STRLEN(m_val)};
   default:
      break;
   }
   return clone(Type::String).toString();
}

/**
 * Retrieve the value as decimal
 * @return double
 */
double Variant::toDouble() const
{
   if (isDouble()) {
      return Z_DVAL(m_val);
   }
   return clone(Type::Double).toDouble();
   // why not use zval_get_double(m_val)
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
