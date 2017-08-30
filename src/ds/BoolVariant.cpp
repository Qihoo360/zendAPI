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

#include "zapi/ds/BoolVariant.h"
#include "zapi/ds/ArrayItemProxy.h"

namespace zapi
{
namespace ds
{

using zapi::ds::internal::VariantPrivate;

BoolVariant::BoolVariant()
   : Variant(false)
{}

BoolVariant::BoolVariant(bool value)
   : Variant(value)
{}

BoolVariant::BoolVariant(zval &other, bool isRef)
   : BoolVariant(&other, isRef)
{}

BoolVariant::BoolVariant(zval &&other, bool isRef)
   : BoolVariant(&other, isRef)
{}

BoolVariant::BoolVariant(zval *other, bool isRef)
{
   zval *self = getUnDerefZvalPtr();
   if (nullptr != other) {
      if (isRef && (Z_TYPE_P(other) == IS_TRUE || Z_TYPE_P(other) == IS_FALSE)) {
         ZVAL_MAKE_REF(other);
         zend_reference *ref = Z_REF_P(other);
         ++GC_REFCOUNT(ref);
         ZVAL_REF(self, ref);
      } else {
         ZVAL_DUP(self, other);
         convert_to_boolean(self);
      }
   } else {
      ZVAL_BOOL(self, false);
   }
}

BoolVariant::BoolVariant(const BoolVariant &other)
   : Variant(other)
{}

BoolVariant::BoolVariant(BoolVariant &other, bool isRef)
{
   zval *self = getUnDerefZvalPtr();
   if (!isRef) {
      ZVAL_BOOL(self, Z_TYPE_INFO_P(other.getZvalPtr()) == IS_TRUE);
   } else {
      zval *source = other.getUnDerefZvalPtr();
      ZVAL_MAKE_REF(source);
      ZVAL_COPY(self, source);
   }
}

BoolVariant::BoolVariant(BoolVariant &&other) ZAPI_DECL_NOEXCEPT
   : Variant(std::move(other))
{}

BoolVariant::BoolVariant(const Variant &other)
{
   zval *self = getZvalPtr();
   zval *from = const_cast<zval *>(other.getZvalPtr());
   if (other.getType() == Type::True || other.getType() == Type::False) {
      ZVAL_BOOL(self, Z_TYPE_INFO_P(from) == IS_TRUE);
   } else {
      zval temp;
      ZVAL_DUP(&temp, from);
      convert_to_boolean(&temp);
      ZVAL_COPY_VALUE(self, &temp);
   }
}

BoolVariant::BoolVariant(Variant &&other)
   : Variant(std::move(other))
{
   if (getType() != Type::Boolean) {
      convert_to_boolean(getZvalPtr());
   }
}

BoolVariant &BoolVariant::operator=(const BoolVariant &other)
{
   if (this != &other) {
      ZVAL_BOOL(getZvalPtr(), Z_TYPE_INFO_P(other.getZvalPtr()) == IS_TRUE);
   }
   return *this;
}

BoolVariant &BoolVariant::operator=(const Variant &other)
{
   ZVAL_BOOL(getZvalPtr(), other.toBool());
   return *this;
}

BoolVariant &BoolVariant::operator=(ArrayItemProxy &&other)
{
   return operator =(other.toBoolVariant());
}

BoolVariant &BoolVariant::operator=(bool value)
{
   ZVAL_BOOL(getZvalPtr(), value);
   return *this;
}

BoolVariant::~BoolVariant()
{}

BoolVariant::operator bool() const
{
   return toBool();
}

bool BoolVariant::toBool() const ZAPI_DECL_NOEXCEPT
{
   return Z_TYPE_INFO_P(getZvalPtr()) == IS_TRUE ? true : false;
}

bool operator ==(const BoolVariant &lhs, const BoolVariant &rhs)
{
   return lhs.toBool() == rhs.toBool();
}

bool operator !=(const BoolVariant &lhs, const BoolVariant &rhs)
{
   return lhs.toBool() != rhs.toBool();
}

} // ds
} // zapi