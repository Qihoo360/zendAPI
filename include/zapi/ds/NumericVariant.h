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

#ifndef ZAPI_DS_NUMERIC_VARIANT_H
#define ZAPI_DS_NUMERIC_VARIANT_H

#include "zapi/ds/Variant.h"
#include <type_traits>

namespace zapi
{
namespace ds
{

class DoubleVariant;
class ArrayItemProxy;

class ZAPI_DECL_EXPORT NumericVariant final : public Variant
{
public:
   NumericVariant();
   NumericVariant(std::int8_t value);
   NumericVariant(std::int16_t value);
   NumericVariant(std::int32_t value);
#if SIZEOF_ZEND_LONG == 8
   NumericVariant(std::int64_t value);
#endif
   NumericVariant(zval &other, bool isRef = false);
   NumericVariant(zval &&other, bool isRef = false);
   NumericVariant(zval *other, bool isRef = false);
   NumericVariant(const NumericVariant &other);
   NumericVariant(NumericVariant &other, bool isRef);
   NumericVariant(NumericVariant &&other) ZAPI_DECL_NOEXCEPT;
   NumericVariant(const Variant &other);
   NumericVariant(Variant &&other);
   NumericVariant &operator =(std::int8_t other);
   NumericVariant &operator =(std::int16_t other);
   NumericVariant &operator =(std::int32_t other);
   NumericVariant &operator =(std::int64_t other);
   NumericVariant &operator =(double other);
   NumericVariant &operator =(const NumericVariant &other);
   NumericVariant &operator =(const Variant &other);
   NumericVariant &operator =(const DoubleVariant &other);
   NumericVariant &operator =(ArrayItemProxy &&other);
   template <typename T,
             typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   operator T () const;
   NumericVariant &operator++();
   NumericVariant operator++(int);
   NumericVariant &operator--();
   NumericVariant operator--(int);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
   NumericVariant &operator +=(T value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator +=(double value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator +=(const NumericVariant &value) ZAPI_DECL_NOEXCEPT;
   
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
   NumericVariant &operator -=(T value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator -=(double value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator -=(const NumericVariant &value) ZAPI_DECL_NOEXCEPT;
   
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
   NumericVariant &operator *=(T value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator *=(double value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator *=(const NumericVariant &value) ZAPI_DECL_NOEXCEPT;
   
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
   NumericVariant &operator /=(T value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator /=(double value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator /=(const NumericVariant &value) ZAPI_DECL_NOEXCEPT;
   
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
   NumericVariant &operator %=(T value) ZAPI_DECL_NOEXCEPT;
   NumericVariant &operator %=(const NumericVariant &value) ZAPI_DECL_NOEXCEPT;
   
   virtual bool toBool() const ZAPI_DECL_NOEXCEPT override;
   zapi_long toLong() const ZAPI_DECL_NOEXCEPT;
   virtual ~NumericVariant() ZAPI_DECL_NOEXCEPT;
};

template <typename T,
          typename Selector>
NumericVariant::operator T () const
{
   return zval_get_long(const_cast<zval *>(getZvalPtr()));
}

template <typename T, typename Selector>
NumericVariant &NumericVariant::operator +=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_LONG(getZvalPtr(), toLong() + static_cast<zapi_long>(value));
   return *this;
}

template <typename T, typename Selector>
NumericVariant &NumericVariant::operator -=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_LONG(getZvalPtr(), toLong() - static_cast<zapi_long>(value));
   return *this;
}

template <typename T, typename Selector>
NumericVariant &NumericVariant::operator *=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_LONG(getZvalPtr(), toLong() * static_cast<zapi_long>(value));
   return *this;
}

template <typename T, typename Selector>
NumericVariant &NumericVariant::operator /=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_LONG(getZvalPtr(), toLong() / static_cast<zapi_long>(value));
   return *this;
}

template <typename T, typename Selector>
NumericVariant &NumericVariant::operator %=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_LONG(getZvalPtr(), toLong() % static_cast<zapi_long>(value));
   return *this;
}

ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() == static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() != static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() < static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() <= static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() > static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() >= static_cast<zapi_long>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) == rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) != rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) < rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) <= rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) > rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) >= rhs.toLong();
}

ZAPI_DECL_EXPORT bool operator ==(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator !=(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <=(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >=(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

ZAPI_DECL_EXPORT bool operator ==(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator !=(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <=(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >=(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator +(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) + rhs.toLong();
}

ZAPI_DECL_EXPORT double operator +(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator -(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) - rhs.toLong();
}

ZAPI_DECL_EXPORT double operator -(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator *(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) * rhs.toLong();
}

ZAPI_DECL_EXPORT double operator *(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator /(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) / rhs.toLong();
}

ZAPI_DECL_EXPORT double operator/(double lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator %(T lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<zapi_long>(lhs) % rhs.toLong();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator +(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() + static_cast<zapi_long>(rhs);
}

ZAPI_DECL_EXPORT double operator +(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator -(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() - static_cast<zapi_long>(rhs);
}

ZAPI_DECL_EXPORT double operator -(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator *(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() * static_cast<zapi_long>(rhs);
}

ZAPI_DECL_EXPORT double operator *(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator /(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() / static_cast<zapi_long>(rhs);
}

ZAPI_DECL_EXPORT double operator /(const NumericVariant &lhs, double rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
ZAPI_DECL_EXPORT zapi_long operator %(const NumericVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toLong() % static_cast<zapi_long>(rhs);
}

ZAPI_DECL_EXPORT zapi_long operator +(const NumericVariant &lhs, NumericVariant rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT zapi_long operator -(const NumericVariant &lhs, NumericVariant rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT zapi_long operator *(const NumericVariant &lhs, NumericVariant rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT zapi_long operator /(const NumericVariant &lhs, NumericVariant rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT zapi_long operator %(const NumericVariant &lhs, NumericVariant rhs) ZAPI_DECL_NOEXCEPT;

} // ds
} // zapi

#endif // ZAPI_DS_NUMERIC_VARIANT_H
