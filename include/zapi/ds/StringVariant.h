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
   std::string left(size_t size) const ZAPI_DECL_NOEXCEPT;
   std::string right(size_t size) const ZAPI_DECL_NOEXCEPT;
   // modify methods
  
   StringVariant &append(const char *str);
   StringVariant &append(const char *str, size_t length);
   StringVariant &append(const char c);
   StringVariant &append(const std::string &str);
   StringVariant &append(const StringVariant &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &append(T value);
  
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
   
   zapi_long lastIndexOf(const StringVariant &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char *needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const std::string &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long lastIndexOf(char (&needle)[arrayLength], int length, zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool contains(const StringVariant &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char *needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const std::string &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool contains(char (&needle)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool startsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool startsWith(char (&str)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
 
   bool endsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool endsWith(char (&str)[arrayLength], int length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
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
StringVariant &StringVariant::append(T value)
{
   std::string temp = std::to_string(value);
   return append(temp.c_str(), temp.length());
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
bool StringVariant::contains(char (&needle)[arrayLength], int length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, length, 0, caseSensitive);
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
bool StringVariant::endsWith(char (&str)[arrayLength], int length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return endsWith(buffer.get(), caseSensitive);
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_STRING_VARIANT_PRIVATE_H