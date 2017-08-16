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
#include "zapi/ds/NumericVariant.h"
#include "zapi/ds/DoubleVariant.h"
#include "zapi/ds/StringVariant.h"
#include "zapi/ds/BoolVariant.h"
#include <iostream>
#include <string>

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
   ArrayItemProxyPrivate(zend_array *array, const KeyType &requestKey, ArrayItemProxy *apiPtr)
      : m_requestKey(requestKey),
        m_array(m_array),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zend_array *array, const std::string &key, ArrayItemProxy *apiPtr)
      : m_requestKey(-1, std::make_shared<std::string>(key)), // -1 is very big ulong
        m_array(array),
        m_apiPtr(apiPtr)
   {}
   
   ArrayItemProxyPrivate(zend_array *array, zapi_ulong index, ArrayItemProxy *apiPtr)
      : m_requestKey(index, nullptr),
        m_array(array),
        m_apiPtr(apiPtr)
   {}
   
   ~ArrayItemProxyPrivate()
   {
      // here we check some things
      if (m_needCheckRequestItem) {
         m_apiPtr->retrieveZvalPtr();
      }
   }
   ZAPI_DECLARE_PUBLIC(ArrayItemProxy)
   ArrayItemProxy::KeyType m_requestKey;
   zend_array *m_array;
   bool m_needCheckRequestItem = true;
   ArrayItemProxy *m_parent = nullptr;
   ArrayItemProxy *m_apiPtr;
};

} // internal

using zapi::ds::internal::ArrayItemProxyPrivate;

ArrayItemProxy::ArrayItemProxy(zend_array *array, const KeyType &requestKey)
   : m_implPtr(new ArrayItemProxyPrivate(array, requestKey, this))
{}

ArrayItemProxy::ArrayItemProxy(zend_array *array, const std::string &key)
   : m_implPtr(new ArrayItemProxyPrivate(array, key, this))
{}

ArrayItemProxy::ArrayItemProxy(zend_array *array, zapi_ulong index)
   : m_implPtr(new ArrayItemProxyPrivate(array, index, this))
{
   
}

ArrayItemProxy::~ArrayItemProxy()
{}

ArrayItemProxy::operator NumericVariant()
{
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   Type type = value.getType();
   if (type != Type::Long && type != Type::Double) {
      zapi::notice << "Array proxy type "<< value.getTypeStr() 
                   << " not compatible with NumericVariant" << std::endl;
   }
   return NumericVariant(std::move(value));
}

ArrayItemProxy::operator DoubleVariant()
{
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   Type type = value.getType();
   if (type != Type::Long && type != Type::Double) {
      zapi::notice << "Array proxy type "<< value.getTypeStr() 
                   << "not compatible with DoubleVariant" << std::endl;
   }
   return DoubleVariant(std::move(value));
}

ArrayItemProxy::operator Variant()
{
   zval *value = retrieveZvalPtr();
   m_implPtr->m_needCheckRequestItem = false;
   return Variant(value);
}

ArrayItemProxy::operator StringVariant()
{
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   Type type = value.getType();
   if (type != Type::Long && type != Type::Double &&
       type != Type::String && type != Type::Boolean) {
      zapi::notice << "Array proxy type "<< value.getTypeStr() 
                   << "not compatible with StringVariant" << std::endl;
   }
   return StringVariant(std::move(value));
}

ArrayItemProxy::operator BoolVariant()
{
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   return BoolVariant(std::move(value));
}

ArrayItemProxy::operator ArrayVariant()
{
   return ArrayVariant();
}

ArrayItemProxy ArrayItemProxy::operator [](zapi_long index)
{
   // we check parent item exist here
   
}

ArrayItemProxy ArrayItemProxy::operator [](const std::string &key)
{
   // we check parent item exist here
}

zval *ArrayItemProxy::recursiveEnsureArrayExist()
{
   
}

zval *ArrayItemProxy::retrieveZvalPtr(bool quiet) const
{
   zval *valPtr = nullptr;
   if (m_implPtr->m_requestKey.second) {
      std::string *key = m_implPtr->m_requestKey.second.get();
      valPtr = zend_hash_str_find(m_implPtr->m_array, key->c_str(), key->length());
      if (nullptr == valPtr && !quiet) {
         zapi::notice << "Undefined offset: " << *key << std::endl;
      }
   } else {
      valPtr = zend_hash_index_find(m_implPtr->m_array, m_implPtr->m_requestKey.first);
      if (nullptr == valPtr && !quiet) {
         zapi::notice << "Undefined index: " << m_implPtr->m_requestKey.first << std::endl;
      }
   }
   return valPtr;
}

} // ds
} // zapi