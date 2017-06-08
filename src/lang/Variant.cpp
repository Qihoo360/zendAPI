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
// Created by zzu_softboy on 06/06/2017.

#include "zapi/lang/FatalError.h"
#include "zapi/lang/Variant.h"
#include "zapi/lang/StdClass.h"
#include "zapi/vm/StdClassImpl.h"

namespace zapi
{
namespace lang
{

using zapi::vm::StdClassImpl;

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
Variant::Variant(int16_t value)
{
   ZVAL_LONG(m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(int32_t value)
{
   ZVAL_LONG(m_val, value);
}

/**
 * Constructor based on integer value
 * @param  value
 */
Variant::Variant(int64_t value)
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
   if (!isRef) {
      ZVAL_DUP(m_val, value);
   } else {
      ZVAL_MAKE_REF(value);
      zend_reference *ref = Z_REF_P(value);
      ++GC_REFCOUNT(ref);
      ZVAL_REF(m_val, ref);
   }
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
 * Creates a reference to another Value
 *
 * Value a = b.makeReference();
 *
 * is equivalent to
 *
 * $a = &$b;
 *
 * @param to Variable to which the reference should be created
 * @return Value
 */
Variant Variant::makeReference()
{
   Variant result;
   zval *from = m_val;
   zval *to = result.m_val;
   ZVAL_MAKE_REF(from);
   zend_reference *ref = Z_REF_P(from);
   GC_REFCOUNT(ref)++;
   ZVAL_REF(to, ref);
   return result;
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
 * @return Value
 */
Variant &Variant::operator=(Variant &&value)
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
      // and make sure it is no longer a reference
      ZVAL_UNDEF(m_val);
   }
   return *this;
}

/**
 * Assign a raw zval structure
 *
 * @param  value   The value to assign
 * @return Value
 */
Variant &Variant::operator=(_zval_struct *value)
{
   // the value to assign to
   zval *to = m_val;
   // Dereference values
   if (Z_ISREF_P(value)) {
      value = Z_REFVAL_P(value);
   }
   if (Z_ISREF_P(to)) {
      to = Z_REFVAL_P(to);
   }
   // check if we are allowed to update this value
   if (Z_IMMUTABLE_P(to)) {
      throw Exception("Cannot assign to an immutable variable");
   }
   // If the destination is refcounted
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
      assert(Z_TYPE_P(to) == IS_REFERENCE);
      if (Z_REFCOUNT_P(to) > 1) {
         // If reference count is greater than 1, we need to separate zval
         // This is the optimized version of SEPARATE_ZVAL_NOREF()
         if (Z_COPYABLE_P(to)) {
            // this will decrement the reference count and invoke GC_ZVAL_CHECK_FOR_POSSIBLE_ROOT()
            zval_ptr_dtor(to);
            zval_copy_ctor_func(to);
         }
      } else {
         // Destroy the current value of the variable and free up resources
         zval_dtor(to);
      }
   }
   ZVAL_COPY(to, value);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(const Variant &value)
{
   return operator=(value.m_val);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(std::nullptr_t value)
{
   zval z;
   ZVAL_NULL(&z);
   return operator=(&z);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(int16_t value)
{
   zval z;
   ZVAL_LONG(&z, value);
   return operator=(&z);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(int32_t value)
{
   zval z;
   ZVAL_LONG(&z, value);
   return operator=(&z);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(int64_t value)
{
   zval z;
   ZVAL_LONG(&z, value);
   return operator=(&z);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(bool value)
{
   zval z;
   ZVAL_BOOL(&z, value);
   return operator=(&z);
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(char value)
{
   zval z;
   ZVAL_STRINGL(&z, &value, 1);
   operator=(&z);
   zval_dtor(&z);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(const std::string &value)
{
   zval z;
   if (value.size()) {
      ZVAL_STRINGL(&z, value.c_str(), value.size());
   } else {
      ZVAL_EMPTY_STRING(&z);
   }
   operator=(&z);
   zval_dtor(&z);
   return *this;
}

/**
 * Assignment operator
 * @param  value
 * @return Value
 */
Variant &Variant::operator=(const char *value)
{
   zval z;
   if (value) {
      ZVAL_STRINGL(&z, value, std::strlen(value));
   } else {
      ZVAL_EMPTY_STRING(&z);
   }
   operator=(&z);
   zval_dtor(&z);
   return *this;
}

/**
 * Assignment operator
      * @param  value
* @return Value
*/
Variant &Variant::operator=(double value)
{
   zval z;
   ZVAL_DOUBLE(&z, value);
   return operator=(&z);
}

/**
 * Add a value to the object
 * @param  value
 * @return Value
 */


} // lang
} // zapi