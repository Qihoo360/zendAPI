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
#include "zapi/vm/Zval.h"
#include "zapi/lang/Type.h"

#include <vector>
#include <map>
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
   Variant(int16_t value);
   Variant(int32_t value);
   Variant(int64_t value);
   Variant(bool value);
   Variant(char value);
   Variant(const std::string &value);
   Variant(const char *value, int length = -1);
   Variant(double value);

   Variant(Type typeValue) : Variant()
   {
      setType(typeValue);
   }

   template <typename T>
   Variant(const std::vector<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         setRaw(i++, item);
      }
   }

#ifdef ZAPI_COMPILER_INITIALIZER_LISTS
   template <typename T>
   Variant(const std::initializer_list<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         setRaw(i++, item);
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
         setRaw(iter.first.c_str(), iter.first.size(), iter.second);
      }
   }

   /**
    * Wrap object around zval
    * @param  zval Zval to wrap
    * @param  ref Force this to be a reference
    */
   Variant(_zval_struct *zval, bool isRef = false);

   /**
    * Wrap around an object implemented by us
    * @param object Object to be wrapped
    */
   Variant(const StdClass *object);

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
   Variant &operator=(int16_t value);
   Variant &operator=(int32_t value);
   Variant &operator=(int64_t value);
   Variant &operator=(bool value);
   Variant &operator=(char value);
   Variant &operator=(const std::string &value);
   Variant &operator=(const char *value);
   Variant &operator=(double value);

   /**
    * Add a value to the object
    * @param  value
    * @return Value
    */
   Variant &operator+=(const Variant &value);
   Variant &operator+=(int16_t value);
   Variant &operator+=(int32_t value);
   Variant &operator+=(int64_t value);
   Variant &operator+=(bool value);
   Variant &operator+=(char value);
   Variant &operator+=(const std::string &value);
   Variant &operator+=(const char *value);
   Variant &operator+=(double value);

   /**
    * Subtract a value from the object
    * @param  value
    * @return Value
    */
   Variant &operator-=(const Variant &value);
   Variant &operator-=(int16_t value);
   Variant &operator-=(int32_t value);
   Variant &operator-=(int64_t value);
   Variant &operator-=(bool value);
   Variant &operator-=(char value);
   Variant &operator-=(const std::string &value);
   Variant &operator-=(const char *value);
   Variant &operator-=(double value);

   /**
    * Multiply the object with a certain value
    * @param  value
    * @return Value
    */
   Variant &operator*=(const Variant &value);
   Variant &operator*=(int16_t value);
   Variant &operator*=(int32_t value);
   Variant &operator*=(int64_t value);
   Variant &operator*=(bool value);
   Variant &operator*=(char value);
   Variant &operator*=(const std::string &value);
   Variant &operator*=(const char *value);
   Variant &operator*=(double value);

   /**
    * Divide the object with a certain value
    * @param  value
    * @return Value
    */
   Variant &operator/=(const Variant &value);
   Variant &operator/=(int16_t value);
   Variant &operator/=(int32_t value);
   Variant &operator/=(int64_t value);
   Variant &operator/=(bool value);
   Variant &operator/=(char value);
   Variant &operator/=(const std::string &value);
   Variant &operator/=(const char *value);
   Variant &operator/=(double value);

   /**
    * Divide the object with a certain value, and get the rest
    * @param  value
    * @return Value
    */
   Variant &operator%=(const Variant &value);
   Variant &operator%=(int16_t value);
   Variant &operator%=(int32_t value);
   Variant &operator%=(int64_t value);
   Variant &operator%=(bool value);
   Variant &operator%=(char value);
   Variant &operator%=(const std::string &value);
   Variant &operator%=(const char *value);
   Variant &operator%=(double value);

   /**
    * Addition operator
    * @param  value
    * @return Value
    */
   Variant &operator+(const Variant &value);
   Variant &operator+(int16_t value);
   Variant &operator+(int32_t value);
   Variant &operator+(int64_t value);
   Variant &operator+(bool value);
   Variant &operator+(char value);
   Variant &operator+(const std::string &value);
   Variant &operator+(const char *value);
   Variant &operator+(double value);

   /**
    * Subtraction operator
    * @param  value
    * @return Value
    */
   Variant &operator-(const Variant &value);
   Variant &operator-(int16_t value);
   Variant &operator-(int32_t value);
   Variant &operator-(int64_t value);
   Variant &operator-(bool value);
   Variant &operator-(char value);
   Variant &operator-(const std::string &value);
   Variant &operator-(const char *value);
   Variant &operator-(double value);

   /**
    * Multiplication operator
    * @param  value
    * @return Value
    */
   Variant &operator*(const Variant &value);
   Variant &operator*(int16_t value);
   Variant &operator*(int32_t value);
   Variant &operator*(int64_t value);
   Variant &operator*(bool value);
   Variant &operator*(char value);
   Variant &operator*(const std::string &value);
   Variant &operator*(const char *value);
   Variant &operator*(double value);

   /**
    * Division operator
    * @param  value
    * @return Value
    */
   Variant &operator/(const Variant &value);
   Variant &operator/(int16_t value);
   Variant &operator/(int32_t value);
   Variant &operator/(int64_t value);
   Variant &operator/(bool value);
   Variant &operator/(char value);
   Variant &operator/(const std::string &value);
   Variant &operator/(const char *value);
   Variant &operator/(double value);

   /**
    * Modulus operator
    * @param  value
    * @return Value
    */
   Variant &operator%(const Variant &value);
   Variant &operator%(int16_t value);
   Variant &operator%(int32_t value);
   Variant &operator%(int64_t value);
   Variant &operator%(bool value);
   Variant &operator%(char value);
   Variant &operator%(const std::string &value);
   Variant &operator%(const char *value);
   Variant &operator%(double value);

   /**
    * The type of object
    * @return Type
    */
   Type getType() const;

   /**
    * Change the internal type of the variable
    *
    * @param Type typeValue
    * @return Variant&
    */
   virtual Variant &setType(Type typeValue) &;

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
   bool isNumeric() const;
   bool isBoolean() const;
   bool isString() const;
   bool isFloat() const;
   bool isObject() const;
   bool isArray() const;
   bool isScalar() const
   {
      return isNull() || isNumeric() || isBoolean() || isString() || isFloat();
   }
   bool isCallable() const;

   /**
    * Get access to the raw buffer - you can use this for direct reading and
    * writing to and from the buffer. Note that this only works for string
    * variables - other variables return nullptr.
    *
    * If you are going to write to the buffer, make sure that you first call
    * the reserve() method to ensure that the buffer is big enough.
    *
    * @return char *
    */
   char *getBuffer() const;

   /**
    * Get access to the raw buffer for read operations. Note that this
    * only works for string variables - other variables return nullptr.
    *
    * @return const char *
    */
   const char *getRawValue() const;

   /**
    * Retrieve the value as number
    *
    * We force this to be a int64_t because we assume that most
    * servers run 64 bits nowadays, and because we use int32_t, int64_t
    * almost everywhere, instead of 'long' and on OSX neither of
    * these intxx_t types is defined as 'long'...
    *
    * @return int64_t
    */
   int64_t getNumericValue() const;

   /**
    * Retrieve the value as a string
    * @return string
    */
   std::string getStringValue() const;

   /**
    * Retrieve the value as decimal
    * @return double
    */
   double getFloatValue() const;

   /**
    * Convert the object to a vector
    *
    * This only works for regular arrays that are indexed by a number, start
    * with position 0 and have no empty spaces.
    *
    * @return std::vector
    */
   template <typename T>
   std::vector<T> getVectorValue() const
   {
      if (!isArray()) {
         return std::vector<T>();
      }
      std::vector<T> result;
      size_t count = size();
      result.reserve(count);
      for (size_t i = 0; i < count; i++) {

      }
   }

   /**
    * Creates a reference to another Value
    * Variant a = b.makeReference();
    * is equivalent to
    *
    * $a = &$b;
    *
    * @return
    */
   Variant makeReference();

   /**
    * Set a certain property without running any checks (you must already know
    * for sure that this is an array, and that the index is not yet in use)
    *
    * @param int index       Index of the property to set
    * @param Variant& value       Value to set
    */
   void setRaw(int index, const Variant &value);

   /**
    * Set a certain property without any checks (you must already know for
    * sure that this is either an object or an array, and that the index is
    * not yet in use)
    *
    * @param  key         Key of the property to set
    * @param  size        Size of the key
    * @param  value       Value to set
    */
   void setRaw(const char *key, int size, const Variant &value);

   /**
    * The number of members in case of an array or object
    *
    * @return int
    */
   size_t size() const;

   /**
    * Is a certain index set in the array
    * @param  index
    * @return bool
    */
   virtual bool contains(int index) const;

   /**
    * Is a certain key set in the array
    * @param  key
    * @return bool
    */
   virtual bool contains(const std::string &key) const
   {
      return contains(key.c_str(), key.size());
   }

   /**
    * Is a certain key set in the array, when that key is stored as value object
    * @param  key
    * @return bool
    */
   virtual bool contains(const Variant &value) const
   {
      if (value.isNumeric()) {
         return contains(value.getNumericValue());
      }
      if (value.isString()) {
         return contains(value.getRawValue(), value.size());
      }
      return contains(value.getStringValue());
   }

   /**
    * Is a certain key set in the array
    * @param  key
    * @param  size
    * @return bool
    */
   bool contains(const char *key, size_t size) const;

   /**
    * Is a certain key set in the array
    * @param  key
    * @param  size
    * @return bool
    */
   bool contains(const char *key) const
   {
      return contains(key, std::strlen(key));
   }
};

} // lang
} // zapi

#endif //ZAPI_LANG_VARIANT_H
