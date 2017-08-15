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
// Created by softboy on 2017/08/14.

#include "zapi/ds/ArrayVariant.h"
#include "zapi/ds/ArrayItemProxy.h"

namespace zapi
{
namespace ds
{

ArrayVariant::ArrayVariant()
{
   // constructor null array
   zval *self = getZvalPtr();
   Z_ARRVAL_P(self) = nullptr;
   Z_TYPE_INFO_P(self) = IS_ARRAY_EX;
}

ArrayVariant::ArrayVariant(const Variant &other)
{
   
}

ArrayVariant::ArrayVariant(Variant &&other) ZAPI_DECL_NOEXCEPT
{
   
}

ArrayItemProxy ArrayVariant::operator [](zapi_ulong index)
{
   return ArrayItemProxy(getZvalPtr(), index);
}

ArrayItemProxy ArrayVariant::operator [](const std::string &key)
{
   return ArrayItemProxy(getZvalPtr(), key);
}

Variant ArrayVariant::operator [](zapi_ulong index) const
{
   
}

Variant ArrayVariant::operator [](const std::string &key) const
{
   
}

bool ArrayVariant::isEmpty() const ZAPI_DECL_NOEXCEPT
{
   return 0 == getSize();
}

bool ArrayVariant::isNull() const ZAPI_DECL_NOEXCEPT
{
   return nullptr == getZendArrayPtr();
}

ArrayVariant::SizeType ArrayVariant::getSize() const ZAPI_DECL_NOEXCEPT
{
   return isNull() ? 0 : zend_hash_num_elements(getZendArrayPtr());
}
ArrayVariant::SizeType ArrayVariant::count() const ZAPI_DECL_NOEXCEPT
{
   return getSize();
}

ArrayVariant::~ArrayVariant()
{}

_zend_array *ArrayVariant::getZendArrayPtr() const ZAPI_DECL_NOEXCEPT
{
   return Z_ARR(getZval());
}

_zend_array &ArrayVariant::getZendArray() const ZAPI_DECL_NOEXCEPT
{
   return *Z_ARR(getZval());
}

void ArrayVariant::deployCopyOnWrite()
{
   zval *self = getZvalPtr();
   if (nullptr == Z_ARRVAL_P(self)) {
      // null array alloc memory
      Z_ARR_P(self) =  static_cast<zend_array *>(emalloc(sizeof(zend_array)));
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
}

} // ds
} // zapi