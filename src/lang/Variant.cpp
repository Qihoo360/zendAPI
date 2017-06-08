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

#include <zapi/lang/FatalError.h>
#include "zapi/lang/Variant.h"

namespace zapi
{
namespace lang
{

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
   auto *impl = object->implementation();
   if (nullptr == impl) {
      throw FatalError("Assigning an unassigned object to a variable");
   }
}

} // lang
} // zapi