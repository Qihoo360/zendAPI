// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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

#ifndef ZAPI_DS_VARIANT_H
#define ZAPI_DS_VARIANT_H

#include "zapi/Global.h"
#include "zapi/lang/Type.h"

#include <vector>
#include <map>
#include <cstring>
#include <string>

// forward declare with namespace
namespace zapi
{

namespace lang
{
class StdClass;
} // lang

namespace ds
{
namespace internal
{
class VariantPrivate;
}

class StringVariant;
class NumericVariant;
class DoubleVariant;
class ArrayVariant;
class BoolVariant;

} // ds
} // zapi
// end forward declare 

namespace zapi
{
namespace ds
{

using internal::VariantPrivate;
using zapi::lang::StdClass;
using zapi::lang::Type;

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
   Variant(const std::nullptr_t value);
   Variant(const std::int8_t value);
   Variant(const std::int16_t value);
   Variant(const std::int32_t value);
#if SIZEOF_ZEND_LONG == 8
   Variant(const std::int64_t value);
#endif
   Variant(const double value);
   Variant(const bool value);
   Variant(const char value);
   Variant(const std::string &value);
   Variant(const char *value, size_t length);
   Variant(const char *value);
   Variant(const StdClass &stdClass);
   
   /**
    * Copy constructor
    * @param  value
    */
   Variant(const Variant &other);
   Variant(Variant &other, bool isRef);
   Variant(BoolVariant &value, bool isRef);
   Variant(NumericVariant &value, bool isRef);
   Variant(DoubleVariant &value, bool isRef);
   Variant(StringVariant &value, bool isRef);
   Variant(ArrayVariant &value, bool isRef);
   
   Variant(const BoolVariant &value);
   Variant(const NumericVariant &value);
   Variant(const StringVariant &value);
   Variant(const DoubleVariant &value);
   Variant(const ArrayVariant &value);

   Variant(BoolVariant &&value);
   Variant(NumericVariant &&value);
   Variant(StringVariant &&value);
   Variant(DoubleVariant &&value);
   Variant(ArrayVariant &&value);
   template <typename T, 
             size_t arrayLength,
             typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   Variant(char (&value)[arrayLength], T length);
   template <size_t arrayLength>
   Variant(char (&value)[arrayLength]);
   /**
    * Wrap object around zval
    * @param  zval Zval to wrap
    * @param  ref Force this to be a reference
    */
   Variant(zval *value, bool isRef = false);
   Variant(zval &value, bool isRef = false);
   
   /**
    * Move constructor
    * @param  value
    */
   Variant(Variant &&other) ZAPI_DECL_NOEXCEPT;
   
   /**
    * Destructor
    */
   virtual ~Variant() ZAPI_DECL_NOEXCEPT;
   
   /**
    * Move assignment
    * @param  value
    * @return Value
    */
   Variant &operator =(const Variant &value);
   Variant &operator =(Variant &&value) ZAPI_DECL_NOEXCEPT;
   /**
    * Assignment operator for various types
    * @param  value
    * @return Value
    */
   Variant &operator =(std::nullptr_t value);
   Variant &operator =(std::int8_t value);
   Variant &operator =(std::int16_t value);
   Variant &operator =(std::int32_t value);
#if SIZEOF_ZEND_LONG == 8
   Variant &operator =(std::int64_t value);
#endif
   Variant &operator =(bool value);
   Variant &operator =(char value);
   Variant &operator =(const std::string &value);
   Variant &operator =(const char *value);
   Variant &operator =(double value);
   Variant &operator =(zval *value);
   bool operator ==(const Variant &other) const;
   bool operator !=(const Variant &other) const;
   bool strictEqual(const Variant &other) const;
   bool strictNotEqual(const Variant &other) const;
   /**
    * Cast to a boolean
    * @return boolean
    */
   virtual operator bool () const
   {
      return toBool();
   }
   
   /**
    * Cast to a string
    * @return string
    */
   virtual operator std::string () const
   {
      return toString();
   }
   
   operator zval * () const;
   
   /**
    * The type of object
    * @return Type
    */
   Type getType() const ZAPI_DECL_NOEXCEPT;
   
   /**
    * The type of object
    * @return Type
    */
   Type getUnDerefType() const ZAPI_DECL_NOEXCEPT;
   
   std::string getTypeStr() const ZAPI_DECL_NOEXCEPT;
   
   /**
    * Make a clone of the value with the same type
    * 
    * @return Value
    */
   Variant clone() const;
   
   /**
    * Check if the value is of a certain type
    * 
    * @return bool
    */
   bool isNull() const ZAPI_DECL_NOEXCEPT;
   bool isLong() const ZAPI_DECL_NOEXCEPT;
   bool isBool() const ZAPI_DECL_NOEXCEPT;
   bool isString() const ZAPI_DECL_NOEXCEPT;
   bool isDouble() const ZAPI_DECL_NOEXCEPT;
   bool isObject() const ZAPI_DECL_NOEXCEPT;
   bool isArray() const ZAPI_DECL_NOEXCEPT;
   bool isScalar() const ZAPI_DECL_NOEXCEPT
   {
      return isNull() || isLong() || isBool() || isString() || isDouble();
   }
   
   /**
    * Retrieve the value as boolean
    * 
    * @return bool
    */
   virtual bool toBool() const ZAPI_DECL_NOEXCEPT;
   
   /**
    * Retrieve the value as a string
    * 
    * @return string
    */
   virtual std::string toString() const ZAPI_DECL_NOEXCEPT;
   
   zval &getZval() const ZAPI_DECL_NOEXCEPT;
   zval *getZvalPtr() ZAPI_DECL_NOEXCEPT;
   const zval *getZvalPtr() const ZAPI_DECL_NOEXCEPT;
   zval *getUnDerefZvalPtr() ZAPI_DECL_NOEXCEPT;
   const zval *getUnDerefZvalPtr() const ZAPI_DECL_NOEXCEPT;
   uint32_t getRefCount() const ZAPI_DECL_NOEXCEPT;
   zval detach(bool keeprefcount);
   Variant makeReferenceByZval();
   bool isReference() const ZAPI_DECL_NOEXCEPT;
   void invalidate() ZAPI_DECL_NOEXCEPT;
protected:
   static void stdCopyZval(zval *dest, zval *source);
   static void stdAssignZval(zval *dest, zval *source);
protected: 
   friend class StringVariant;
   friend class NumericVariant;
   friend class DoubleVariant;
   friend class ArrayVariant;
   friend class BoolVariant;
   ZAPI_DECLARE_PRIVATE(Variant)
   std::shared_ptr<VariantPrivate> m_implPtr;
};

/**
 * Custom output stream operator
 * 
 * @param  stream
 * @param  value
 * @return ostream
 */
ZAPI_DECL_EXPORT std::ostream &operator<<(std::ostream &stream, const Variant &value);

template <typename T, 
          size_t arrayLength, 
          typename Selector>
Variant::Variant(char (&value)[arrayLength], T length)
   : Variant(reinterpret_cast<char *>(value),
             length < 0 ? arrayLength : static_cast<size_t>(length))
{}

template <size_t arrayLength>
Variant::Variant(char (&value)[arrayLength])
   : Variant(reinterpret_cast<char *>(value), arrayLength)
{}

} // ds
} // zapi

#endif //ZAPI_DS_VARIANT_H
