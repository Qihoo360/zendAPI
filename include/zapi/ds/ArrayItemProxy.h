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
// Created by softboy on 2017/08/15.

#include "zapi/Global.h"

namespace zapi
{
namespace ds
{

// forward declare with namespace
namespace internal
{
class ArrayItemProxyPrivate;
} // internal

// forward declare
class Variant;
class NumericVarint;
class DoubleVariant;
class StringVariant;
class BoolVariant;
class ArrayVariant;

using internal::ArrayItemProxyPrivate;
using zapi::lang::Type;

class ZAPI_DECL_EXPORT ArrayItemProxy final
{
public:
   using KeyType = std::pair<zapi_ulong, std::string>;
public:
   ArrayItemProxy(zval *arrayZval, const KeyType &requestKey);
   ArrayItemProxy(zval *arrayZval, const std::string &key);
   ArrayItemProxy(zval *arrayZval, zapi_ulong index);
   // operators 
   ArrayItemProxy &operator =(const Variant &value);
   ArrayItemProxy &operator =(const NumericVarint &value);
   ArrayItemProxy &operator =(const DoubleVariant &value);
   ArrayItemProxy &operator =(const StringVariant &value);
   ArrayItemProxy &operator =(const BoolVariant &value);
   ArrayItemProxy &operator =(const ArrayVariant &value);
   // cast operators
   operator Variant();
   operator NumericVarint();
   operator DoubleVariant();
   operator StringVariant();
   operator BoolVariant();
   operator ArrayVariant();
   // nest assign
   ArrayItemProxy operator [](zapi_long index);
   ArrayItemProxy operator [](const std::string &key);
   bool canConvert(Type type) const ZAPI_DECL_NOEXCEPT;
protected:
   void checkItemExist();
   zval *recursiveEnsureArrayExist();
protected:
   ZAPI_DECLARE_PRIVATE(ArrayItemProxy)
   std::shared_ptr<ArrayItemProxyPrivate> m_implPtr;
};

} // ds
} // zapi