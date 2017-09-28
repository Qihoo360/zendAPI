// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// @copyright 2013, 2014 Copernica BV
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by softboy on 2017/08/17.

#ifndef ZAPI_DS_INTERNAL_ARRAY_ITEM_PROXY_PRIVATE_H
#define ZAPI_DS_INTERNAL_ARRAY_ITEM_PROXY_PRIVATE_H

#include "zapi/Global.h"

namespace zapi
{
namespace ds
{

// forward declare
class ArrayItemProxy;

namespace internal
{

using KeyType = zapi::ds::ArrayItemProxy::KeyType;
class ArrayItemProxyPrivate
{
public:
   ArrayItemProxyPrivate(zval *array, const KeyType &requestKey, 
                         ArrayItemProxy *apiPtr, ArrayItemProxy *parent)
      : m_requestKey(requestKey),
        m_array(array),
        m_parent(parent),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zval *array, const std::string &key, 
                         ArrayItemProxy *apiPtr, ArrayItemProxy *parent)
      : m_requestKey(-1, std::make_shared<std::string>(key)), // -1 is very big ulong
        m_array(array),
        m_parent(parent),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zval *array, zapi_ulong index, 
                         ArrayItemProxy *apiPtr, ArrayItemProxy *parent)
      : m_requestKey(index, nullptr),
        m_array(array),
        m_parent(parent),
        m_apiPtr(apiPtr)
   {}
   
   ~ArrayItemProxyPrivate()
   {
      // here we check some things
      if (m_parent) {
         bool stop = false;
         m_apiPtr->checkExistRecursive(stop, m_array, m_apiPtr);
      } else if (m_needCheckRequestItem) {
         m_apiPtr->retrieveZvalPtr();
      }
   }
   ZAPI_DECLARE_PUBLIC(ArrayItemProxy)
   ArrayItemProxy::KeyType m_requestKey;
   zval *m_array;
   bool m_needCheckRequestItem = true;
   ArrayItemProxy *m_parent = nullptr;
   ArrayItemProxy *m_apiPtr;
};

} // internal
} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_ARRAY_ITEM_PROXY_PRIVATE_H