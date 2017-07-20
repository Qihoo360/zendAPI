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
// Created by softboy on 5/26/17.

#ifndef ZAPI_LANG_VARIANT_H
#define ZAPI_LANG_VARIANT_H

#include "zapi/Global.h"
#include "zapi/lang/Type.h"

#include <vector>
#include <map>
#include <cstring>
#include <string>

namespace zapi
{
namespace lang
{

class StdClass;
/**
 * Base class for values that are stored in the Zend engine. One instance
 * of the value class represents a variable that exists in user space in
 * the PHP environment, for example as global variable, local variable
 * inside a function or as a member of an object or an array.
 *
 * A value can be a scalar or a more complicated structure like an object
 * or an array.
 *
 * Internally, the Zend engine works with "zval" objects for this. These "zval"
 * object hold a reference counter and a reference setting. The PHP-CPP Value
 * class takes care of doing this, so all you need to do is use objects of
 * this class.
 *
 * @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */
class ZAPI_DECL_EXPORT Variant
{
public:
   /**
    * Empty constructor (value = NULL)
    */
   Variant();
   
   /**
    * Constructor for various types
    */
   Variant(std::nullptr_t value);
   Variant(std::int16_t value);
   Variant(std::int32_t value);
   Variant(std::int64_t value);
   Variant(std::uint16_t value);
   Variant(std::uint32_t value);
   Variant(std::uint64_t value);
   Variant(bool value);
   Variant(char value);
   Variant(const std::string &value);
   Variant(const char *value, int length = -1);
   Variant(double value);
   
   Variant(Type targetType) : Variant()
   {
      convert(targetType);
   }
   
   template <typename T>
   Variant(const std::vector<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         
      }
   }
   
#ifdef ZAPI_COMPILER_INITIALIZER_LISTS
   template <typename T>
   Variant(const std::initializer_list<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         
      }
   }
#endif
   
   /**
    * Constructor from a map (this will create an associative array)
    * @param  value
    */
   template <typename T>
   Variant(const std::map<std::string, T> &values) : Variant(Type::Array)
   {
      for (auto &iter : values)
      {
         
      }
   }
   
   /**
    * Wrap object around zval
    * @param  zval Zval to wrap
    * @param  ref Force this to be a reference
    */
   Variant(_zval_struct *zval, bool isRef = false);
   
   /**
     * Copy constructor
     * @param  value
     */
   Variant(const Variant &other);
   
   /**
    * Move constructor
    * @param  value
    */
   Variant(Variant &&other) ZAPI_DECL_NOEXCEPT;
   
   /**
    * Destructor
    */
   virtual ~Variant();
   
   /**
    * Move assignment
    * @param  value
    * @return Value
    */
   Variant &operator=(Variant &&value) ZAPI_DECL_NOEXCEPT;
   
   /**
    * Assignment operator for various types
    * @param  value
    * @return Value
    */
   Variant &operator=(std::nullptr_t value);
   Variant &operator=(const Variant &value);
   Variant &operator=(std::int16_t value);
   Variant &operator=(std::int32_t value);
   Variant &operator=(std::int64_t value);
   Variant &operator=(std::uint16_t value);
   Variant &operator=(std::uint32_t value);
   Variant &operator=(std::uint64_t value);
   Variant &operator=(bool value);
   Variant &operator=(char value);
   Variant &operator=(const std::string &value);
   Variant &operator=(const char *value);
   Variant &operator=(double value);
   Variant &operator=(zval *value);
   
   /**
    * Comparison operators for hardcoded Variant
    * @param  value
    */
   bool operator==(const Variant &value) const;
   bool operator!=(const Variant &value) const
   {
      return !operator==(value);
   }
   
   /**
    * Cast to a number
    * @return std::int32_t
    */
   operator std::int16_t () const
   {
      return static_cast<std::int16_t>(toLong());
   }
   
   /**
    * Cast to a number
    * @return std::int32_t
    */
   operator std::int32_t () const
   {
      return static_cast<std::int32_t>(toLong());
   }
   
   /**
    * Cast to a number
    * @return std::int64_t
    */
   operator std::int64_t () const
   {
      return static_cast<std::int64_t>(toLong());
   }
   
   /**
    * Cast to a boolean
    * @return boolean
    */
   operator bool () const
   {
      return toBool();
   }
   
   /**
    * Cast to a string
    * @return string
    */
   operator std::string () const
   {
      return toString();
   }
   
   /**
    * Cast to a floating point
    * @return double
    */
   operator double () const
   {
      return toDouble();
   }
   
   operator zval * () const
   {
      Z_TRY_ADDREF_P(const_cast<zval *>(&m_val));
      return const_cast<zval *>(&m_val);
   }
   
   /**
    * The type of object
    * @return Type
    */
   Type getType() const;
   
   /**
    * convert current variant type to target type
    * If the cast cannot be done, the variant is cleared. Returns true if the current type of the 
    * variant was successfully cast; otherwise returns false.
    *
    * @param Type typeValue
    * @return bool
    */
   bool convert(Type targetType);
   
   /**
    * Returns true if the variant can be converted to the type targetType, otherwise false.
    * 
    * @return bool
    */
   bool canConvert(Type targetType) const;
   
   /**
    * Make a clone of the value with the same type
    * @return Value
    */
   Variant clone() const;
   
   /**
    * Make a clone of the value with a different type
    * @param  type
    * @return Value
    */
   Variant clone(Type typeValue) const;
   
   /**
    * Check if the value is of a certain type
    * @return bool
    */
   bool isNull() const;
   bool isLong() const;
   bool isBool() const;
   bool isString() const;
   bool isDouble() const;
   bool isObject() const;
   bool isArray() const;
   bool isScalar() const
   {
      return isNull() || isLong() || isBool() || isString() || isDouble();
   }
   
   /**
    * Retrieve the value as number
    *
    * We force this to be a std::int64_t because we assume that most
    * servers run 64 bits nowadays, and because we use std::int32_t, std::int64_t
    * almost everywhere, instead of 'long' and on OSX neither of
    * these intxx_t types is defined as 'long'...
    *
    * @return std::int64_t
    */
   std::int64_t toLong() const;
   
   /**
    * Retrieve the value as boolean
    * @return bool
    */
   bool toBool() const;
   
   /**
    * Retrieve the value as a string
    * @return string
    */
   std::string toString() const;
   
   /**
    * Retrieve the value as decimal
    * @return double
    */
   double toDouble() const;
   
   inline zval &getZval()
   {
      return m_val;
   }
   
protected:
   zval m_val;
};

/**
 * Custom output stream operator
 * @param  stream
 * @param  value
 * @return ostream
 */
ZAPI_DECL_EXPORT std::ostream &operator<<(std::ostream &stream, const Variant &value);

} // lang
} // zapi

#endif //ZAPI_LANG_VARIANT_H
