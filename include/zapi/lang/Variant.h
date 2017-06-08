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
using zapi::vm::Zval;
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
    * Cast to a number
    * @return int32_t
    */
   operator int16_t () const
   {
      return static_cast<int16_t>(getNumericValue());
   }

   /**
    * Cast to a number
    * @return int32_t
    */
   operator int32_t () const
   {
      return static_cast<int32_t>(getNumericValue());
   }

   /**
    * Cast to a number
    * @return int64_t
    */
   operator int64_t () const
   {
      return static_cast<int64_t>(getNumericValue());
   }

   /**
    * Cast to a boolean
    * @return boolean
    */
   operator bool () const
   {
      return getBooleanValue();
   }

   /**
    * Cast to a string
    * @return string
    */
   operator std::string () const
   {
      return getStringValue();
   }

   /**
    * Cast to byte array
    *
    * Note that this only works for string values, other
    * variables return a nullptr.
    *
    * @return const char *
    */
   operator const char * () const
   {
      return getRawValue();
   }

   /**
    * Cast to a floating point
    * @return double
    */
   operator double () const
   {
      return getDoubleValue();
   }

   /**
    * Convert the object to a vector
    * @return std::vector
    */
   template <typename T>
   operator std::vector<T> () const
   {
      return getVectorValue();
   }

   /**
    * Array access operator
    * This can be used for accessing arrays
    * @param  index
    * @return Value
    */
   Variant operator[](int index) const
   {
      return get(index);
   }

   /**
    * Array access operator
    * This can be used for accessing associative arrays
    * @param  key
    * @return Value
    */
   Variant operator[](const std::string &key) const
   {
      return get(key);
   }

   /**
    * Array access operator
    * This can be used for accessing associative arrays
    * @param  key
    * @return Value
    */
   Variant operator[](const char *key) const
   {
      return get(key);
   }

   /**
    * Index by other value object
    * @param  key
    * @return HashMember<std::string>
    */
   Variant operator[](const Variant &key) const
   {
      if (key.isNumeric()) {
         return get(key.getNumericValue());
      } else if (key.isString()) {
         return get(key.getRawValue(), key.getSize());
      } else {
         return get(key.getStringValue());
      }
   }

   /**
    * Call the function in PHP
    * This call operator is only useful when the variable represents a callable
    * @return Value
    */
   Variant operator()() const;

   /**
    * Call the function - if the variable holds a callable thing
    * @param  args        Optional arguments
    * @return Value
    */
   template <typename ...Args>
   Variant operator()(Args... args) const
   {
      Variant vargs[] = { static_cast<Variant>(args)... };
      return exec(sizeof...(Args), vargs);
   }

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
   bool isDouble() const;
   bool isObject() const;
   bool isArray() const;
   bool isScalar() const
   {
      return isNull() || isNumeric() || isBoolean() || isString() || isDouble();
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
    * Retrieve the value as boolean
    * @return bool
    */
   bool getBooleanValue() const;

   /**
    * Retrieve the value as a string
    * @return string
    */
   std::string getStringValue() const;

   /**
    * Retrieve the value as decimal
    * @return double
    */
   double getDoubleValue() const;

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
      size_t count = getSize();
      result.reserve(count);
      for (size_t i = 0; i < count; i++) {
         if (!contains(i)) {
            continue;
         }
         result.push_back(get(i));
      }
      return result;
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
    * The number of members in case of an array or object
    *
    * @return int
    */
   size_t getSize() const;

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
         return contains(value.getRawValue(), value.getSize());
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

   /**
    * Get access to a certain array member
    * @param  index
    * @return Value
    */
   virtual Variant get(int index) const;

   /**
    * Get access to a certain assoc member
    * @param  key
    * @return Value
    */
   virtual Variant get(const std::string &key) const
   {
      return get(key.c_str(), key.size());
   }

   /**
    * Get access to a certain variant member
    * @param  key
    * @return Value
    */
   virtual Variant get(const Variant &key) const
   {
      if (key.isNumeric()) {
         return get(key.getNumericValue());
      }
      if (key.isString()) {
         return get(key.getRawValue(), key.getSize());
      }
      return get(key.getStringValue());
   }

   /**
    * Get access to a certain assoc member
    * @param  key
    * @param  size
    * @return Value
    */
   Variant get(const char *key, int size = -1) const;

   /**
    * Set a certain property
    * Calling this method will turn the value into an array
    * @param  index       Index of the property to set
    * @param  value       Value to set
    * @return Value       The value that was set
    */
   virtual void set(int index, const Variant &value);

   /**
    * Set a certain property
    * Calling this method will turn the value into an array
    * @param  key         Key of the property to set
    * @param  size        Size of the key
    * @param  value       Value to set
    */
   virtual void set(const char *key, int size, const Variant &value);

   /**
    * Set a certain property
    * Calling this method will turn the object into an array
    * @param  key         Key to set
    * @param  value       Value to set
    */
   virtual void set(const std::string &key, const Variant &value)
   {
      set(key.c_str(), key.size(), value);
   }

   /**
    * Overwrite the value at a certain variant index
    * @param  key
    * @param  value
    */
   virtual void set(const Variant &key, const Variant &value)
   {
      if (key.isNumeric()) {
         set(key.getNumericValue(), value);
      } else if(key.isString()) {
         set(key.getRawValue(), key.getSize(), value);
      } else {
         set(key.getStringValue(), value);
      }
   }

   /**
    * Set a certain property
    * Calling this method will turn the object into an array
    * @param  key         Key to set
    * @param  value       Value to set
    */
   void set(const char *key, const Variant &value)
   {
      set(key, std::strlen(key), value);
   }


   /**
    * Unset a member by its index
    * @param  index
    */
   virtual void unset(int index);

   /**
    * Unset a member by its key
    * @param  key
    */
   virtual void unset(const std::string &key)
   {
      unset(key.c_str(), key.size());
   }

   /**
    * Unset a member by its key
    * @param  key
    */
   virtual void unset(const Variant &key)
   {
      if (key.isNumeric()) {
         unset(key.getNumericValue());
      } else if (key.isString()) {
         unset(key.getRawValue(), key.getSize());
      } else {
         unset(key.getStringValue());
      }
   }

   /**
    * Unset by key name and length of the key
    * @param  key
    * @param  size
    */
   void unset(const char *key, int size);

   /**
    * Unset by key name and length of the key
    * @param  key
    * @param  size
    */
   void unset(const char *key)
   {
      unset(key, std::strlen(key));
   }

   /**
    * Is a method with the given name callable?
    *
    * This is only applicable when the Value contains a PHP object
    *
    * @param  name        Name of the function
    * @return boolean
    */
   bool isCallable(const char *name);

   /**
    * Call a method
    *
    * This is only applicable when the Value contains a PHP object
    *
    * @param  name        Name of the function
    * @return Value
    */
   Variant call(const char *name) const;
   Variant call(const char *name);

   /**
    *
    * Call the method - if the variable holds an object with the given method
    * @param  name        name of the method to call
    * @param  p0          The first parameter
    * @return Value
    */
   template <typename ...Args>
   Variant call(const char *name, Args&&... args) const
   {
      Variant vargs[] = { static_cast<Variant>(args)... };
      return exec(name, sizeof...(Args), vargs);
   }

   template <typename ...Args>
   Variant call(const char *name, Args&&... args)
   {
      Variant vargs[] = { static_cast<Variant>(args)... };
      return exec(name, sizeof...(Args), vargs);
   }

   /**
    * Retrieve the original implementation
    *
    * This only works for classes that were implemented using PHP-CPP,
    * it returns nullptr for all other classes
    *
    * @return Base*
    */
   StdClass *implementation() const;

   /**
    * Retrieve the original implementation
    *
    * This only works for classes that were implemented using PHP-CPP,
    * it returns nullptr for all other classes
    *
    * @return mixed
    */
   template <typename T>
   T *implementation() const
   {
      StdClass *base = implementation();
      if (!base) {
         return nullptr;
      }
      return dynamic_cast<T *>(base);
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
   bool instanceOf(const char *className, size_t size, bool allowString = false) const;
   bool instanceOf(const char *className, bool allowString = false) const
   {
      return instanceOf(className, std::strlen(className), allowString);
   }
   bool instanceOf(const std::string &className, bool allowString = false) const
   {
      return instanceOf(className.c_str(), className.size(), allowString);
   }

   /**
    * Check whether this object is derived from a certain class.
    *
    * If you set the parameter 'allowString' to true, and the Value object
    * holds a string, the string will be treated as class name.
    *
    * @param  classname   The class of which this should be an instance
    * @param  size        Length of the classname string
    * @param  allowString Is it allowed for 'this' to be a string
    * @return bool
    */
   bool derivedFrom(const char *className, size_t size, bool allowString = false) const;
   bool derivedFrom(const char *className, bool allowString = false) const
   {
      return derivedFrom(className, std::strlen(className), allowString);
   }
   bool derivedFrom(const std::string &className, bool allowString = false) const
   {
      return derivedFrom(className.c_str(), className.size(), allowString);
   }

   /**
    * @internal
    */
   std::string debugZval() const;

protected:
   /**
    * Detach the zval
    *
    * This will unlink the zval internal structure from the Value object,
    * so that the destructor will not reduce the number of references and/or
    * deallocate the zval structure. This is used for functions that have to
    * return a zval pointer, that would otherwise be deallocated the moment
    * the function returns.
    *
    * @param  keeprefcount    Keep the same refcount
    * @return zval
    */
   Zval detach(bool keepRefCount = true);

   /**
    *  Invalidate the object - so that it will not be destructed
    */
   void invalidate();

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

private:
   /**
    * all function with a number of parameters
    * @param  argc        Number of parameters
    * @param  argv        The parameters
    * @return Variant
    */
   Variant exec(int argc, Variant *argv) const;

   /**
    * Call method with a number of parameters
    * @param name Name of method to call
    * @param argc Number of parameters
    * @param argv The parameters
    * @return Value
    */
   Variant exec(const char *name, int argc, Variant *argv) const;
   Variant exec(const char *name, int argc, Variant *argv);

   /**
    * Assign a raw zval structure
    *
    * @param  value   The value to assign
    * @return Value
    */
   Variant &operator=(_zval_struct *value);

   /**
    * Refcount - the number of references to the value
    * @return int
    */
   int getRefCount() const;

   /**
    * Retrieve the class entry
    * @param  allowString Allow the 'this' object to be a string
    * @return zend_class_entry
    */
   _zend_class_entry *classEntry(bool allowString = true) const;

protected:
   Zval m_val;
};

/**
 * Custom +=, -=, *=, /=, &= operators, to update integral types with a zapi::lang::Variant
 *
 * This code looks complicated, it ensures that the operators are only
 * overloaded for integral types (int, bool, etc) - and not for complex types
 * (arrays, objects, etc)
 */
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type * = nullptr>
X &operator+=(X &x, const Variant &value)
{
   return x += static_cast<X>(value);
};

template <typename X, typename std::enable_if<std::is_integral<X>::value>::type * = nullptr>
X &operator-=(X &x, const Variant &value)
{
   return x -= static_cast<X>(value);
};

template <typename X, typename std::enable_if<std::is_integral<X>::value>::type * = nullptr>
X &operator*=(X &x, const Variant &value)
{
   return x *= static_cast<X>(value);
};

template <typename X, typename std::enable_if<std::is_integral<X>::value>::type * = nullptr>
X &operator/=(X &x, const Variant &value)
{
   return x /= static_cast<X>(value);
};

template <typename X, typename std::enable_if<std::is_integral<X>::value>::type * = nullptr>
X &operator%=(X &x, const Variant &value)
{
   return x %= static_cast<X>(value);
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
