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

#ifndef ZAPI_DS_INTERNAL_DOUBLE_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_DOUBLE_VARIANT_PRIVATE_H

#include "zapi/ds/Variant.h"
#include <cmath>

namespace zapi
{
namespace ds
{

// forward declare
class NumericVariant;

class ZAPI_DECL_EXPORT DoubleVariant final : public Variant
{
public:
   DoubleVariant();
   DoubleVariant(std::int8_t value);
   DoubleVariant(std::int16_t value);
   DoubleVariant(std::int32_t value);
   DoubleVariant(std::int64_t value);
   DoubleVariant(double value);
   DoubleVariant(const DoubleVariant &other);
   DoubleVariant(const Variant &other);
   DoubleVariant(Variant &&other);
   virtual bool toBool() const ZAPI_DECL_NOEXCEPT override;
   double toDouble() const ZAPI_DECL_NOEXCEPT;
   operator double() const;
   DoubleVariant &operator =(std::int8_t other);
   DoubleVariant &operator =(std::int16_t other);
   DoubleVariant &operator =(std::int32_t other);
   DoubleVariant &operator =(std::int64_t other);
   DoubleVariant &operator =(double other);
   DoubleVariant &operator =(const DoubleVariant &other);
   DoubleVariant &operator =(const NumericVariant &other);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator +=(T value) ZAPI_DECL_NOEXCEPT;
   DoubleVariant &operator +=(const DoubleVariant &value) ZAPI_DECL_NOEXCEPT;
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator -=(T value) ZAPI_DECL_NOEXCEPT;
   DoubleVariant &operator -=(const DoubleVariant &value) ZAPI_DECL_NOEXCEPT;
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator *=(T value) ZAPI_DECL_NOEXCEPT;
   DoubleVariant &operator *=(const DoubleVariant &value) ZAPI_DECL_NOEXCEPT;
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator /=(T value) ZAPI_DECL_NOEXCEPT;
   DoubleVariant &operator /=(const DoubleVariant &value) ZAPI_DECL_NOEXCEPT;
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator %=(T value) ZAPI_DECL_NOEXCEPT;
   DoubleVariant &operator %=(const DoubleVariant &value) ZAPI_DECL_NOEXCEPT;
   
   virtual ~DoubleVariant() ZAPI_DECL_NOEXCEPT;
};

// member template operators

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator +=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator -=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() - value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator *=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() * value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator /=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() / value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator %=(T value) ZAPI_DECL_NOEXCEPT
{
   ZVAL_DOUBLE(getZvalPtr(), std::fmod(toDouble(), static_cast<double>(value)));
   return *this;
}

// template compare operators
template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) == 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) != 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) < 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) <= 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) > 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return (static_cast<T>(lhs) - rhs.toDouble()) >= 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator ==(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) == 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator !=(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) != 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) < 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator <=(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) <= 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) > 0;
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT bool operator >=(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return (lhs.toDouble() - static_cast<T>(rhs)) >= 0;
}

ZAPI_DECL_EXPORT bool operator ==(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator !=(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator <=(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool operator >=(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;

ZAPI_DECL_EXPORT double operator +(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator -(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator *(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator /(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator %(const DoubleVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;

ZAPI_DECL_EXPORT double operator +(const DoubleVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator -(const DoubleVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator *(const DoubleVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator /(const DoubleVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator %(const DoubleVariant &lhs, const NumericVariant &rhs) ZAPI_DECL_NOEXCEPT;

ZAPI_DECL_EXPORT double operator +(const NumericVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator -(const NumericVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator *(const NumericVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator /(const NumericVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT double operator %(const NumericVariant &lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT;

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator +(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<double>(lhs) + rhs.toDouble();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator -(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<double>(lhs) - rhs.toDouble();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator *(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<double>(lhs) * rhs.toDouble();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator /(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return static_cast<double>(lhs) / rhs.toDouble();
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator %(T lhs, const DoubleVariant &rhs) ZAPI_DECL_NOEXCEPT
{
   return std::fmod(static_cast<double>(lhs), rhs.toDouble());
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator +(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toDouble() + static_cast<double>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator -(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toDouble() - static_cast<double>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator *(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toDouble() * static_cast<double>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator /(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return lhs.toDouble() / static_cast<double>(rhs);
}

template <typename T, 
          typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
ZAPI_DECL_EXPORT double operator %(const DoubleVariant &lhs, T rhs) ZAPI_DECL_NOEXCEPT
{
   return std::fmod(lhs.toDouble(), static_cast<double>(rhs));
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_DOUBLE_VARIANT_PRIVATE_H