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
   ZVAL_NULL(m_val);
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
   ZVAL_LONG(m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::int32_t value)
{
   ZVAL_LONG(m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(std::int64_t value)
{
   ZVAL_LONG(m_val, value);
}

/**
 * Constructor based on boolean value
 * @param  value
 */
Variant::Variant(bool value)
{
   ZVAL_BOOL(m_val, value);
}

/**
 * Constructor based on single character
 * @param  value
 */
Variant::Variant(char value)
{
   ZVAL_STRINGL(m_val, &value, 1);
}

/**
 * Constructor based on string value
 * @param  value
 */
Variant::Variant(const std::string &value)
{
   ZVAL_STRINGL(m_val, value.c_str(), value.size());
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
      ZVAL_STRINGL(m_val, value, size);
   } else {
      ZVAL_NULL(m_val);
   }
}

/**
 * Constructor based on decimal value
 * @param  value
 */
Variant::Variant(double value)
{
   ZVAL_DOUBLE(m_val, value);
}

/**
 * Wrap object around zval
 * @param  zval        Value to wrap
 * @param  ref         Force this to be a reference
 */
Variant::Variant(_zval_struct *value, bool isRef)
{
   ZVAL_DUP(m_val, value);
   if (!Z_REFCOUNTED_P(m_val)) {
      return;
   }
   if (isRef && Z_REFCOUNT_P(m_val) > 1) {
      SEPARATE_ZVAL_IF_NOT_REF(m_val);
   }
   Z_ADDREF_P(m_val);
   if (!isRef || Z_ISREF_P(m_val)) {
      return;
   }
   ZVAL_MAKE_REF(m_val);
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
   ZVAL_OBJ(m_val, impl->getZendObject());
   Z_ADDREF_P(m_val);
}

/**
 * Copy constructor
 * @param  value
 */
Variant::Variant(const Variant &other)
{
   zval *from = other.m_val;
   zval *to = m_val;
   ZVAL_DEREF(from);
   // copy the value
   ZVAL_COPY(to, from);
}

/**
 * Move constructor
 * @param  value
 */
Variant::Variant(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   ZVAL_UNDEF(m_val);
   std::swap(m_val, other.m_val);
}

Variant::~Variant()
{
   zval_ptr_dtor(m_val);
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
   ZVAL_COPY_VALUE(result, m_val);
   // should we keep the reference count?
   /// FIXME: what if reference count becomes 0?
   /// Maybe we should call zval_ptr_dtor()?
   if (!keepRefCount) {
      Z_TRY_DELREF_P(m_val);
   }
   ZVAL_UNDEF(m_val);
   return result;
}

/**
 * Invalidate the object - so that it will not be destructed
 */
void Variant::invalidate()
{
   // do nothing if object is already undefined
   if (Z_TYPE_P(m_val) == IS_UNDEF) {
      return;
   }
   zval_ptr_dtor(m_val);
   ZVAL_UNDEF(m_val);
}

/**
 * Retrieve the refcount
 * @return int
 */
int Variant::getRefCount() const
{
   if (!Z_REFCOUNTED_P(m_val))
   {
      return 0;
   }
   return Z_REFCOUNT_P(m_val);
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
   if (!Z_ISREF_P(value.m_val) && (!m_val || !Z_ISREF_P(m_val))) {
      std::swap(m_val, value.m_val);
   } else if (m_val) {
      ZVAL_COPY_VALUE(m_val, value.m_val);
   } else {
      std::swap(m_val, value.m_val);
      ZVAL_UNREF(m_val);
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
   if (Z_ISREF_P(m_val)) {
      int refCount = Z_REFCOUNT_P(m_val);
      zval_dtor(m_val);
      *m_val = *value.m_val;
      zval_copy_ctor(m_val);
      ZVAL_MAKE_REF(m_val);
      Z_SET_REFCOUNT_P(m_val, refCount);
   } else {
      zval_ptr_dtor(m_val);
      m_val = value.m_val;
      Z_TRY_ADDREF_P(m_val);
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
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_NULL(m_val);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int16_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_LONG(m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int32_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_LONG(m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(std::int64_t value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_LONG(m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(bool value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_BOOL(m_val, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(char value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_STRINGL(m_val, &value, 1);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const std::string &value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_STRINGL(m_val, value.c_str(), value.size());
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(const char *value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_STRINGL(m_val, value, std::strlen(value));
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant &Variant::operator=(double value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   zval_dtor(m_val);
   ZVAL_DOUBLE(m_val, value);
   return *this;
}

/**
 * Add a value to the object
 * @param  value
 * @return Variant
 */
Variant &Variant::operator+=(const Variant &value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(std::int16_t value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(std::int32_t value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(std::int64_t value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(bool value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(char value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(const std::string &value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(const char *value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

Variant &Variant::operator+=(double value)
{
   return Arithmetic<std::plus>(this).assign(value);
}

/**
 * Subtract a value from the object
 * @param  value
 * @return Variant
 */
Variant &Variant::operator-=(const Variant &value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(std::int16_t value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(std::int32_t value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(std::int64_t value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(bool value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(char value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(const std::string &value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(const char *value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

Variant &Variant::operator-=(double value)
{
   return Arithmetic<std::minus>(this).assign(value);
}

/**
 * Multiply the object with a certain value
 * @param  value
 * @return Variant
 */
Variant &Variant::operator*=(const Variant &value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(std::int16_t value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(std::int32_t value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(std::int64_t value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(bool value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(char value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(const std::string &value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(const char *value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

Variant &Variant::operator*=(double value)
{
   return Arithmetic<std::multiplies>(this).assign(value);
}

/**
 * Divide the object with a certain value
 * @param  value
 * @return Variant
 */
Variant &Variant::operator/=(const Variant &value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(std::int16_t value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(std::int32_t value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(std::int64_t value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(bool value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(char value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(const std::string &value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(const char *value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

Variant &Variant::operator/=(double value)
{
   return Arithmetic<std::divides>(this).assign(value);
}

/**
 * Divide the object with a certain value and get the rest
 * Note that this does not use the Arithmetic object, because no conversion between floats is necessary
 * @param  value
 * @return Variant
 */
Variant &Variant::operator%=(const Variant &value)
{
   return operator=(getNumericValue() % value.getNumericValue());
}

Variant &Variant::operator%=(std::int16_t value)
{
   return operator=(getNumericValue() % value);
}

Variant &Variant::operator%=(std::int32_t value)
{
   return operator=(getNumericValue() % value);
}

Variant &Variant::operator%=(std::int64_t value)
{
   return operator=(getNumericValue() % value);
}

Variant &Variant::operator%=(bool value)
{
   return operator=(getNumericValue() % value);
}

Variant &Variant::operator%=(char value)
{
   value = value < '0' || value > '9' ? 0 : value - '0';
   return operator=(getNumericValue() % value);
}

Variant &Variant::operator%=(const std::string &value)
{
   return operator=(getNumericValue() % std::stol(value));
}

Variant &Variant::operator%=(const char *value)
{
   return operator=(getNumericValue() % std::atol(value));
}

Variant &Variant::operator%=(double value)
{
   return operator=(getNumericValue() % static_cast<long>(value));
}

/**
 * Assignment operator
 * @param  value
 * @return Variant
 */
Variant Variant::operator+(const Variant &value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(std::int16_t value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(std::int32_t value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(std::int64_t value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(bool value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(char value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(const std::string &value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(const char *value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

Variant Variant::operator+(double value)
{
   return Arithmetic<std::plus>(this).apply(value);
}

/**
 * Subtraction operator
 * @param  value
 * @return Variant
 */
Variant Variant::operator-(const Variant &value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(std::int16_t value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(std::int32_t value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(std::int64_t value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(bool value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(char value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(const std::string &value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(const char *value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

Variant Variant::operator-(double value)
{
   return Arithmetic<std::minus>(this).apply(value);
}

/**
 * Multiplication operator
 * @param  value
 * @return Variant
 */
Variant Variant::operator*(const Variant &value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(std::int16_t value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(std::int32_t value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(std::int64_t value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(bool value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(char value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(const std::string &value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(const char *value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

Variant Variant::operator*(double value)
{
   return Arithmetic<std::multiplies>(this).apply(value);
}

/**
 * Division operator
 * @param  value
 * @return Variant
 */
Variant Variant::operator/(const Variant &value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(std::int16_t value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(std::int32_t value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(std::int64_t value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(bool value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(char value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(const std::string &value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(const char *value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

Variant Variant::operator/(double value)
{
   return Arithmetic<std::divides>(this).apply(value);
}

/**
 * Modulus operator
 * @param  value
 * @return Variant
 */
Variant Variant::operator%(const Variant &value)
{
   return Variant(getNumericValue() % value.getNumericValue());
}

Variant Variant::operator%(std::int16_t value)
{
   return Variant(getNumericValue() % value);
}

Variant Variant::operator%(std::int32_t value)
{
   return Variant(getNumericValue() % value);
}

Variant Variant::operator%(std::int64_t value)
{
   return Variant(getNumericValue() % value);
}

Variant Variant::operator%(bool value)
{
   return Variant(getNumericValue() % value);
}

Variant Variant::operator%(char value)
{
   value = value < '0' || value > '9' ? 0 : value - '0';
   return Variant(getNumericValue() % value);
}

Variant Variant::operator%(const std::string &value)
{
   return Variant(getNumericValue() % std::stol(value));
}

Variant Variant::operator%(const char *value)
{
   return Variant(getNumericValue() % std::atol(value));
}

Variant Variant::operator%(double value)
{
   return Variant(getNumericValue() % static_cast<long>(value));
}

namespace
{

/**
 * Helper function that runs the actual call
 * @param  object      The object to call it on
 * @param  method      The function or method to call
 * @param  argc        Number of arguments
 * @param  argv        The parameters
 * @return Variant
 */
Variant do_exec(const zval *object, zval *method, int argc, zval *argv)
{
   zval retval;
   zend_object *oldException = EG(exception);
   // call the function
   // we're casting the const away here, object is only const so we can call this method
   // from const methods after all..
   if (call_user_function_ex(CG(function_table), const_cast<zval *>(object),
                             method, &retval, argc, argv, 1, nullptr) != SUCCESS) {
      // throw an exception, the function does not exist
      throw Exception("Invalid call to " + Variant(method).getStringValue());
      // unreachable, but let's return at least something to prevent compiler warnings
      return nullptr;
   } else {
      // was an exception thrown inside the function? In that case we throw a C++ new exception
      // to give the C++ code the chance to catch it
      if (oldException != EG(exception) && EG(exception)) {
         throw OrigException(EG(exception));
      }
      if (Z_ISUNDEF(retval)) {
         return nullptr;
      }
      Variant result(&retval);
      // destruct the retval (this just decrements the refcounter, which is ok, because
      // it is already wrapped in a Variant so still has 1 reference)
      zval_ptr_dtor(&retval);
      return result;
   }
}

} // end of namespace

/**
 * Call the function in PHP
 * We have ten variants of this function, depending on the number of parameters
 * This call operator is only useful when the variable represents a callable
 * @param  p0-p10          Parameters of the function to be called.
 * @return Variant
 */
Variant Variant::operator()() const
{
   return do_exec(nullptr, m_val, 0, nullptr);
}

/**
 * Is a method with the given name callable?
 *
 * This is only applicable when the Value contains a PHP object
 *
 * @param name Name of the function
 * @return boolean
 */
bool Variant::isCallable(const char *name)
{
   // this only makes sense if we are an object
   if (!isObject()) {
      return false;
   }
   // get the class properties
   zend_class_entry *ce = Z_OBJCE_P(m_val);
   LowerCase methodName{ String(name) };
   if (zend_hash_exists(&ce->function_table, methodName)) {
      return true;
   }
   // can we dynamically fetch the method?
   if (Z_OBJ_HT_P(m_val)->get_method == nullptr)
   {
      return false;
   }
   // get the function
   union _zend_function *func = Z_OBJ_HT_P(m_val)->get_method(&Z_OBJ_P(m_val), methodName, nullptr);
   if (nullptr == func) {
      return false;
   }
   // i dont get this code, it is copied from the method_exists() function (but the code has
   // of course been prettified because the php guys dont know how to write good looking code)
   if (!(func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) {
      return true;
   }
   // check the result ("Returns true to the fake Closure's __invoke")
   bool result = func->common.scope == zend_ce_closure &&
         zend_string_equals_literal(methodName.getValue(), ZEND_INVOKE_FUNC_NAME);
   zend_string_release(func->common.function_name);
   zend_free_trampoline(func);
   return result;
}

/**
 * Call the method - if the variable holds an object with the given method
 * @param  name        name of the method to call
 * @return Variant
 */
Variant Variant::call(const char *name) const
{
   Variant method(name);
   return do_exec(m_val, method.m_val, 0, nullptr);
}

/**
 * Call the method - if the variable holds an object with the given method
 * @param  name        name of the method to call
 * @return Variant
 */
Variant Variant::call(const char *name)
{
   Variant method(name);
   return do_exec(m_val, method.m_val, 0, nullptr);
}

/**
 * Call function with a number of parameters
 * @param  argc        Number of parameters
 * @param  argv        The parameters
 * @return Variant
 */
Variant Variant::exec(int argc, Variant *argv) const
{
   zval params[argc];
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].m_val;
   }
   return do_exec(nullptr, m_val, argc, params);
}

/**
 * Call method with a number of parameters
 * @param  name        Name of method to call
 * @param  argc        Number of parameters
 * @param  argv        The parameters
 * @return Variant
 */
Variant Variant::exec(const char *name, int argc, Variant *argv) const
{
   // wrap the name in a Php::Value object to get a zval
   Variant method(name);
   zval params[argc];
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].m_val;
   }
   return do_exec(m_val, method.m_val, argc, params);
}

/**
 * Call method with a number of parameters
 * @param  name        Name of method to call
 * @param  argc        Number of parameters
 * @param  argv        The parameters
 * @return Variant
 */
Variant Variant::exec(const char *name, int argc, Variant *argv)
{
   // wrap the name in a Php::Value object to get a zval
   Variant method(name);
   zval params[argc];
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].m_val;
   }
   return do_exec(m_val, method.m_val, argc, params);
}

/**
 * Comparison operators== for hardcoded Value
 * @param  value
 */
bool Variant::operator==(const Variant &value) const
{
   TSRMLS_FETCH();
   zval result;
   if (SUCCESS != compare_function(&result, m_val, value.m_val TSRMLS_CC)) {
      return false;
   }
   return result.value.lval == 0;
}

/**
 * Comparison operators< for hardcoded Value
 * @param  value
 * @return bool
 */
bool Variant::operator<(const Variant &value) const
{
   TSRMLS_FETCH();
   zval result;
   if (SUCCESS != compare_function(&result, m_val, value.m_val TSRMLS_CC)) {
      return false;
   }
   return result.value.lval < 0;
}

/**
 * The type of object
 * @return Type
 */
Type Variant::getType() const
{
   return static_cast<Type>(Z_TYPE_P(m_val));
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
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::Null;
}

/**
 * Are we a number? This will also check if we're a reference to a number
 * @return bool
 */
bool Variant::isNumeric() const
{
   if (getType() == Type::Long) {
      return true;
   }
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::Long;
}

/**
 * Are we a boolean? This will also check if we're a reference to a boolean
 * @return bool
 */
bool Variant::isBoolean() const
{
   if (getType() == Type::False || getType() == Type::True) {
      return true;
   }
   Type type = static_cast<Type>(Z_TYPE_P(m_val.dereference()));
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
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::String;
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
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::Double;
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
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::Object;
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
   return static_cast<Type>(Z_TYPE_P(m_val.dereference())) == Type::Array;
}

/**
 * Change the internal type
 * @param  type
 * @return Variant
 */
Variant &Variant::setType(Type typeValue) &
{
   if (this->getType() == typeValue) {
      return *this;
   }
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   // run the conversion, when it fails we throw a fatal error which will
   // in the end result in a zend_error() call. This FatalError class is necessary
   // because a direct call to zend_error() will do a longjmp() which may not
   // clean up the C++ objects created by the extension
   switch (typeValue) {
   case Type::Undefined:
      throw FatalError{ "Cannot make a variable undefined"};
      break;
   case Type::Null:
      convert_to_null(m_val);
      break;
   case Type::Long:
      convert_to_long(m_val);
      break;
   case Type::Double:
      convert_to_double(m_val);
      break;
   case Type::Boolean:
      convert_to_boolean(m_val);
      break;
   case Type::False:
      convert_to_boolean(m_val);
      ZVAL_FALSE(m_val);
      break;
   case Type::True:
      convert_to_boolean(m_val);
      ZVAL_TRUE(m_val);
      break;
   case Type::Array:
      convert_to_array(m_val);
      break;
   case Type::Object:
      convert_to_object(m_val);
      break;
   case Type::String:
      convert_to_string(m_val);
      break;
   case Type::Resource:
      throw FatalError{"Resource types cannot be handled by the zapi library"};
      break;
   case Type::Constant:
      throw FatalError{"Constant types cannot be handled by the zapi library"};
      break;
   case Type::ConstantAST:
      throw FatalError{"ConstantAST types cannot be handled by the zapi library"};
      break;
   case Type::Callable:
      throw FatalError{"Callable types cannot be handled by the zapi library"};
      break;
   case Type::Reference:
      throw FatalError{"Reference types cannot be handled by the zapi library"};
      break;
   default:
      throw FatalError{"Reference types cannot be handled by the zapi library"};
      break;
   }
   return *this;
}

/**
 * Check if the variable holds something that is callable
 * @return bool
 */
bool Variant::isCallable() const
{
   TSRMLS_FETCH();
   return zend_is_callable(m_val, 0, NULL TSRMLS_CC);
}

/**
 * Retrieve the class entry
 * @param  allowString
 * @return zend_class_entry
 */
zend_class_entry *Variant::getClassEntry(bool allowString) const
{
   TSRMLS_FETCH();
   if (isObject()) {
      return Z_OBJCE_P(m_val);
   } else {
      if (!allowString || !isString()) {
         return nullptr;
      }
      return zend_lookup_class(Z_STR_P(m_val) TSRMLS_CC);
   }
}

/**
 * Check whether this object is an instance of a certain class
 *
 * If you set the parameter 'allowString' to true, and the Value object
 * holds a string, the string will be treated as class name.
 *
 * @param  classname   The class of which this should be an instance
 * @param  size        Length of the classname string
 * @param  allowString Is it allowed for 'this' to be a string
 * @return bool
 */
bool Variant::instanceOf(const char *className, size_t size, bool allowString) const
{
   TSRMLS_FETCH();
   zend_class_entry *thisClassEntry = getClassEntry(allowString);
   if (!thisClassEntry) {
      return false;
   }
   zend_class_entry *classEntry = zend_lookup_class_ex(String(className, size), nullptr, 0 TSRMLS_CC);
   if (!classEntry) {
      return false;
   }
   return instanceof_function(thisClassEntry, classEntry TSRMLS_CC);
}

/**
 * Check whether this object is derived from a certain class
 *
 * If you set the parameter 'allowString' to true, and the Value object
 * holds a string, the string will be treated as class name.
 *
 * @param  classame   The class of which this should be an instance
 * @param  size        Length of the classname string
 * @param  allowString Is it allowed for 'this' to be a string
 * @return bool
 */
bool Variant::derivedFrom(const char *className, size_t size, bool allowString) const
{
   TSRMLS_FETCH();
   zend_class_entry *thisClassEntry = getClassEntry(allowString);
   if (!thisClassEntry) {
      return false;
   }
   zend_class_entry *classEntry = zend_lookup_class_ex(String(className, size), nullptr, 0 TSRMLS_CC);
   if (!classEntry) {
      return false;
   }
   if (thisClassEntry == classEntry) {
      return false;
   }
   return instanceof_function(thisClassEntry, classEntry TSRMLS_CC);
}

/**
 * Make a clone of the type
 * @return Variant
 */
Variant Variant::clone() const
{
   Variant output;
   ZVAL_DUP(output.m_val, m_val);
   return output;
}

/**
 * Clone the zval to a different type
 * @param  type
 * @return Variant
 */
Variant Variant::clone(Type typeValue) const
{
   Variant cloned = clone();
   if (this->getType() != typeValue) {
      cloned.setType(typeValue);
   }
   return cloned;
}

/**
 * Retrieve the value as integer
 * @return long
 */
std::int64_t  Variant::getNumericValue() const
{
   if (isNumeric()) {
      return Z_LVAL_P(m_val);
   }
   return clone(Type::Long).getNumericValue();
}

/**
 * Retrieve the value as boolean
 * @return bool
 */
bool Variant::getBooleanValue() const
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
      return getNumericValue();
   case Type::Double:
      return getDoubleValue();
   case Type::String:
      return getSize();
   case Type::Array:
      return getSize();
   default:
      return clone(Type::Boolean).getBooleanValue();
   }
}

/**
 * Retrieve the value as string
 * @return string
 */
std::string Variant::getStringValue() const
{
   zend_string *tempStr = zval_get_string(m_val);
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
      return std::to_string(getNumericValue());
   case Type::Double:
      return std::to_string(getDoubleValue());
   case Type::String:
      return {Z_STRVAL_P(m_val), Z_STRLEN_P(m_val)};
   default:
      break;
   }
   return clone(Type::String).getStringValue();
}

/**
 * Access to the raw buffer
 * @return char *
 */
char *Variant::getBuffer() const
{
   if (!isString()) {
      return nullptr;
   }
   return Z_STRVAL_P(m_val);
}

/**
 * Get access to the raw buffer for read operations. Note that this
 * only works for string variables - other variables return nullptr.
 *
 * @return const char *
 */
const char *Variant::getRawValue() const
{
   if (isString()) {
      return Z_STRVAL_P(m_val);
   }
   return nullptr;
}

/**
 * Retrieve the value as decimal
 * @return double
 */
double Variant::getDoubleValue() const
{
   if (isDouble()) {
      return Z_DVAL_P(m_val);
   }
   return clone(Type::Double).getDoubleValue();
   // why not use zval_get_double(m_val)
}

/**
 * The number of members in case of an array or object
 * @return int
 */
size_t Variant::getSize() const
{
   if (isArray()) {
      return zend_hash_num_elements(Z_ARRVAL_P(m_val));
   } else if (isObject()) {
      if (!Z_OBJ_HT_P(m_val)->count_elements) {
         return 0;
      }
      zend_long result;
      return Z_OBJ_HT_P(m_val)->count_elements(m_val, &result) == SUCCESS ? result : 0;
   } else if (isString()) {
      return Z_STRLEN_P(m_val);
   } else {
      Variant copy(*this);
      copy.setType(Type::String);
      return copy.getSize();
   }
}

/**
 * Does the array contain a certain index?
 * @param  index
 * @return bool
 */
bool Variant::contains(int index) const
{
   if (isObject() && instanceOf("ArrayAcess")) {
      return call("offsetExists", index).getBooleanValue();
   } else if (!isArray()) {
      return false;
   }
   return zend_hash_index_find(Z_ARRVAL_P(m_val.dereference()), index) != nullptr;
}

/**
 * Does the array contain a certain key
 * @param  key
 * @param  size
 * @return
 */
bool Variant::contains(const char *key, ssize_t size) const
{
   if (size < 0) {
      size = std::strlen(key);
   }
   if (isArray()) {
      return zend_hash_find(Z_ARRVAL_P(m_val.dereference()), String(key, size)) != nullptr;
   } else if (isObject()) {
      if (zend_check_property_access(Z_OBJ_P(m_val), String(key, size)) == FAILURE) {
         return false;
      }
      zend_object_has_property_t has_property = Z_OBJ_HT_P(m_val)->has_property;
      if (!has_property) {
         return false;
      }
      Variant property(key, size);
      return has_property(m_val, property.m_val, 0, nullptr);
   } else {
      return false;
   }
}

/**
 * Get access to a certain array member
 * @param  index
 * @return Variant
 */
Variant Variant::get(int index) const
{
   if (isArray()) {
      zval *result = zend_hash_index_find(Z_ARRVAL_P(m_val.dereference()), index);
      if (!result) {
         return Type::Undefined;
      }
      return result;
   } else if (isObject() && instanceOf("ArrayAccess")) {
      return call("offsetGet", index);
   } else {
      return Type::Undefined;
   }
}

/**
 * Get access to a certain assoc member
 * @param  key
 * @param  size
 * @return Variant
 */
Variant Variant::get(const char *key, ssize_t size) const
{
   if (!isArray() && !isObject()) {
      return Variant();
   }
   if (size < 0) {
      size = std::strlen(key);
   }
   if (isArray()) {
      zval* value = zend_hash_find(Z_ARRVAL_P(m_val.dereference()), String(key, size));
      return value ? Variant(value) : Variant();
   } else {
      if (size > 0  && key[0] == 0) {
         return Variant();
      }
      zval rv;
#if PHP_VERSION_ID < 70100
      zend_class_entry* scope = EG(scope);
#else
      zend_class_entry* scope = EG(fake_scope) ? EG(fake_scope) : zend_get_executed_scope();
#endif
      zval *property = zend_read_property(scope, m_val, key, size, 0, &rv);
      return Variant(property);
   }
}

/**
 * Set a certain property without performing any checks
 * This method can be used when it is already known that the object is an array
 * @param  index
 * @param  value
 * @return Variant
 */
void Variant::setRaw(int index, const Variant &value)
{
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   add_index_zval(m_val, index, value.m_val);
   Z_TRY_ADDREF_P(value.m_val);
}

/**
 * Set a certain property without running any checks
 * @param  key
 * @param  size
 * @param  value
 */
void Variant::setRaw(const char *key, int size, const Variant &value)
{
   if (!key || (size > 0 && key[0] == 0)) {
      return;
   }
   if (isObject()) {
      SEPARATE_ZVAL_IF_NOT_REF(m_val);
      // update the property
#if PHP_VERSION_ID < 70100
      zend_class_entry* scope = EG(scope);
#else
      zend_class_entry* scope = EG(fake_scope) ? EG(fake_scope) : zend_get_executed_scope();
#endif
      zend_update_property(scope, m_val, key, size, value.m_val);
   } else {
      SEPARATE_ZVAL_IF_NOT_REF(m_val);
      add_assoc_zval_ex(m_val, key, size, value.m_val);
      Z_TRY_ADDREF_P(value.m_val);
   }
}

/**
 * Set a certain property
 * @param  index
 * @param  value
 * @return Variant
 */
void Variant::set(int index, const Variant &value)
{
   zval *current;
   if (isArray() && (current = zend_hash_index_find(Z_ARRVAL_P(m_val.dereference()), index))) {
      // skip if nothing is going to change
      if (value.m_val == current) {
         return;
      }
   }
   setType(Type::Array);
   setRaw(index, value);
}

/**
 * Set a certain property
 * @param  key
 * @param  size
 * @param  value
 * @return Variant
 */
void Variant::set(const char *key, int size, const Variant &value)
{
   zval *current;
   if (isArray() && (current = zend_hash_find(Z_ARRVAL_P(m_val.dereference()), String(key, size)))) {
      if (value.m_val == current) {
         return;
      }
   }
   if (!isObject()) {
      setType(Type::Array);
   }
   setRaw(key, size, value);
}


/**
 * Unset a member by its index
 * @param  index
 */
void Variant::unset(int index)
{
   // only necessary for arrays
   if (!isArray()) return;
   
   // if this is not a reference variable, we should detach it to implement copy on write
   SEPARATE_ZVAL_IF_NOT_REF(m_val);
   
   // remove the index
   zend_hash_index_del(Z_ARRVAL_P(m_val.dereference()), index);
}

/**
 * Unset by key name and length of the key
 * @param  key
 * @param  size
 */
void Variant::unset(const char *key, int size)
{
   if (isObject()) {
      SEPARATE_ZVAL_IF_NOT_REF(m_val);
      add_property_null_ex(m_val, key, size);
   } else if (isArray()) {
      SEPARATE_ZVAL_IF_NOT_REF(m_val);
      zend_hash_del(Z_ARRVAL_P(m_val.dereference()), String(key, size));
   }
}

/**
 * Retrieve the original implementation
 *
 * This only works for classes that were implemented using PHP-CPP,
 * it returns nullptr for all other classes
 *
 * @return StdClass*
 */
StdClass *Variant::implementation() const
{
   if (!isObject()) {
      return nullptr;
   }
   return StdClassImpl::find(m_val)->getObject();
}

/**
 * This function is used in tests to make sure that the way we assign
 * variable is consistent with that of PHP.
 *
 * @internal
 */
std::string Variant::debugZval() const
{
   std::string s;
   zval* z = m_val;
   
   s = "[type=" + std::to_string(static_cast<int>(Z_TYPE_P(z)))
         + " refcounted=" + std::to_string(Z_REFCOUNTED_P(z))
         + " isref=" + std::to_string(Z_ISREF_P(z))
         + " refcount=" + std::to_string(Z_REFCOUNTED_P(z) ? Z_REFCOUNT_P(z) : 0)
         + "] "
         ;
   
   zend_string* zs = zval_get_string(z);
   s += std::string(ZSTR_VAL(zs), ZSTR_LEN(zs));
   zend_string_release(zs);
   return s;
}

/**
 * Gete the wrapper Zval object reference
 * 
 * @return 
 */
const Zval& Variant::getZval()
{
   return m_val;
}

/**
 * Custom output stream operator
 * @param  stream
 * @param  value
 * @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const Variant &value)
{
   return stream << value.getStringValue();
}

} // lang
} // zapi
