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
// Created by softboy on 2017/08/09.

#include "zapi/ds/DoubleVariant.h"
#include "zapi/ds/NumericVariant.h"

namespace zapi
{
namespace ds
{

DoubleVariant::DoubleVariant()
   : Variant(0.0)
{}

DoubleVariant::DoubleVariant(std::int8_t value)
   : Variant(value)
{}

DoubleVariant::DoubleVariant(std::int16_t value)
   : Variant(value)
{}

DoubleVariant::DoubleVariant(std::int32_t value)
   : Variant(value)
{}

DoubleVariant::DoubleVariant(std::int64_t value)
   : Variant(value)
{}

DoubleVariant::DoubleVariant(double value)
   : Variant(value)
{}


DoubleVariant::DoubleVariant(const DoubleVariant &other)
   : Variant(other)
{}

DoubleVariant::DoubleVariant(const Variant &source)
{
   Type sourceType = source.getType();
   zval *sourceZvalPtr = const_cast<zval *>(source.getZvalPtr());
   zval *self = getZvalPtr();
   if (sourceType == Type::Double) {
      ZVAL_DOUBLE(self, zval_get_double(sourceZvalPtr));
   } else {
      zval temp;
      ZVAL_DUP(&temp, sourceZvalPtr);
      convert_to_double(&temp);
      ZVAL_DOUBLE(self, zval_get_double(sourceZvalPtr));
   }
}

bool DoubleVariant::toBool() const
{
   return toDouble();
}

DoubleVariant::operator double() const
{
   return toDouble();
}

double DoubleVariant::toDouble() const
{
   return zval_get_double(const_cast<zval *>(getZvalPtr()));
}

DoubleVariant &DoubleVariant::operator =(std::int8_t other)
{
   ZVAL_DOUBLE(getZvalPtr(), static_cast<double>(other));
   return *this;
}

DoubleVariant &DoubleVariant::operator =(std::int16_t other)
{
   ZVAL_DOUBLE(getZvalPtr(), static_cast<double>(other));
   return *this;
}

DoubleVariant &DoubleVariant::operator =(std::int32_t other)
{
   ZVAL_DOUBLE(getZvalPtr(), static_cast<double>(other));
   return *this;
}

DoubleVariant &DoubleVariant::operator =(std::int64_t other)
{
   ZVAL_DOUBLE(getZvalPtr(), static_cast<double>(other));
   return *this;
}

DoubleVariant &DoubleVariant::operator =(double other)
{
   ZVAL_DOUBLE(getZvalPtr(), other);
   return *this;
}

DoubleVariant &DoubleVariant::operator =(const DoubleVariant &other)
{
   ZVAL_DOUBLE(getZvalPtr(), other.toDouble());
   return *this;
}

DoubleVariant &DoubleVariant::operator =(const NumericVariant &other)
{
   ZVAL_DOUBLE(getZvalPtr(), static_cast<double>(other.toLong()));
   return *this;
}

DoubleVariant::~DoubleVariant()
{}

DoubleVariant &DoubleVariant::operator +=(const DoubleVariant &value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value.toDouble());
   return *this;
}

DoubleVariant &DoubleVariant::operator -=(const DoubleVariant &value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value.toDouble());
   return *this;
}

DoubleVariant &DoubleVariant::operator *=(const DoubleVariant &value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value.toDouble());
   return *this;
}

DoubleVariant &DoubleVariant::operator /=(const DoubleVariant &value)
{
   ZVAL_DOUBLE(getZvalPtr(), toDouble() + value.toDouble());
   return *this;
}

DoubleVariant &DoubleVariant::operator %=(const DoubleVariant &value)
{
   ZVAL_DOUBLE(getZvalPtr(), std::fmod(toDouble(), value.toDouble()));
   return *this;
}

bool operator ==(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) == 0;
}

bool operator !=(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) != 0;
}

bool operator <(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) < 0;
}

bool operator <=(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) <= 0;
}

bool operator >(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) > 0;
}

bool operator >=(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return (lhs.toDouble() - rhs.toDouble()) >= 0;
}

double operator +(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toDouble() + rhs.toDouble();
}

double operator -(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toDouble() - rhs.toDouble();
}

double operator *(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toDouble() * rhs.toDouble();
}

double operator /(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toDouble() / rhs.toDouble();
}

double operator %(const DoubleVariant &lhs, const DoubleVariant &rhs)
{
   return std::fmod(lhs.toDouble(), rhs.toDouble());
}

double operator +(const DoubleVariant &lhs, const NumericVariant &rhs)
{
   return lhs.toDouble() + rhs.toLong();
}

double operator -(const DoubleVariant &lhs, const NumericVariant &rhs)
{
   return lhs.toDouble() - rhs.toLong();
}

double operator *(const DoubleVariant &lhs, const NumericVariant &rhs)
{
   return lhs.toDouble() * rhs.toLong();
}

double operator /(const DoubleVariant &lhs, const NumericVariant &rhs)
{
   return lhs.toDouble() / rhs.toLong();
}

double operator %(const DoubleVariant &lhs, const NumericVariant &rhs)
{
   return std::fmod(lhs.toDouble(), static_cast<double>(rhs.toLong()));
}

double operator +(const NumericVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toLong() + rhs.toDouble();
}

double operator -(const NumericVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toLong() - rhs.toDouble();
}

double operator *(const NumericVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toLong() * rhs.toDouble();
}

double operator /(const NumericVariant &lhs, const DoubleVariant &rhs)
{
   return lhs.toLong() / rhs.toDouble();
}

double operator %(const NumericVariant &lhs, const DoubleVariant &rhs)
{
   return std::fmod(static_cast<double>(lhs.toLong()), rhs.toDouble());
}

} // ds
} // zapi

