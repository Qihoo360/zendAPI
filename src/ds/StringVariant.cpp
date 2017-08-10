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
   : StringVariant("")
{}

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

bool StringVariant::toBool() const ZAPI_DECL_NOEXCEPT
{
   return 0 != Z_STRLEN_P(const_cast<zval *>(getZvalPtr()));
}

std::string StringVariant::toString() const ZAPI_DECL_NOEXCEPT
{
   zval *self = const_cast<zval *>(getZvalPtr());
   return std::string(Z_STRVAL_P(self), Z_STRLEN_P(self));
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


StringVariant::~StringVariant() ZAPI_DECL_NOEXCEPT
{
   m_implPtr->m_strCapacity = 0;
}

zend_string *StringVariant::getZendStringPtr()
{
   return Z_STR_P(getZvalPtr());
}

constexpr size_t StringVariant::calculateNewStrSize(size_t length)
{
   return ((length + STR_VARIANT_OVERHEAD + STR_VARIAMT_PAGE_SIZE) & ~(STR_VARIAMT_PAGE_SIZE - 1)) - STR_VARIANT_OVERHEAD - 1;
}

} // ds
} // zapi