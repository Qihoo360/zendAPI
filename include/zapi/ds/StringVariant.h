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
// Created by softboy on 2017/08/08.

#ifndef ZAPI_DS_INTERNAL_STRING_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_STRING_VARIANT_PRIVATE_H

#include "zapi/ds/Variant.h"
#include "zapi/utils/CommonFuncs.h"
#include "php/Zend/zend_smart_str.h"
#include <iterator>
#include <cstdlib>
#include <string>

namespace zapi
{
namespace ds
{

class ZAPI_DECL_EXPORT StringVariant final: public Variant
{
public:
   using SizeType = size_t;
   using Reference = char &;
   using ConstReference = const char &;
   using Pointer = char *;
   using ConstPointer = const char *;
   using ValueType = char;
   using Iterator = char *;
   using ConstrIterator = const char *;
   using ReverseIterator = std::reverse_iterator<Iterator>;
   using ConstReverseIterator = std::reverse_iterator<ConstrIterator>;
   using DifferenceType = zapi_ptrdiff;
protected:
   // define some usefull type alias
   using GuardValuePtrType = std::unique_ptr<StringVariant::ValueType, std::function<void(StringVariant::Pointer ptr)>>;
public:
   StringVariant();
   StringVariant(const Variant &other);
   StringVariant(Variant &&other) ZAPI_DECL_NOEXCEPT;
   StringVariant(const StringVariant &other, bool ref = false);
   StringVariant(StringVariant &&other) ZAPI_DECL_NOEXCEPT;
   StringVariant(const std::string &value);
   StringVariant(const char *value, size_t length);
   StringVariant(const char *value);
   StringVariant &operator =(const StringVariant &other);
   StringVariant &operator =(StringVariant &&other) ZAPI_DECL_NOEXCEPT;
   StringVariant &operator =(const Variant &other);
   StringVariant &operator =(Variant &&other) ZAPI_DECL_NOEXCEPT;
   StringVariant &operator =(char value);
   StringVariant &operator =(const std::string &value);
   StringVariant &operator =(const char *value);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &operator =(T value);
   StringVariant &operator +=(const char *str);
   StringVariant &operator +=(const char c);
   StringVariant &operator +=(const std::string &str);
   StringVariant &operator +=(const StringVariant &str);
   virtual bool toBool() const ZAPI_DECL_NOEXCEPT override;
   virtual std::string toString() const ZAPI_DECL_NOEXCEPT override;
   // iterator
   Iterator begin() ZAPI_DECL_NOEXCEPT;
   ConstrIterator begin() const ZAPI_DECL_NOEXCEPT;
   ConstrIterator cbegin() const ZAPI_DECL_NOEXCEPT;
   ReverseIterator rbegin() ZAPI_DECL_NOEXCEPT;
   ConstReverseIterator rbegin() const ZAPI_DECL_NOEXCEPT;
   ConstReverseIterator crbegin() const ZAPI_DECL_NOEXCEPT;
   Iterator end() ZAPI_DECL_NOEXCEPT;
   ConstrIterator end() const ZAPI_DECL_NOEXCEPT;
   ConstrIterator cend() const ZAPI_DECL_NOEXCEPT;
   ReverseIterator rend() ZAPI_DECL_NOEXCEPT;
   ConstReverseIterator rend() const ZAPI_DECL_NOEXCEPT;
   ConstReverseIterator crend() const ZAPI_DECL_NOEXCEPT;
   // conversion method
   std::string toLowerCase() const;
   std::string toUpperCase() const;
   std::string trimmed() const;
   std::string simplified() const;
   std::string left(size_t size) const;
   std::string right(size_t size) const;
   std::string leftJustified(size_t size, char fill = ' ') const;
   std::string rightJustified(size_t size, char fill = ' ') const;
   std::string substring(size_t pos, size_t length) const;
   std::string substring(size_t pos) const;
   std::string repeated(size_t times) const;
   // modify methods
   
   StringVariant &prepend(const char *str);
   StringVariant &prepend(const char c);
   StringVariant &prepend(const std::string &str);
   StringVariant &prepend(const StringVariant &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &prepend(T value);
   template<size_t arrayLength>
   StringVariant &prepend(char (&str)[arrayLength], int length);
   template<size_t arrayLength>
   StringVariant &prepend(char (&str)[arrayLength]);
   
   StringVariant &append(const char *str);
   StringVariant &append(const char c);
   StringVariant &append(const std::string &str);
   StringVariant &append(const StringVariant &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &append(T value);
   template<size_t arrayLength>
   StringVariant &append(char (&str)[arrayLength], int length);
   template<size_t arrayLength>
   StringVariant &append(char (&str)[arrayLength]);
   
   StringVariant &remove(size_t pos, size_t length);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &remove(T pos, size_t length);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &remove(T pos);
   StringVariant &remove(char c, bool caseSensitive = true);
   StringVariant &remove(const char *str, bool caseSensitive = true);
   StringVariant &remove(const std::string &str, bool caseSensitive = true);
   StringVariant &remove(const StringVariant &str, bool caseSensitive = true);
   
   StringVariant &insert(size_t pos, const char *str);
   StringVariant &insert(size_t pos, const char c);
   StringVariant &insert(size_t pos, const std::string &str);
   StringVariant &insert(size_t pos, const StringVariant &str);
   template<size_t arrayLength>
   StringVariant &insert(size_t pos, char (&str)[arrayLength], int length);
   template<size_t arrayLength>
   StringVariant &insert(size_t pos, char (&str)[arrayLength]);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, char (&str)[arrayLength], int length);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, char (&str)[arrayLength]);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                                     std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, const char *str);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                                     std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, const char c);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                                     std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, const std::string &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                                     std::is_signed<T>::value>::type>
   StringVariant &insert(T pos, const StringVariant &str);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &insert(size_t pos, T value);
   template <typename T,
             typename V,
             typename SelectorT = typename std::enable_if<std::is_integral<T>::value &&
                                                          std::is_signed<T>::value>::type,
             typename Selector = typename std::enable_if<std::is_arithmetic<V>::value>::type>
   StringVariant &insert(T pos, V value);
   
   StringVariant &clear();
   
   void resize(SizeType size);
   void resize(SizeType size, char fillChar);
   // access methods
   zapi_long indexOf(const StringVariant &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const char *needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const std::string &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const char needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long indexOf(char (&needle)[arrayLength], int length, zapi_long offset = 0,
                     bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long indexOf(char (&needle)[arrayLength], zapi_long offset = 0,
                     bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   zapi_long lastIndexOf(const StringVariant &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char *needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const std::string &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long lastIndexOf(char (&needle)[arrayLength], int length, zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long lastIndexOf(char (&needle)[arrayLength], zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool contains(const StringVariant &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char *needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const std::string &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool contains(char (&needle)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool contains(char (&needle)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool startsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool startsWith(char (&str)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool startsWith(char (&str)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool endsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool endsWith(char (&str)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool endsWith(char (&str)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   StringVariant makeReference() const;
   Reference at(SizeType pos);
   ConstReference at(SizeType pos) const;
   const char *getCStr() const ZAPI_DECL_NOEXCEPT;
   SizeType getSize() const ZAPI_DECL_NOEXCEPT;
   SizeType getLength() const ZAPI_DECL_NOEXCEPT;
   bool isEmpty() const ZAPI_DECL_NOEXCEPT;
   SizeType getCapacity() const ZAPI_DECL_NOEXCEPT;
   virtual ~StringVariant() ZAPI_DECL_NOEXCEPT;
protected:
   zend_string *getZendStringPtr();
   char *getRawStrPtr() const ZAPI_DECL_NOEXCEPT;
   constexpr size_t calculateNewStrSize(size_t length) ZAPI_DECL_NOEXCEPT;
   void strStdRealloc(zend_string *&str, size_t length);
   void strPersistentRealloc(zend_string *&str, size_t length);
   size_t strAlloc(zend_string *&str, size_t length, bool persistent);
   size_t strReAlloc(zend_string *&str, size_t length, bool persistent);
};

template <typename T, typename Selector>
StringVariant &StringVariant::prepend(T value)
{
   std::string temp = std::to_string(value);
   return prepend(temp.c_str());
}

template<size_t arrayLength>
StringVariant &StringVariant::prepend(char (&str)[arrayLength], int length)
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return prepend(buffer.get());
}

template<size_t arrayLength>
StringVariant &StringVariant::prepend(char (&str)[arrayLength])
{
   return prepend(str, arrayLength);
}

template <typename T, typename Selector>
StringVariant &StringVariant::append(T value)
{
   std::string temp = std::to_string(value);
   return append(temp.c_str());
}

template<size_t arrayLength>
StringVariant &StringVariant::append(char (&str)[arrayLength], int length)
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return append(buffer.get());
}

template<size_t arrayLength>
StringVariant &StringVariant::append(char (&str)[arrayLength])
{
   return append(str, arrayLength);
}

template <typename T, typename Selector>
StringVariant &StringVariant::remove(T pos, size_t length)
{
   size_t targetPos = 0;
   size_t selfLength = getLength();
   size_t positivePos = static_cast<size_t>(std::abs(pos));
   if (pos < 0) {
      // out of range
      if (selfLength < positivePos) {
         throw std::out_of_range("string pos out of range");
      }
      targetPos = selfLength - positivePos;
   } else {
      targetPos = static_cast<size_t>(pos);
   }
   return remove(targetPos, length);
}

template <typename T, typename Selector>
StringVariant &StringVariant::remove(T pos)
{
   return remove(pos, 1);
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const char *str)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), str);
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const char c)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), c);
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const std::string &str)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), str.c_str());
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const StringVariant &str)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), str.getCStr());
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(size_t pos, T value)
{
   std::string buffer = std::to_string(value);
   return insert(pos, buffer.c_str());
}

template <typename T, typename V, typename SelectorT, typename Selector>
StringVariant &StringVariant::insert(T pos, V value)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   std::string buffer = std::to_string(value);
   return insert(static_cast<size_t>(pos), buffer.c_str());
}

template<size_t arrayLength>
StringVariant &StringVariant::insert(size_t pos, char (&str)[arrayLength], int length)
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return insert(pos, buffer.get());
}

template<size_t arrayLength>
StringVariant &StringVariant::insert(size_t pos, char (&str)[arrayLength])
{
   return insert(pos, str, arrayLength);
}

template<typename T, 
         size_t arrayLength,
         typename Selector>
StringVariant &StringVariant::insert(T pos, char (&str)[arrayLength], int length)
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), str, length);
}

template<typename T, 
         size_t arrayLength,
         typename Selector>
StringVariant &StringVariant::insert(T pos, char (&str)[arrayLength])
{
   pos = static_cast<zapi_long>(pos);
   if (pos < 0) {
      pos += getLength();
   }
   if (pos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(pos), str, arrayLength);
}

template <typename T, typename Selector>
StringVariant &StringVariant::operator =(T value)
{
   std::string temp = std::to_string(value);
   return operator =(temp.c_str());
}

template<size_t arrayLength>
zapi_long StringVariant::indexOf(char (&needle)[arrayLength], int length, 
                                 zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), needle, length);
   buffer.get()[length] = '\0';
   return indexOf(buffer.get(), offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::indexOf(char (&needle)[arrayLength], 
                                 zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return indexOf(needle, arrayLength, offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::lastIndexOf(char (&needle)[arrayLength], int length, 
                                     zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), needle, length);
   buffer.get()[length] = '\0';
   return lastIndexOf(buffer.get(), offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::lastIndexOf(char (&needle)[arrayLength], 
                                     zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return lastIndexOf(needle, arrayLength, offset, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::contains(char (&needle)[arrayLength], int length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, length, 0, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::contains(char (&needle)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, arrayLength, 0, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::startsWith(char (&str)[arrayLength], int length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return startsWith(buffer.get(), caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::startsWith(char (&str)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return startsWith(str, arrayLength, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::endsWith(char (&str)[arrayLength], int length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return endsWith(buffer.get(), caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::endsWith(char (&str)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return endsWith(str, arrayLength, caseSensitive);
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_STRING_VARIANT_PRIVATE_H