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

class ZAPI_DECL_EXPORT DoubleVariant : public Variant
{
public:
   DoubleVariant(std::int8_t value);
   DoubleVariant(std::int16_t value);
   DoubleVariant(std::int32_t value);
   DoubleVariant(std::int64_t value);
   DoubleVariant(const DoubleVariant &other);
   DoubleVariant(const Variant &source);
   virtual bool toBool() const override;
   double toDouble() const;
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator +=(T value);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator -=(T value);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator *=(T value);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator /=(T value);
   template <typename T, typename Selector = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   DoubleVariant &operator %=(T value);
   
   virtual ~DoubleVariant();
};

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator +=(T value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator -=(T value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() - value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator *=(T value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() * value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator /=(T value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() / value);
   return *this;
}

template <typename T, typename Selector>
DoubleVariant &DoubleVariant::operator %=(T value)
{
   ZVAL_DOUBLE(getZvalPtr(), std::fmod(toDouble(), value));
   return *this;
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_DOUBLE_VARIANT_PRIVATE_H