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

BoolVariant::BoolVariant(const BoolVariant &other)
   : Variant(other)
{
}

BoolVariant &BoolVariant::operator=(const BoolVariant &other)
{
   Variant::operator=(other);
   return *this;
}

BoolVariant &BoolVariant::operator=(const Variant &other)
{
   ZVAL_BOOL(getZvalPtr(), other.toBool());
   return *this;
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