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
#include <iostream>

namespace zapi
{
namespace ds
{

namespace internal
{
using zapi::ds::ArrayItemProxy;
using zapi::ds::ArrayVariant;
using KeyType = zapi::ds::ArrayItemProxy::KeyType;
class ArrayItemProxyPrivate
{
public:
   ArrayItemProxyPrivate(zval *arrayZval, const KeyType &requestKey, ArrayItemProxy *apiPtr)
      : m_requestKey(requestKey),
        m_arrayZval(arrayZval),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zval *arrayZval, const std::string &key, ArrayItemProxy *apiPtr)
      : m_requestKey(-1, key), // -1 is very big ulong
        m_arrayZval(arrayZval),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zval *arrayZval, zapi_ulong index, ArrayItemProxy *apiPtr)
      : m_requestKey(index, std::string()),
        m_arrayZval(arrayZval),
        m_apiPtr(apiPtr)
   {}
   
   ~ArrayItemProxyPrivate()
   {
      // here we check some things
      if (m_needCheckRequestItem) {
         m_apiPtr->checkItemExist();
      }
   }
   ZAPI_DECLARE_PUBLIC(ArrayItemProxy)
   ArrayItemProxy::KeyType m_requestKey;
   zval *m_arrayZval;
   bool m_needCheckRequestItem = true;
   ArrayItemProxy *m_parent = nullptr;
   ArrayItemProxy *m_apiPtr;
};

} // internal


ArrayItemProxy::ArrayItemProxy(zval *arrayZval, const KeyType &requestKey)
   : m_implPtr(new ArrayItemProxyPrivate(arrayZval, requestKey, this))
{}

ArrayItemProxy::ArrayItemProxy(zval *arrayZval, const std::string &key)
   : m_implPtr(new ArrayItemProxyPrivate(arrayZval, key, this))
{}

ArrayItemProxy::ArrayItemProxy(zval *arrayZval, zapi_ulong index)
   : m_implPtr(new ArrayItemProxyPrivate(arrayZval, index, this))
{}

ArrayItemProxy ArrayItemProxy::operator [](zapi_long index)
{
   // we check parent item exist here
}

ArrayItemProxy ArrayItemProxy::operator [](const std::string &key)
{
   // we check parent item exist here
}

void ArrayItemProxy::checkItemExist()
{
   
}

zval *ArrayItemProxy::recursiveEnsureArrayExist()
{
   
}

} // ds
} // zapi