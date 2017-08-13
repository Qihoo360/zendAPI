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
// Created by zzu_softboy on 2017/08/08.

#include "zapi/ds/StringVariant.h"
#include "zapi/ds/internal/VariantPrivate.h"

#include <cstring>
#include <stdexcept>

namespace zapi
{
namespace ds
{

using zapi::ds::internal::VariantPrivate;

constexpr size_t STR_VARIANT_OVERHEAD = ZEND_MM_OVERHEAD + _ZSTR_HEADER_SIZE;
#ifdef SMART_STR_PAGE
constexpr size_t STR_VARIAMT_PAGE_SIZE = SMART_STR_PAGE; // just use zend default now
#else
constexpr size_t STR_VARIAMT_PAGE_SIZE = 4096;
#endif

#ifdef SMART_STR_START_SIZE
constexpr size_t STR_VARIANT_START_SIZE SMART_STR_START_SIZE;
#else
constexpr size_t STR_VARIANT_START_SIZE (256 - STR_VARIANT_OVERHEAD - 1);
#endif

StringVariant::StringVariant()
{}

StringVariant::StringVariant(const Variant &other)
{
   // chech the type, if the type is not the string, we just try to convert
   // if the type is string we deploy copy on write idiom
   const zval *from = other.getZvalPtr();
   zval *self = getZvalPtr();
   if (other.getType() == Type::String) {
      ZVAL_COPY(self, from);
   } else {
      zval temp;
      // will increase 1 to gc refcount
      ZVAL_DUP(&temp, from);
      // will decrease 1 to gc refcount
      convert_to_string(&temp);
      ZVAL_COPY_VALUE(self, &temp);
   }
   // we just set the capacity equal to default allocator algorithm
   // ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(len))
   m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
}

StringVariant::StringVariant(StringVariant &&other) ZAPI_DECL_NOEXCEPT
   : Variant(std::move(other))
{}

StringVariant::StringVariant(const StringVariant &other, bool ref)
{
   if (ref) {
      m_implPtr->m_ref = other.m_implPtr;
   } else {
      const zval *from = other.getZvalPtr();
      zval *self = getZvalPtr();
      ZVAL_COPY(self, from);
      m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
   }
}

StringVariant::StringVariant(Variant &&other) ZAPI_DECL_NOEXCEPT
   : Variant(std::move(other))
{
   zval *self = getZvalPtr();
   if (getType() != Type::String) {
      // first we need convert to string type
      convert_to_string(self);
   }
   m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
}

StringVariant::StringVariant(const std::string &value)
   : StringVariant(value.c_str(), value.size())
{}

StringVariant::StringVariant(const char *value, size_t length)
{
   // we alloc memory here
   // we don't use default ZVAL_STRINGL to setup ourser zval
   zend_string *strPtr = nullptr;
   strAlloc(strPtr, length, false);
   ZVAL_NEW_STR(getZvalPtr(), strPtr);
   // we need copy memory ourself
   memcpy(ZSTR_VAL(strPtr), value, length);
   ZSTR_VAL(strPtr)[length] = '\0';
   ZSTR_LEN(strPtr) = length;
}

StringVariant::StringVariant(const char *value)
   : StringVariant(value, std::strlen(value))
{}

StringVariant &StringVariant::operator =(const StringVariant &other)
{
   if (this != &other) {
      zval *self = getZvalPtr();
      zval *from = const_cast<zval *>(other.getZvalPtr());
      if (Type::Null == getType()) {
         Z_STR_P(self) = nullptr;
         Z_TYPE_INFO_P(self) = IS_STRING_EX;
      } else {
         SEPARATE_ZVAL_NOREF(self);
      }
      // need update gc info
      Variant::operator =(from);
      m_implPtr->m_strCapacity = other.m_implPtr->m_strCapacity;
   }
   return *this;
}

StringVariant &StringVariant::operator =(StringVariant &&other) ZAPI_DECL_NOEXCEPT
{
   assert(this != &other);
   // here object been slice, but no problem
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

StringVariant &StringVariant::operator =(const Variant &other)
{
   zval *self = getZvalPtr();
   if (Type::Null == getType()) {
      Z_STR_P(self) = nullptr;
      Z_TYPE_INFO_P(self) = IS_STRING_EX;
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
   zval *from = const_cast<zval *>(other.getZvalPtr());
   // need set gc info
   if (other.getType() == Type::String) {
      // standard copy
      Variant::operator =(from);
   } else {
      zval temp;
      // will increase 1 to gc refcount
      ZVAL_DUP(&temp, from);
      // will decrease 1 to gc refcount
      convert_to_string(&temp);
      // we need free original zend_string memory
      zend_string_free(Z_STR_P(self));
      ZVAL_COPY_VALUE(self, &temp);
   }
   m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
   return *this;
}

StringVariant &StringVariant::operator =(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   m_implPtr = std::move(other.m_implPtr);
   zval *self = getZvalPtr();
   if (getType() != Type::String) {
      convert_to_string(self);
   }
   m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
   return *this;
}

StringVariant &StringVariant::operator =(char value)
{
   ValueType buffer[2] = {value, '\0'};
   return operator =(buffer);
}

StringVariant &StringVariant::operator =(const std::string &value)
{
   return operator =(value.c_str());
}

StringVariant &StringVariant::operator =(const char *value)
{
   zval *self = getZvalPtr();
   if (Type::Null == getType()) {
      Z_STR_P(self) = nullptr;
      Z_TYPE_INFO_P(self) = IS_STRING_EX;	
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
   zend_string *strPtr = getZendStringPtr();
   size_t length = std::strlen(value);
   strReAlloc(strPtr, length, 0);
   ConstPointer sourcePtr = value;
   Pointer destPtr = ZSTR_VAL(strPtr);
   std::memcpy(destPtr, sourcePtr, length);
   destPtr[length] = '\0';
   ZSTR_LEN(strPtr) = length;
   Z_STR_P(self) = strPtr;
   return *this;
}

bool StringVariant::toBool() const ZAPI_DECL_NOEXCEPT
{
   return getType() != Type::Null && 0 != Z_STRLEN_P(const_cast<zval *>(getZvalPtr()));
}

std::string StringVariant::toString() const ZAPI_DECL_NOEXCEPT
{
   zval *self = const_cast<zval *>(getZvalPtr());
   return std::string(Z_STRVAL_P(self), Z_STRLEN_P(self));
}

std::string StringVariant::toLowerCase() const
{
   std::string ret(cbegin(), cend());
   return zapi::utils::str_tolower(ret);
}

std::string StringVariant::toUpperCase() const
{
   std::string ret(cbegin(), cend());
   return zapi::utils::str_toupper(ret);
}

StringVariant::Reference StringVariant::at(SizeType pos)
{
   return const_cast<StringVariant::Reference>(const_cast<const StringVariant &>(*this).at(pos));
}

StringVariant::ConstReference StringVariant::at(SizeType pos) const
{
   if (pos >= getSize()) {
      throw std::out_of_range("string pos out of range");
   }
   char *str = getRawStrPtr();
   return str[pos];
}

StringVariant::Iterator StringVariant::begin() ZAPI_DECL_NOEXCEPT
{
   return getRawStrPtr();
}

StringVariant::ConstrIterator StringVariant::begin() const ZAPI_DECL_NOEXCEPT
{
   return getCStr();
}

StringVariant::ConstrIterator StringVariant::cbegin() const ZAPI_DECL_NOEXCEPT
{
   return getCStr();
}

StringVariant::ReverseIterator StringVariant::rbegin() ZAPI_DECL_NOEXCEPT
{
   return ReverseIterator(getRawStrPtr() + getLength());
}

StringVariant::ConstReverseIterator StringVariant::rbegin() const ZAPI_DECL_NOEXCEPT
{
   return ConstReverseIterator(getCStr() + getLength());
}

StringVariant::ConstReverseIterator StringVariant::crbegin() const ZAPI_DECL_NOEXCEPT
{
   return ConstReverseIterator(getCStr() + getLength());
}

StringVariant::Iterator StringVariant::end() ZAPI_DECL_NOEXCEPT
{
   return getRawStrPtr() + getLength();
}

StringVariant::ConstrIterator StringVariant::end() const ZAPI_DECL_NOEXCEPT
{
   return getCStr() + getLength();
}

StringVariant::ConstrIterator StringVariant::cend() const ZAPI_DECL_NOEXCEPT
{
   return getCStr() + getLength();
}

StringVariant::ReverseIterator StringVariant::rend() ZAPI_DECL_NOEXCEPT
{
   return ReverseIterator(getRawStrPtr());
}

StringVariant::ConstReverseIterator StringVariant::rend() const ZAPI_DECL_NOEXCEPT
{
   return ConstReverseIterator(getCStr());
}

StringVariant::ConstReverseIterator StringVariant::crend() const ZAPI_DECL_NOEXCEPT
{
   return ConstReverseIterator(getCStr());
}

zapi_long StringVariant::indexOf(const char *needle, zapi_long offset, 
                                 bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   Pointer haystack = getRawStrPtr();
   size_t haystackLength = getSize();
   size_t needleLength = std::strlen(needle);
   GuardValuePtrType haystackDup(nullptr, zapi::utils::std_php_memory_deleter);
   GuardValuePtrType needleDup(nullptr, zapi::utils::std_php_memory_deleter);
   ConstPointer found = nullptr;
   if (offset < 0) {
      offset += haystackLength;
   }
   if (offset < 0 || static_cast<size_t>(offset) > haystackLength) {
      // TODO need throw exception here or return -1 ?
      return -1;
   }
   if (0 == needleLength) {
      return -1;
   }
   if (!caseSensitive) {
      haystackDup.reset(static_cast<Pointer>(emalloc(haystackLength)));
      needleDup.reset(static_cast<Pointer>(emalloc(needleLength)));
      std::memcpy(haystackDup.get(), haystack, haystackLength);
      std::memcpy(needleDup.get(), needle, needleLength);
      haystack = haystackDup.get();
      needle = needleDup.get();
      zapi::utils::str_tolower(haystackDup.get(), haystackLength);
      zapi::utils::str_tolower(needleDup.get(), needleLength);
   }
   found = zend_memnstr(haystack + offset, needle, needleLength, 
                        haystack + haystackLength);
   if (nullptr != found) {
      return found - haystack;
   }
   return -1;
}

zapi_long StringVariant::indexOf(const StringVariant &needle, zapi_long offset, 
                                 bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return indexOf(needle.getCStr(), offset, caseSensitive);
}

zapi_long StringVariant::indexOf(const std::string &needle, zapi_long offset, 
                                 bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return indexOf(needle.c_str(), offset, caseSensitive);
}

zapi_long StringVariant::indexOf(const char needle, zapi_long offset, 
                                 bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   ValueType buffer[2] = {needle, '\0'};
   return indexOf(reinterpret_cast<Pointer>(buffer), offset, caseSensitive);
}

StringVariant StringVariant::makeReference() const
{
   return StringVariant(*this, true);
}

zapi_long StringVariant::lastIndexOf(const char *needle, zapi_long offset, 
                                     bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   Pointer haystack = getRawStrPtr();
   size_t haystackLength = getSize();
   size_t needleLength = std::strlen(needle);
   GuardValuePtrType haystackDup(nullptr, zapi::utils::std_php_memory_deleter);
   GuardValuePtrType needleDup(nullptr, zapi::utils::std_php_memory_deleter);
   Pointer p = nullptr;
   Pointer e = nullptr;
   ConstPointer found = nullptr;
   if (!caseSensitive) {
      haystackDup.reset(static_cast<Pointer>(emalloc(haystackLength)));
      needleDup.reset(static_cast<Pointer>(emalloc(needleLength)));
      std::memcpy(haystackDup.get(), haystack, haystackLength);
      std::memcpy(needleDup.get(), needle, needleLength);
      haystack = haystackDup.get();
      needle = needleDup.get();
      zapi::utils::str_tolower(haystackDup.get(), haystackLength);
      zapi::utils::str_tolower(needleDup.get(), needleLength);
   }
   if (offset >= 0) {
      if (static_cast<size_t>(offset) > haystackLength) {
         // here we do like php
         php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
         return -1;
      }
      p = haystack + static_cast<size_t>(offset);
      e = haystack + haystackLength;
   } else {
      if (offset < -INT_MAX || static_cast<size_t>(-offset) > haystackLength) {
         php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
         return -1;
      }
      p = haystack;
      // find the best end pos
      if (static_cast<size_t>(-offset) < needleLength) {
         e = haystack + haystackLength;
      } else {
         e = haystack + haystackLength + offset + needleLength;
      }
   }
   found = zend_memnrstr(p, needle, needleLength, e);
   if (nullptr != found) {
      return found - haystack;
   }
   return -1;
}

zapi_long StringVariant::lastIndexOf(const StringVariant &needle, zapi_long offset, 
                                 bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return lastIndexOf(needle.getCStr(), offset, caseSensitive);
}

zapi_long StringVariant::lastIndexOf(const std::string &needle, zapi_long offset, 
                                     bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return lastIndexOf(needle.c_str(), offset, caseSensitive);
}

zapi_long StringVariant::lastIndexOf(const char needle, zapi_long offset, 
                                     bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   ValueType buffer[2] = {needle, '\0'};
   return lastIndexOf(reinterpret_cast<Pointer>(buffer), offset, caseSensitive);
}

bool StringVariant::contains(const StringVariant &needle, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle.getCStr(), 0, caseSensitive);
}

bool StringVariant::contains(const char *needle, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, 0, caseSensitive);
}

bool StringVariant::contains(const std::string &needle, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, 0, caseSensitive);
}

bool StringVariant::contains(const char needle, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return -1 != indexOf(needle, 0, caseSensitive);
}

bool StringVariant::startsWith(const StringVariant &str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return startsWith(str.getCStr(), caseSensitive);
}

bool StringVariant::startsWith(const char *str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   Pointer selfStr = getRawStrPtr();
   Pointer otherStrPtr = const_cast<Pointer>(str);
   GuardValuePtrType selfStrLowerCase(nullptr, zapi::utils::std_php_memory_deleter);
   GuardValuePtrType otherStrLowerCase(nullptr, zapi::utils::std_php_memory_deleter);
   size_t selfStrLength = getSize();
   size_t otherStrLength = std::strlen(str);
   if (selfStrLength < otherStrLength) {
      return false;
   }
   if (!caseSensitive) {
      selfStrLowerCase.reset(static_cast<Pointer>(emalloc(selfStrLength)));
      otherStrLowerCase.reset(static_cast<Pointer>(emalloc(otherStrLength)));
      std::memcpy(selfStrLowerCase.get(), selfStr, selfStrLength);
      std::memcpy(otherStrLowerCase.get(), otherStrPtr, otherStrLength);
      selfStr = selfStrLowerCase.get();
      otherStrPtr = otherStrLowerCase.get();
      zapi::utils::str_tolower(selfStr);
      zapi::utils::str_tolower(otherStrPtr);
   }
   return 0 == std::memcmp(selfStr, otherStrPtr, otherStrLength);
}

bool StringVariant::startsWith(const std::string &str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return startsWith(str.c_str(), caseSensitive);
}

bool StringVariant::startsWith(char c, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   ValueType buffer[2] = {c, '\0'};
   return startsWith(reinterpret_cast<Pointer>(buffer), caseSensitive);
}

bool StringVariant::endsWith(const StringVariant &str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return endsWith(str.getCStr(), caseSensitive);
}

bool StringVariant::endsWith(const char *str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   Pointer selfStr = getRawStrPtr();
   Pointer otherStrPtr = const_cast<Pointer>(str);
   GuardValuePtrType selfStrLowerCase(nullptr, zapi::utils::std_php_memory_deleter);
   GuardValuePtrType otherStrLowerCase(nullptr, zapi::utils::std_php_memory_deleter);
   size_t selfStrLength = getSize();
   size_t otherStrLength = std::strlen(str);
   if (selfStrLength < otherStrLength) {
      return false;
   }
   if (!caseSensitive) {
      selfStrLowerCase.reset(static_cast<Pointer>(emalloc(selfStrLength)));
      otherStrLowerCase.reset(static_cast<Pointer>(emalloc(otherStrLength)));
      std::memcpy(selfStrLowerCase.get(), selfStr, selfStrLength);
      std::memcpy(otherStrLowerCase.get(), otherStrPtr, otherStrLength);
      selfStr = selfStrLowerCase.get();
      otherStrPtr = otherStrLowerCase.get();
      zapi::utils::str_tolower(selfStr);
      zapi::utils::str_tolower(otherStrPtr);
   }
   return 0 == std::memcmp(selfStr + selfStrLength - otherStrLength, otherStrPtr, otherStrLength);
}

bool StringVariant::endsWith(const std::string &str, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   return endsWith(str.c_str(), caseSensitive);
}

bool StringVariant::endsWith(char c, bool caseSensitive) const ZAPI_DECL_NOEXCEPT
{
   ValueType buffer[2] = {c, '\0'};
   return endsWith(reinterpret_cast<Pointer>(buffer), caseSensitive);
}

StringVariant &StringVariant::prepend(const char *str)
{
   zval *self = getZvalPtr();
   if (Type::Null == getType()) {
      Z_STR_P(self) = nullptr;
      Z_TYPE_INFO_P(self) = IS_STRING_EX;	
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
   zend_string *destStrPtr = getZendStringPtr();
   size_t length = std::strlen(str);
   size_t selfLength = getSize();
   size_t newLength = strAlloc(destStrPtr, length, 0);
   Pointer newRawStr = ZSTR_VAL(destStrPtr);
   // copy backward
   size_t iterator = selfLength;
   while (iterator--) {
      *(newRawStr + iterator + length) = *(newRawStr + iterator);
   }
   // copy prepend
   std::memcpy(newRawStr, str, length);
   // set self state
   ZSTR_VAL(destStrPtr)[newLength] = '\0';
   ZSTR_LEN(destStrPtr) = newLength;
   Z_STR_P(self) = destStrPtr;
   return *this;
}

StringVariant &StringVariant::prepend(const char c)
{
   ValueType buffer[2] = {c, '\0'};
   return prepend(reinterpret_cast<Pointer>(buffer));
}

StringVariant &StringVariant::prepend(const std::string &str)
{
   return prepend(str.c_str());
}

StringVariant &StringVariant::prepend(const StringVariant &str)
{
   return prepend(str.getCStr());
}

StringVariant &StringVariant::append(const char *str)
{
   zval *self = getZvalPtr();
   if (Type::Null == getType()) {
      Z_STR_P(self) = nullptr;
      Z_TYPE_INFO_P(self) = IS_STRING_EX;	
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
   zend_string *destStrPtr = getZendStringPtr();
   size_t length = std::strlen(str);
   size_t newLength = strAlloc(destStrPtr, length, 0);
   memcpy(ZSTR_VAL(destStrPtr) + getLength(), str, length);
   // set self state
   ZSTR_VAL(destStrPtr)[newLength] = '\0';
   ZSTR_LEN(destStrPtr) = newLength;
   Z_STR_P(self) = destStrPtr;
   return *this;
}

StringVariant &StringVariant::append(const char c)
{
   ValueType buffer[2] = {c, '\0'};
   return append(reinterpret_cast<Pointer>(buffer));
}

StringVariant &StringVariant::append(const std::string &str)
{
   return append(str.c_str());
}

StringVariant &StringVariant::append(const StringVariant &str)
{
   return append(str.getCStr());
}

StringVariant &StringVariant::clear()
{
   // here we release zend_string memory
   // and set capacity to zero
   if (getType() == Type::Null) {
      return *this;
   }
   zval *self = getZvalPtr();
   SEPARATE_ZVAL_NOREF(self);
   zend_string *strPtr = getZendStringPtr();
   zend_string_free(strPtr);
   Z_STR_P(self) = nullptr;
   ZVAL_NULL(self);
   m_implPtr->m_strCapacity = 0;
   return *this;
}

void StringVariant::resize(SizeType size)
{
   if (size == m_implPtr->m_strCapacity) {
      return;
   }
   zval *self = getZvalPtr();
   if (Type::Null == getType()) {
      Z_STR_P(self) = nullptr;
      Z_TYPE_INFO_P(self) = IS_STRING_EX;	
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
   // here we use std string alloc 
   zend_string *newStr = zend_string_alloc(size, 0);
   zend_string *oldStr = getZendStringPtr();
   if (oldStr) {
      // we need copy the org content
      size_t needCopyLength = std::min(size, getSize());
      std::memcpy(ZSTR_VAL(newStr), ZSTR_VAL(oldStr), needCopyLength);
      // release old resource
      zend_string_free(oldStr);
   } else {
      std::memset(ZSTR_VAL(newStr), '\0', size);
   }
   ZSTR_VAL(newStr)[size] = '\0';
   Z_STR_P(self) = newStr;
   m_implPtr->m_strCapacity = ZEND_MM_ALIGNED_SIZE(_ZSTR_STRUCT_SIZE(Z_STRLEN_P(self)));
}

void StringVariant::resize(SizeType size, char fillChar)
{
   size_t oldSize = getSize();
   resize(size);
   if (size > oldSize) {
      Pointer strPtr = getRawStrPtr();
      std::memset(strPtr + oldSize, fillChar, size - oldSize);
   }
}

std::string StringVariant::trimmed() const
{
   ConstPointer start = getRawStrPtr();
   ConstPointer end = start + getLength();
   while (start < end && std::isspace(*start)) {
      ++start;
   }
   if (start < end) {
      while (start < end && std::isspace(*(end - 1))) {
         end--;
      }
   }
   return std::string(start, end);
}

std::string StringVariant::simplified() const
{
   ConstPointer start = getRawStrPtr();
   ConstPointer end = start + getLength();
   GuardValuePtrType tempStr(static_cast<Pointer>(emalloc(getSize())), zapi::utils::std_php_memory_deleter);
   // trim two side space chars
   while (start < end && std::isspace(*start)) {
      ++start;
   }
   if (start < end) {
      while (start < end && std::isspace(*(end - 1))) {
         end--;
      }
   }
   // trim mid space chars
   Pointer dest = tempStr.get();
   bool seeSpace = false;
   while (start < end) {
      if (std::isspace(*start)) {
         ++start;
         if (!seeSpace) {
            *dest++ = ' ';
            seeSpace = true;
         }
      } else {
         *dest++ = *start++;
         seeSpace = false;
      }
   }
   return std::string(tempStr.get(), dest);
}

std::string StringVariant::left(size_t size) const
{
   size_t needCopyLength = std::min(size, getSize());
   Pointer strPtr = getRawStrPtr();
   return std::string(strPtr, strPtr + needCopyLength);
}

std::string StringVariant::right(size_t size) const
{
   size_t selfLength = getSize();
   size_t needCopyLength = std::min(size, selfLength);
   Pointer strPtr = getRawStrPtr();
   return std::string(strPtr + selfLength - needCopyLength, strPtr + selfLength);
}

std::string StringVariant::leftJustified(size_t size, char fill) const
{
   Pointer strPtr = getRawStrPtr();
   size_t selfLength = getLength();
   size_t needCopyLength = std::min(size, selfLength);
   std::string ret(strPtr, strPtr + needCopyLength);
   if (size > selfLength) {
      // need fill
      size_t fillLength = size - selfLength;
      while(fillLength--) {
         ret.push_back(fill);
      }
   }
   return ret;
}

std::string StringVariant::rightJustified(size_t size, char fill) const
{
   Pointer strPtr = getRawStrPtr();
   size_t selfLength = getLength();
   size_t needCopyLength = std::min(size, selfLength);
   std::string ret;
   ret.reserve(std::max(size, selfLength));
   if (size > selfLength) {
      // need fill
      size_t fillLength = size - selfLength;
      while(fillLength--) {
         ret.push_back(fill);
      }
   }
   ret.append(strPtr, needCopyLength);
   return ret;
}

std::string StringVariant::substring(size_t pos, size_t length) const
{
   size_t selfLength = getLength();
   if (pos > selfLength) {
      throw std::out_of_range("string pos out of range");
   }
   Pointer strPtr = getRawStrPtr();
   return std::string(strPtr + pos, strPtr + std::min(selfLength, pos + length));
}

std::string StringVariant::substring(size_t pos) const
{
   return substring(pos, getSize());
}

const char *StringVariant::getCStr() const ZAPI_DECL_NOEXCEPT
{
   return Z_STRVAL_P(const_cast<zval *>(getZvalPtr()));
}

char *StringVariant::getRawStrPtr() const ZAPI_DECL_NOEXCEPT
{
   return Z_STRVAL_P(const_cast<zval *>(getZvalPtr()));
}

StringVariant::SizeType StringVariant::getSize() const ZAPI_DECL_NOEXCEPT
{
   zval *self = const_cast<zval *>(getZvalPtr());
   return Type::Null == getType() || !Z_STR_P(self) ? 0 : Z_STRLEN_P(self);
}

bool StringVariant::isEmpty() const ZAPI_DECL_NOEXCEPT
{
   return 0 == getSize();
}

StringVariant::SizeType StringVariant::getLength() const ZAPI_DECL_NOEXCEPT
{
   return getSize();
}

StringVariant::SizeType StringVariant::getCapacity() const ZAPI_DECL_NOEXCEPT
{
   return m_implPtr->m_strCapacity;
}

StringVariant::~StringVariant() ZAPI_DECL_NOEXCEPT
{
   if (m_implPtr) {
      m_implPtr->m_strCapacity = 0;
   }
}

zend_string *StringVariant::getZendStringPtr()
{
   return Z_STR_P(getZvalPtr());
}

constexpr size_t StringVariant::calculateNewStrSize(size_t length) ZAPI_DECL_NOEXCEPT
{
   return ((length + STR_VARIANT_OVERHEAD + STR_VARIAMT_PAGE_SIZE) & ~(STR_VARIAMT_PAGE_SIZE - 1)) - STR_VARIANT_OVERHEAD - 1;
}

void StringVariant::strStdRealloc(zend_string *&str, size_t length)
{
   if (UNEXPECTED(!str)) {
      m_implPtr->m_strCapacity = length < STR_VARIANT_START_SIZE
            ? STR_VARIANT_START_SIZE
            : calculateNewStrSize(length);
      str = zend_string_alloc(m_implPtr->m_strCapacity, 0);
      ZSTR_LEN(str) = 0;
   } else {
      m_implPtr->m_strCapacity = calculateNewStrSize(length);
      str = static_cast<zend_string *>(erealloc2(str, _ZSTR_HEADER_SIZE + m_implPtr->m_strCapacity + 1, 
                                                 _ZSTR_HEADER_SIZE + ZSTR_LEN(str) + 1));
   }
}

void StringVariant::strPersistentRealloc(zend_string *&str, size_t length)
{
   if (UNEXPECTED(!str)) {
      m_implPtr->m_strCapacity = length < STR_VARIANT_START_SIZE
            ? STR_VARIANT_START_SIZE
            : calculateNewStrSize(length);
      str = zend_string_alloc(m_implPtr->m_strCapacity, 1);
      ZSTR_LEN(str) = 0;
   } else {
      m_implPtr->m_strCapacity = calculateNewStrSize(length);
      str = static_cast<zend_string *>(realloc(str, _ZSTR_HEADER_SIZE + m_implPtr->m_strCapacity + 1));
   }
}

size_t StringVariant::strAlloc(zend_string *&str, size_t length, bool persistent)
{
   if (UNEXPECTED(!str)) {
      goto do_smart_str_realloc;
   } else {
      length += ZSTR_LEN(str);
      if (UNEXPECTED(length >= m_implPtr->m_strCapacity)) {
do_smart_str_realloc:
         if (persistent) {
            strPersistentRealloc(str, length);
         } else {
            strStdRealloc(str, length);
         }
      }
   }
   return length;
}

size_t StringVariant::strReAlloc(zend_string *&str, size_t length, bool persistent)
{
   if (UNEXPECTED(!str)) {
      goto do_smart_str_realloc;
   } else {
      if (UNEXPECTED(length >= m_implPtr->m_strCapacity)) {
do_smart_str_realloc:
         if (persistent) {
            strPersistentRealloc(str, length);
         } else {
            strStdRealloc(str, length);
         }
      }
   }
   return length;
}

} // ds
} // zapi