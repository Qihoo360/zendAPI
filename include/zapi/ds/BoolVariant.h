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

#ifndef ZAPI_DS_INTERNAL_BOOL_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_BOOL_VARIANT_PRIVATE_H

#include "zapi/ds/Variant.h"

namespace zapi
{
namespace ds
{

class ZAPI_DECL_EXPORT BoolVariant : public Variant
{
public:
   BoolVariant();
   BoolVariant(const BoolVariant &other);
   BoolVariant(bool value);
   operator bool () const override;
   BoolVariant &operator=(const BoolVariant &other);
   BoolVariant &operator=(const Variant &other);
   BoolVariant &operator=(bool value);
   virtual ~BoolVariant();
};

ZAPI_DECL_EXPORT bool operator ==(const BoolVariant &lhs, const BoolVariant &rhs);
ZAPI_DECL_EXPORT bool operator !=(const BoolVariant &lhs, const BoolVariant &rhs);
} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_BOOL_VARIANT_PRIVATE_H