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

#ifndef ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H

#include "zapi/ds/Variant.h"
#include <utility>
#include <string>

namespace zapi
{
namespace ds
{

class ZAPI_DECL_EXPORT ArrayVariant : public Variant
{
public:
   using IndexType = uint32_t;
   using SizeType = uint32_t;
   using KeyType = std::pair<zapi_ulong, std::string>;
   using DifferenceType = zapi_ptrdiff;
   using ValueType = Variant;
public:
   ArrayVariant();
   ArrayVariant(const ArrayVariant &other);
   ArrayVariant(ArrayVariant &&other) ZAPI_DECL_NOEXCEPT;
   ArrayVariant(const Variant &other);
   ArrayVariant(Variant &&other) ZAPI_DECL_NOEXCEPT;
   // info access
   bool isEmpty() const ZAPI_DECL_NOEXCEPT;
   bool isNull() const ZAPI_DECL_NOEXCEPT;
   SizeType getSize() const ZAPI_DECL_NOEXCEPT;
   SizeType count() const ZAPI_DECL_NOEXCEPT;
   ~ArrayVariant();
protected:
   _zend_array *getZendArrayPtr() const ZAPI_DECL_NOEXCEPT;
   _zend_array &getZendArray() const ZAPI_DECL_NOEXCEPT;
};

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H