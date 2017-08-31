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

#ifndef ZAPI_DS_STRING_VARIANT_H
#define ZAPI_DS_STRING_VARIANT_H

#include "zapi/ds/Variant.h"
#include "zapi/utils/CommonFuncs.h"
#include "php/Zend/zend_smart_str.h"
#include <iterator>
#include <cstdlib>
#include <string>
#include <vector>

namespace zapi
{
namespace ds
{

class ArrayItemProxy;

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
   StringVariant(const StringVariant &other);
   StringVariant(StringVariant &other, bool isRef);
   StringVariant(Variant &&other);
   StringVariant(StringVariant &&other) ZAPI_DECL_NOEXCEPT;
   StringVariant(const std::string &value);
   StringVariant(const char *value, size_t length);
   StringVariant(const char *value);
   
   StringVariant(zval &other, bool isRef);
   StringVariant(zval &&other, bool isRef);
   StringVariant(zval *other, bool isRef);
   
   StringVariant &operator =(const StringVariant &other);
   StringVariant &operator =(const Variant &other);
   StringVariant &operator =(ArrayItemProxy &&other);
   StringVariant &operator =(char value);
   StringVariant &operator =(const std::string &value);
   StringVariant &operator =(const char *value);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &operator =(T value);
   StringVariant &operator +=(const char *str);
   StringVariant &operator +=(const char c);
   StringVariant &operator +=(const std::string &str);
   StringVariant &operator +=(const StringVariant &str);
   template <size_t arrayLength>
   StringVariant &operator +=(char (&str)[arrayLength]);
   
   Reference operator [](size_t pos);
   ConstReference operator [](size_t pos) const;
   
   // compare operators
   bool operator !=(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator !=(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator !=(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator !=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   bool operator ==(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator ==(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator ==(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator ==(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   bool operator <(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator <(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator <(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator <(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   bool operator <=(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator <=(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator <=(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator <=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   bool operator >(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator >(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator >(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator >(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   bool operator >=(const char *other) const ZAPI_DECL_NOEXCEPT;
   bool operator >=(const std::string &other) const ZAPI_DECL_NOEXCEPT;
   bool operator >=(const StringVariant &other) const ZAPI_DECL_NOEXCEPT;
   template <size_t arrayLength>
   bool operator >=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT;
   
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
   std::vector<std::string> split(char sep, bool keepEmptyParts = true, bool caseSensitive = true);
   std::vector<std::string> split(const char *sep, bool keepEmptyParts = true, bool caseSensitive = true);
   // modify methods
   
   StringVariant &prepend(const char *str);
   StringVariant &prepend(const char c);
   StringVariant &prepend(const std::string &str);
   StringVariant &prepend(const StringVariant &str);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &prepend(T value);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &prepend(char (&str)[arrayLength], T length);
   template<size_t arrayLength>
   StringVariant &prepend(char (&str)[arrayLength]);
   
   StringVariant &append(const char *str);
   StringVariant &append(const char c);
   StringVariant &append(const std::string &str);
   StringVariant &append(const StringVariant &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &append(T value);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &append(char (&str)[arrayLength], T length);
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
   StringVariant &insert(size_t pos, char (&str)[arrayLength], size_t length);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(size_t pos, char (&str)[arrayLength], T length);
   template<size_t arrayLength>
   StringVariant &insert(size_t pos, char (&str)[arrayLength]);
   template<typename T,
            size_t arrayLength,
            typename V,
            typename SelectorT = typename std::enable_if<std::is_integral<T>::value>::type,
            typename SelectorV = typename std::enable_if<std::is_integral<V>::value>::type>
   StringVariant &insert(T pos, char (&str)[arrayLength], V length);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(T pos, char (&str)[arrayLength]);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(T pos, const char *str);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(T pos, const char c);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(T pos, const std::string &str);
   template <typename T, typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   StringVariant &insert(T pos, const StringVariant &str);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   StringVariant &insert(size_t pos, T value);
   template <typename T,
             typename V,
             typename SelectorT = typename std::enable_if<std::is_integral<T>::value>::type,
             typename Selector = typename std::enable_if<std::is_arithmetic<V>::value>::type>
   StringVariant &insert(T pos, V value);
   
   StringVariant &replace(size_t pos, size_t length, const char *replace);
   StringVariant &replace(size_t pos, size_t length, const char replace);
   StringVariant &replace(size_t pos, size_t length, const std::string &replace);
   StringVariant &replace(size_t pos, size_t length, const StringVariant &replace);
   StringVariant &replace(char search, char replaceStr, bool caseSensitive = true);
   StringVariant &replace(char search, const char *replaceStr, bool caseSensitive = true);
   StringVariant &replace(char search, const std::string &replaceStr, bool caseSensitive = true);
   StringVariant &replace(char search, const StringVariant &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const char *search, const char *replaceStr, bool caseSensitive = true);
   StringVariant &replace(const char *search, const std::string &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const char *search, const StringVariant &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const std::string &search, const char *replaceStr, bool caseSensitive = true);
   StringVariant &replace(const std::string &search, const std::string &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const std::string &search, const StringVariant &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const StringVariant &search, const char *replaceStr, bool caseSensitive = true);
   StringVariant &replace(const StringVariant &search, const std::string &replaceStr, bool caseSensitive = true);
   StringVariant &replace(const StringVariant &search, const StringVariant &replaceStr, bool caseSensitive = true);
   template<size_t arrayLength>
   StringVariant &replace(size_t pos, size_t length, char (&replaceArr)[arrayLength], size_t replaceLength);
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   StringVariant &replace(size_t pos, size_t length, char (&replaceArr)[arrayLength], T replaceLength);
   template<size_t arrayLength>
   StringVariant &replace(size_t pos, size_t length, char (&replaceArr)[arrayLength]);
   
   template <typename PosType,
             typename LengthType,
             typename SelectorPosType = typename std::enable_if<std::is_integral<PosType>::value>::type,
             typename SelectorLengthType = typename std::enable_if<std::is_integral<LengthType>::value>::type>
   StringVariant &replace(PosType pos, LengthType length, const char *replace);
   
   template <typename PosType,
             typename LengthType,
             size_t arrayLength,
             typename ReplaceLengthType,
             typename SelectorPosType = typename std::enable_if<std::is_integral<PosType>::value>::type,
             typename SelectorLengthType = typename std::enable_if<std::is_integral<LengthType>::value>::type,
             typename SelectorReplaceLengthType = typename std::enable_if<std::is_integral<ReplaceLengthType>::value>::type>
   StringVariant &replace(PosType pos, LengthType length, char (&replace)[arrayLength], ReplaceLengthType replaceLength);
   
   template <typename PosType,
             typename LengthType,
             size_t arrayLength,
             typename SelectorPosType = typename std::enable_if<std::is_integral<PosType>::value>::type,
             typename SelectorLengthType = typename std::enable_if<std::is_integral<LengthType>::value>::type>
   StringVariant &replace(PosType pos, LengthType length, char (&replace)[arrayLength]);
   
   StringVariant &clear();
   
   void resize(SizeType size);
   void resize(SizeType size, char fillChar);
   // access methods
   zapi_long indexOf(const StringVariant &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const char *needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const std::string &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long indexOf(const char needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long indexOf(char (&needle)[arrayLength], size_t length, zapi_long offset = 0,
                     bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<typename T,
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   zapi_long indexOf(char (&needle)[arrayLength], T length, zapi_long offset = 0,
                     bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long indexOf(char (&needle)[arrayLength], zapi_long offset = 0,
                     bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   zapi_long lastIndexOf(const StringVariant &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char *needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const std::string &needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   zapi_long lastIndexOf(const char needle, zapi_long offset = 0, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long lastIndexOf(char (&needle)[arrayLength], size_t length, zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<typename T,
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   zapi_long lastIndexOf(char (&needle)[arrayLength], T length, zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   zapi_long lastIndexOf(char (&needle)[arrayLength], zapi_long offset = 0,
                         bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool contains(const StringVariant &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char *needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const std::string &needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool contains(const char needle, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool contains(char (&needle)[arrayLength], size_t length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   bool contains(char (&needle)[arrayLength], T length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool contains(char (&needle)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool startsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool startsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool startsWith(char (&str)[arrayLength], size_t length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   bool startsWith(char (&str)[arrayLength], T length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool startsWith(char (&str)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   bool endsWith(const StringVariant &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const char *str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(const std::string &str, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   bool endsWith(char c, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool endsWith(char (&str)[arrayLength], size_t length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<typename T, 
            size_t arrayLength,
            typename Selector = typename std::enable_if<std::is_integral<T>::value &&
                                                        std::is_signed<T>::value>::type>
   bool endsWith(char (&str)[arrayLength], T length, bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   template<size_t arrayLength>
   bool endsWith(char (&str)[arrayLength], bool caseSensitive = true) const ZAPI_DECL_NOEXCEPT;
   
   Reference at(SizeType pos);
   ConstReference at(SizeType pos) const;
   const char *getCStr() const ZAPI_DECL_NOEXCEPT;
   char *getData() ZAPI_DECL_NOEXCEPT;
   const char *getData() const ZAPI_DECL_NOEXCEPT;
   SizeType getSize() const ZAPI_DECL_NOEXCEPT;
   SizeType getLength() const ZAPI_DECL_NOEXCEPT;
   bool isEmpty() const ZAPI_DECL_NOEXCEPT;
   SizeType getCapacity() const ZAPI_DECL_NOEXCEPT;
   virtual ~StringVariant() ZAPI_DECL_NOEXCEPT;
private:
   zend_string *getZendStringPtr();
   char *getRawStrPtr() const ZAPI_DECL_NOEXCEPT;
   size_t calculateNewStrSize(size_t length) ZAPI_DECL_NOEXCEPT;
   void strStdRealloc(zend_string *&str, size_t length);
   void strPersistentRealloc(zend_string *&str, size_t length);
   size_t strAlloc(zend_string *&str, size_t length, bool persistent);
   size_t strReAlloc(zend_string *&str, size_t length, bool persistent);
};

bool operator ==(const char *lhs, const StringVariant &rhs);
bool operator ==(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator ==(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      return 0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), 
                              rhs.getCStr(), std::max(arrayLength, rhs.getLength()));
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return 0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(),
                              std::max(arraySize, rhs.getLength()));
   }
}

bool operator !=(const char *lhs, const StringVariant &rhs);
bool operator !=(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator !=(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      return 0 != std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(),
                              std::max(arrayLength, rhs.getLength()));
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return 0 != std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(),
                              std::max(arraySize, rhs.getLength()));
   }
}

bool operator <(const char *lhs, const StringVariant &rhs);
bool operator <(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator <(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(),
                         std::max(arrayLength, rhs.getLength())) < 0;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(),
                         std::max(arraySize, rhs.getLength())) < 0;
   }
}

bool operator <=(const char *lhs, const StringVariant &rhs);
bool operator <=(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator <=(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      size_t len = std::max(arrayLength, rhs.getLength());;
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len) < 0 ||
            0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len);
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      size_t len = std::max(arraySize, rhs.getLength());;
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(), len) < 0 ||
            0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len);
   }
}

bool operator >(const char *lhs, const StringVariant &rhs);
bool operator >(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator >(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), 
                         std::max(arrayLength, rhs.getLength())) > 0;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(),
                         std::max(arraySize, rhs.getLength())) > 0;
   }
}

bool operator >=(const char *lhs, const StringVariant &rhs);
bool operator >=(const std::string &lhs, const StringVariant &rhs);
template<size_t arrayLength>
bool operator >=(char (&lhs)[arrayLength], const StringVariant &rhs)
{
   if (*(lhs + arrayLength - 1) == '\0') {
      size_t len = std::max(arrayLength, rhs.getLength());
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len) > 0 ||
            0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len);
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      size_t len = std::max(arraySize, rhs.getLength());
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(reinterpret_cast<StringVariant::Pointer>(buffer), rhs.getCStr(), len) > 0 ||
            0 == std::memcmp(reinterpret_cast<StringVariant::Pointer>(lhs), rhs.getCStr(), len);
   }
}

std::string operator +(const StringVariant& lhs, const StringVariant &rhs);
std::string operator +(const StringVariant& lhs, const char *rhs);
std::string operator +(const StringVariant& lhs, const std::string &rhs);
std::string operator +(const StringVariant& lhs, char rhs);

template<size_t arrayLength>
std::string operator +(const StringVariant& lhs, char (&rhs)[arrayLength])
{
   std::string ret(lhs.getCStr());
   if (*(rhs + arrayLength - 1) == '\0') {
      ret += rhs;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, rhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      ret += buffer;
   }
   return ret;
}

std::string operator +(const char *lhs, const StringVariant &rhs);
std::string operator +(const std::string &lhs, const StringVariant &rhs);
std::string operator +(char lhs, const StringVariant &rhs);

template<size_t arrayLength>
std::string operator +(char (&lhs)[arrayLength], const StringVariant& rhs)
{
   std::string ret;
   if (*(lhs + arrayLength - 1) == '\0') {
      ret += lhs;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      StringVariant::ValueType buffer[arraySize];
      std::memcpy(buffer, lhs, arrayLength);
      *(buffer + arrayLength) = '\0';
      ret += buffer;
   }
   ret += rhs.getCStr();
   return ret;
}

template <size_t arrayLength>
StringVariant &StringVariant::operator +=(char (&str)[arrayLength])
{
   return append(str, arrayLength);
}

template <size_t arrayLength>
bool StringVariant::operator ==(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      return 0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), 
                              std::max(arrayLength, getLength()));
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return 0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer),
                              std::max(arraySize, getLength()));
   }
}

template <size_t arrayLength>
bool StringVariant::operator !=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      return 0 != std::memcmp(getCStr(), reinterpret_cast<Pointer>(other),
                              std::max(arrayLength, getLength()));
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return 0 != std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer),
                              std::max(arraySize, getLength()));
   }
}

template <size_t arrayLength>
bool StringVariant::operator <(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), 
                         std::max(arrayLength, getLength())) < 0;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer),
                         std::max(arraySize, getLength())) < 0;
   }
}

template <size_t arrayLength>
bool StringVariant::operator <=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      size_t len = std::max(arrayLength, getLength());
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), len) < 0 ||
            0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), len);
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      size_t len = std::max(arraySize, getLength());
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer), len) < 0||
            0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer), len);
   }
}

template <size_t arrayLength>
bool StringVariant::operator >(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), 
                         std::max(arrayLength, getLength())) > 0;
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer), 
                         std::max(arraySize, getLength())) > 0;
   }
}

template <size_t arrayLength>
bool StringVariant::operator >=(char (&other)[arrayLength]) const ZAPI_DECL_NOEXCEPT
{
   if (*(other + arrayLength - 1) == '\0') {
      size_t len = std::max(arrayLength, getLength());
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), len) > 0 ||
            0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(other), len);
   } else {
      constexpr size_t arraySize = arrayLength + 1;
      ValueType buffer[arraySize];
      size_t len = std::max(arraySize, getLength());
      std::memcpy(buffer, other, arrayLength);
      *(buffer + arrayLength) = '\0';
      return std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer), len) > 0||
            0 == std::memcmp(getCStr(), reinterpret_cast<Pointer>(buffer), len);
   }
}

template <typename T, typename Selector>
StringVariant &StringVariant::prepend(T value)
{
   std::string temp = std::to_string(value);
   return prepend(temp.c_str());
}

template<typename T, size_t arrayLength, typename Selector>
StringVariant &StringVariant::prepend(char (&str)[arrayLength], T length)
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   len = std::min(arrayLength, len);
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(len)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, len);
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

template<typename T, size_t arrayLength, typename Selector>
StringVariant &StringVariant::append(char (&str)[arrayLength], T length)
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   len = std::min(arrayLength, len);
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(len)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, len);
   buffer.get()[len] = '\0';
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
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      // out of range
      if (selfLength < positivePos) {
         throw std::out_of_range("string pos out of range");
      }
      targetPos = selfLength - positivePos;
   } else {
      targetPos = static_cast<size_t>(lpos);
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
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(lpos), str);
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const char c)
{
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(lpos), c);
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const std::string &str)
{
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(lpos), str.c_str());
}

template <typename T, typename Selector>
StringVariant &StringVariant::insert(T pos, const StringVariant &str)
{
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(lpos), str.getCStr());
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
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   std::string buffer = std::to_string(value);
   return insert(static_cast<size_t>(lpos), buffer.c_str());
}

template<size_t arrayLength>
StringVariant &StringVariant::insert(size_t pos, char (&str)[arrayLength], size_t length)
{
   length = std::min(arrayLength, length);
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return insert(pos, buffer.get());
}

template<typename T, size_t arrayLength, typename Selector>
StringVariant &StringVariant::insert(size_t pos, char (&str)[arrayLength], T length)
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return insert(pos, str, len);
}

template<size_t arrayLength>
StringVariant &StringVariant::insert(size_t pos, char (&str)[arrayLength])
{
   return insert(pos, str, arrayLength);
}

template<typename T, 
         size_t arrayLength, 
         typename V,  
         typename SelectorT, 
         typename SelectorV>
StringVariant &StringVariant::insert(T pos, char (&str)[arrayLength], V length)
{
   size_t rlen;
   if (length < 0) {
      rlen = arrayLength;
   } else {
      rlen = static_cast<size_t>(length);
   }
   zapi_long lpos = static_cast<zapi_long>(pos);
   if (lpos < 0) {
      lpos += getLength();
   }
   if (lpos < 0) {
      throw std::out_of_range("string pos out of range");
   }
   return insert(static_cast<size_t>(lpos), str, rlen);
}

template<typename T, 
         size_t arrayLength,
         typename Selector>
StringVariant &StringVariant::insert(T pos, char (&str)[arrayLength])
{
   return insert(pos, str, arrayLength);
}

template<size_t arrayLength>
StringVariant &StringVariant::replace(size_t pos, size_t length, char (&replaceArr)[arrayLength], size_t replaceLength)
{
   replaceLength = std::min(arrayLength, replaceLength);
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(replaceLength)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), replaceArr, replaceLength);
   buffer.get()[replaceLength] = '\0';
   return replace(pos, length, buffer.get());
}

template<typename T, size_t arrayLength, typename Selector>
StringVariant &StringVariant::replace(size_t pos, size_t length, char (&replaceArr)[arrayLength], T replaceLength)
{
   size_t len;
   if (replaceLength < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(replaceLength);
   }
   return replace(pos, length, replaceArr, len);
}

template<size_t arrayLength>
StringVariant &StringVariant::replace(size_t pos, size_t length, char (&replaceArr)[arrayLength])
{
   return replace(pos, length, replaceArr, arrayLength);
}

template <typename PosType,
          typename LengthType,
          typename SelectorPosType,
          typename SelectorLengthType>
StringVariant &StringVariant::replace(PosType pos, LengthType length, const char *replace)
{
   // calculate pos
   size_t rpos;
   size_t rlength;
   size_t selfLength = getLength();
   
   if (pos < 0) {
      pos += selfLength;
      if (pos < 0) {
         throw std::out_of_range("string pos out of range");
      }
   }
   rpos = static_cast<size_t>(pos);
   if (length < 0) {
      rlength = selfLength - rpos;
   } else {
      rlength = static_cast<size_t>(length);
   }
   remove(rpos, rlength);
   insert(rpos, replace);
   return *this;
}

template <typename PosType,
          typename LengthType,
          size_t arrayLength,
          typename ReplaceLengthType,
          typename SelectorPosType,
          typename SelectorLengthType,
          typename SelectorReplaceLengthType>
StringVariant &StringVariant::replace(PosType pos, LengthType length, char (&replaceArr)[arrayLength], ReplaceLengthType replaceLength)
{
   // calculate pos
   size_t rpos;
   size_t rlength;
   size_t selfLength = getLength();
   if (pos < 0) {
      pos += selfLength;
      if (pos < 0) {
         throw std::out_of_range("string pos out of range");
      }
   }
   rpos = static_cast<size_t>(pos);
   if (length < 0) {
      rlength = selfLength - rpos;
   } else {
      rlength = static_cast<size_t>(length);
   }
   remove(rpos, rlength);
   // call StringVariant::insert(size_t pos, char (&str)[arrayLength], T length)
   insert(rpos, replaceArr, replaceLength);
   return *this;
}

template <typename PosType,
          typename LengthType,
          size_t arrayLength,
          typename SelectorPosType,
          typename SelectorLengthType>
StringVariant &StringVariant::replace(PosType pos, LengthType length, char (&replaceArr)[arrayLength])
{
   return replace(pos, length, replaceArr, arrayLength);
}

template <typename T, typename Selector>
StringVariant &StringVariant::operator =(T value)
{
   std::string temp = std::to_string(value);
   return operator =(temp.c_str());
}

template<size_t arrayLength>
zapi_long StringVariant::indexOf(char (&needle)[arrayLength], size_t length, 
                                 zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), needle, length);
   buffer.get()[length] = '\0';
   return indexOf(buffer.get(), offset, caseSensitive);
}

template<typename T, size_t arrayLength, typename Selector>
zapi_long StringVariant::indexOf(char (&needle)[arrayLength], T length, 
                                 zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return indexOf(needle, len, offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::indexOf(char (&needle)[arrayLength], 
                                 zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return indexOf(needle, arrayLength, offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::lastIndexOf(char (&needle)[arrayLength], size_t length, 
                                     zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, length);
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), needle, length);
   buffer.get()[length] = '\0';
   return lastIndexOf(buffer.get(), offset, caseSensitive);
}

template<typename T, size_t arrayLength, typename Selector>
zapi_long StringVariant::lastIndexOf(char (&needle)[arrayLength], T length, 
                                     zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return lastIndexOf(needle, len, offset, caseSensitive);
}

template<size_t arrayLength>
zapi_long StringVariant::lastIndexOf(char (&needle)[arrayLength], 
                                     zapi_long offset, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return lastIndexOf(needle, arrayLength, offset, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::contains(char (&needle)[arrayLength], size_t length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, length, 0, caseSensitive);
}

template<typename T, size_t arrayLength, typename Selector>
bool StringVariant::contains(char (&needle)[arrayLength], T length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return contains(needle, len, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::contains(char (&needle)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, arrayLength, 0, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::startsWith(char (&str)[arrayLength], size_t length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, length);
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return startsWith(buffer.get(), caseSensitive);
}

template<typename T, size_t arrayLength, typename Selector>
bool StringVariant::startsWith(char (&str)[arrayLength], T length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return startsWith(str, len, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::startsWith(char (&str)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return startsWith(str, arrayLength, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::endsWith(char (&str)[arrayLength], size_t length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   length = std::min(arrayLength, static_cast<size_t>(length));
   // here maybe not c str, we make a local buffer for it
   GuardValuePtrType buffer(static_cast<Pointer>(emalloc(length)), zapi::utils::std_php_memory_deleter);
   std::memcpy(buffer.get(), str, length);
   buffer.get()[length] = '\0';
   return endsWith(buffer.get(), caseSensitive);
}

template<typename T, size_t arrayLength, typename Selector>
bool StringVariant::endsWith(char (&str)[arrayLength], T length, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   size_t len;
   if (length < 0) {
      len = arrayLength;
   } else {
      len = static_cast<size_t>(length);
   }
   return endsWith(str, len, caseSensitive);
}

template<size_t arrayLength>
bool StringVariant::endsWith(char (&str)[arrayLength], bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return endsWith(str, arrayLength, caseSensitive);
}

} // ds
} // zapi

#endif // ZAPI_DS_STRING_VARIANT_H