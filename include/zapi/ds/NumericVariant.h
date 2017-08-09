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

#ifndef ZAPI_DS_INTERNAL_NUMERIC_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_NUMERIC_VARIANT_PRIVATE_H

#include "zapi/ds/Variant.h"

namespace zapi
{
namespace ds
{

class ZAPI_DECL_EXPORT NumericVariant : public Variant
{
public:
   NumericVariant(std::int8_t value);
   NumericVariant(std::int16_t value);
   NumericVariant(std::int32_t value);
#if SIZEOF_ZEND_LONG == 8
   NumericVariant(std::int64_t value);
#endif
   NumericVariant(const NumericVariant &other);
   NumericVariant(const Variant &source);
   operator zapi_long () const;
   NumericVariant &operator++();
   NumericVariant operator++(int);
   NumericVariant &operator--();
   NumericVariant operator--(int);
   NumericVariant &operator+=(std::int8_t value);
   NumericVariant &operator+=(std::int16_t value);
   NumericVariant &operator+=(std::int32_t value);
   NumericVariant &operator+=(std::int64_t value);
   NumericVariant &operator+=(double value);
   
   NumericVariant &operator-=(std::int8_t value);
   NumericVariant &operator-=(std::int16_t value);
   NumericVariant &operator-=(std::int32_t value);
   NumericVariant &operator-=(std::int64_t value);
   NumericVariant &operator-=(double value);
   
   NumericVariant &operator*=(std::int8_t value);
   NumericVariant &operator*=(std::int16_t value);
   NumericVariant &operator*=(std::int32_t value);
   NumericVariant &operator*=(std::int64_t value);
   NumericVariant &operator*=(double value);
   
   NumericVariant &operator/=(std::int8_t value);
   NumericVariant &operator/=(std::int16_t value);
   NumericVariant &operator/=(std::int32_t value);
   NumericVariant &operator/=(std::int64_t value);
   NumericVariant &operator/=(double value);
   
   NumericVariant &operator%=(std::int8_t value);
   NumericVariant &operator%=(std::int16_t value);
   NumericVariant &operator%=(std::int32_t value);
   NumericVariant &operator%=(std::int64_t value);
   
   virtual bool toBool() const override;
   zapi_long toLong() const;
   virtual ~NumericVariant();
};

ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, const NumericVariant &rhs);

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() == static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() != static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() < static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() <= static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() > static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, T rhs)
{
   return lhs.toLong() >= static_cast<zapi_long>(rhs);
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) == rhs.toLong();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) != rhs.toLong();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) < rhs.toLong();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) <= rhs.toLong();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) > rhs.toLong();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(T lhs, const NumericVariant &rhs)
{
   return static_cast<zapi_long>(lhs) >= rhs.toLong();
}

ZAPI_DECL_EXPORT bool operator ==(double lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator !=(double lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator <(double lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator <=(double lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator >(double lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT bool operator >=(double lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, double rhs);
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, double rhs);
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, double rhs);
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, double rhs);
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, double rhs);
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, double rhs);

ZAPI_DECL_EXPORT zapi_long operator+(std::int8_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator+(std::int16_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator+(std::int32_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator+(std::int64_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT double operator+(double lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT zapi_long operator-(std::int8_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator-(std::int16_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator-(std::int32_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator-(std::int64_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT double operator-(double lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT zapi_long operator*(std::int8_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator*(std::int16_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator*(std::int32_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator*(std::int64_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT double operator*(double lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT zapi_long operator/(std::int8_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator/(std::int16_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator/(std::int32_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator/(std::int64_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT double operator/(double lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT zapi_long operator%(std::int8_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator%(std::int16_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator%(std::int32_t lhs, const NumericVariant &rhs);
ZAPI_DECL_EXPORT zapi_long operator%(std::int64_t lhs, const NumericVariant &rhs);

ZAPI_DECL_EXPORT zapi_long operator+(const NumericVariant &lhs, std::int8_t rhs);
ZAPI_DECL_EXPORT zapi_long operator+(const NumericVariant &lhs, std::int16_t rhs);
ZAPI_DECL_EXPORT zapi_long operator+(const NumericVariant &lhs, std::int32_t rhs);
ZAPI_DECL_EXPORT zapi_long operator+(const NumericVariant &lhs, std::int64_t rhs);
ZAPI_DECL_EXPORT double operator+(const NumericVariant &lhs, double rhs);

ZAPI_DECL_EXPORT zapi_long operator-(const NumericVariant &lhs, std::int8_t rhs);
ZAPI_DECL_EXPORT zapi_long operator-(const NumericVariant &lhs, std::int16_t rhs);
ZAPI_DECL_EXPORT zapi_long operator-(const NumericVariant &lhs, std::int32_t rhs);
ZAPI_DECL_EXPORT zapi_long operator-(const NumericVariant &lhs, std::int64_t rhs);
ZAPI_DECL_EXPORT double operator-(const NumericVariant &lhs, double rhs);

ZAPI_DECL_EXPORT zapi_long operator*(const NumericVariant &lhs, std::int8_t rhs);
ZAPI_DECL_EXPORT zapi_long operator*(const NumericVariant &lhs, std::int16_t rhs);
ZAPI_DECL_EXPORT zapi_long operator*(const NumericVariant &lhs, std::int32_t rhs);
ZAPI_DECL_EXPORT zapi_long operator*(const NumericVariant &lhs, std::int64_t rhs);
ZAPI_DECL_EXPORT double operator*(const NumericVariant &lhs, double rhs);

ZAPI_DECL_EXPORT zapi_long operator/(const NumericVariant &lhs, std::int8_t rhs);
ZAPI_DECL_EXPORT zapi_long operator/(const NumericVariant &lhs, std::int16_t rhs);
ZAPI_DECL_EXPORT zapi_long operator/(const NumericVariant &lhs, std::int32_t rhs);
ZAPI_DECL_EXPORT zapi_long operator/(const NumericVariant &lhs, std::int64_t rhs);
ZAPI_DECL_EXPORT double operator/(const NumericVariant &lhs, double rhs);

ZAPI_DECL_EXPORT zapi_long operator%(const NumericVariant &lhs, std::int8_t rhs);
ZAPI_DECL_EXPORT zapi_long operator%(const NumericVariant &lhs, std::int16_t rhs);
ZAPI_DECL_EXPORT zapi_long operator%(const NumericVariant &lhs, std::int32_t rhs);
ZAPI_DECL_EXPORT zapi_long operator%(const NumericVariant &lhs, std::int64_t rhs);

ZAPI_DECL_EXPORT zapi_long operator+(const NumericVariant &lhs, NumericVariant rhs);
ZAPI_DECL_EXPORT zapi_long operator-(const NumericVariant &lhs, NumericVariant rhs);
ZAPI_DECL_EXPORT zapi_long operator*(const NumericVariant &lhs, NumericVariant rhs);
ZAPI_DECL_EXPORT zapi_long operator/(const NumericVariant &lhs, NumericVariant rhs);
ZAPI_DECL_EXPORT zapi_long operator%(const NumericVariant &lhs, NumericVariant rhs);

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_NUMERIC_VARIANT_PRIVATE_H