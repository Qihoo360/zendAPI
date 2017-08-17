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
#include "zapi/ds/internal/ArrayItemProxyPrivate.h"
#include "zapi/ds/NumericVariant.h"
#include "zapi/ds/DoubleVariant.h"
#include "zapi/ds/StringVariant.h"
#include "zapi/ds/BoolVariant.h"
#include "zapi/utils/CommonFuncs.h"
#include <iostream>
#include <string>
#include <typeinfo>

namespace zapi
{
namespace ds
{

using zapi::ds::internal::ArrayItemProxyPrivate;
using KeyType = zapi::ds::ArrayItemProxy::KeyType;

namespace 
{

void print_key_not_exist_notice(const KeyType &key)
{
   if (key.second) {
      std::string *keyStr = key.second.get();
      zapi::notice << "Undefined offset: " << *keyStr << std::endl;
   } else {
      zapi::notice << "Undefined index: " << key.first << std::endl;
   }
}

void print_type_not_compatible_info(const zval *valPtr)
{
   switch (Z_TYPE_P(valPtr)) {
   case IS_OBJECT:
      zapi::error << "Can't use object type as array" << std::endl;
      break;
   case IS_STRING:
      zapi::error << "Can't use string offset as an array" << std::endl;
      break;
   case IS_TRUE:
   case IS_FALSE:
   case _IS_BOOL:
   case IS_DOUBLE:
   case IS_LONG:
      zapi::warning << "Can't use a scalar value as an array" << std::endl;
      break;
   default:
      zapi::warning << "Can't use type of " << zapi::utils::get_zval_type_str(valPtr)
                    << " as array " << std::endl;
      break;
   }
}

}

ArrayItemProxy::ArrayItemProxy(const ArrayItemProxy &other)
   : m_implPtr(other.m_implPtr)
{}

ArrayItemProxy::ArrayItemProxy(ArrayItemProxy &&other) ZAPI_DECL_NOEXCEPT
   : m_implPtr(std::move(other.m_implPtr))
{}

ArrayItemProxy::ArrayItemProxy(zval *array, const KeyType &requestKey, ArrayItemProxy *parent)
   : m_implPtr(new ArrayItemProxyPrivate(array, requestKey, this, parent))
{}

ArrayItemProxy::ArrayItemProxy(zval *array, const std::string &key, ArrayItemProxy *parent)
   : m_implPtr(new ArrayItemProxyPrivate(array, key, this, parent))
{}

ArrayItemProxy::ArrayItemProxy(zval *array, zapi_ulong index, ArrayItemProxy *parent)
   : m_implPtr(new ArrayItemProxyPrivate(array, index, this, parent))
{}

ArrayItemProxy::~ArrayItemProxy()
{}

ArrayItemProxy &ArrayItemProxy::operator =(const ArrayItemProxy &other)
{
   if (this != &other) {
      m_implPtr = other.m_implPtr;
   }
   return *this;
}

ArrayItemProxy &ArrayItemProxy::operator =(ArrayItemProxy &&other) ZAPI_DECL_NOEXCEPT
{
   assert(this != &other);
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

ArrayItemProxy &ArrayItemProxy::operator =(const Variant &value)
{
   if (!m_implPtr->m_array) {
      if (!ensureArrayExistRecusive(m_implPtr->m_array, m_implPtr->m_requestKey, this)){
         // have something wrong
         // just return without change anything
         // don't check recursive, because we already do it
         m_implPtr->m_parent = nullptr;
         m_implPtr->m_needCheckRequestItem = false;
         return *this;
      }
   }
   SEPARATE_ZVAL_NOREF(m_implPtr->m_array);
   // here we don't check exist, we just insert it if not exists
   m_implPtr->m_needCheckRequestItem = false;
   zval *from = const_cast<zval *>(value.getZvalPtr());
   zval temp;
   ZVAL_DEREF(from);
   ZVAL_COPY(&temp, from);
   zend_array *target = Z_ARRVAL_P(m_implPtr->m_array);
   zval *inserted = nullptr;
   if (m_implPtr->m_requestKey.second) {
      std::string *key = m_implPtr->m_requestKey.second.get();
      inserted = zend_hash_str_update(target, key->c_str(), key->length(), &temp);
   } else {
      inserted = zend_hash_index_update(target, m_implPtr->m_requestKey.first, &temp);
   }
   // @TODO here we need check the inserted ?
   return *this;
}

ArrayItemProxy &ArrayItemProxy::operator =(const NumericVariant &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const DoubleVariant &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const StringVariant &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const BoolVariant &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const ArrayVariant &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(NumericVariant &&value)
{
   return operator =(Variant(std::move(value)));
}

ArrayItemProxy &ArrayItemProxy::operator =(DoubleVariant &&value)
{
   return operator =(Variant(std::move(value)));
}

ArrayItemProxy &ArrayItemProxy::operator =(StringVariant &&value)
{
   return operator =(Variant(std::move(value)));
}

ArrayItemProxy &ArrayItemProxy::operator =(BoolVariant &&value)
{
   return operator =(Variant(std::move(value)));
}

ArrayItemProxy &ArrayItemProxy::operator =(ArrayVariant &&value)
{
   return operator =(Variant(std::move(value)));
}

ArrayItemProxy &ArrayItemProxy::operator =(const char *value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const char value)
{
   return operator =(Variant(value));
}

ArrayItemProxy &ArrayItemProxy::operator =(const std::string &value)
{
   return operator =(Variant(value));
}

ArrayItemProxy::operator NumericVariant()
{
   return toNumericVariant();
}

ArrayItemProxy::operator DoubleVariant()
{
   return toDoubleVariant();
}

ArrayItemProxy::operator Variant()
{
   return toVariant();
}

ArrayItemProxy::operator StringVariant()
{
   return toStringVariant();
}

ArrayItemProxy::operator BoolVariant()
{
   return toBoolVariant();
}

ArrayItemProxy::operator ArrayVariant()
{
   return toArrayVariant();
}

Variant ArrayItemProxy::toVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
   zval *value = retrieveZvalPtr();
   m_implPtr->m_needCheckRequestItem = false;
   return Variant(value);
}

NumericVariant ArrayItemProxy::toNumericVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   Type type = value.getType();
   if (type != Type::Long && type != Type::Double) {
      zapi::notice << "Array proxy type "<< value.getTypeStr() 
                   << " not compatible with NumericVariant" << std::endl;
   }
   return NumericVariant(std::move(value));
}

DoubleVariant ArrayItemProxy::toDoubleVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   Type type = value.getType();
   if (type != Type::Long && type != Type::Double) {
      zapi::notice << "Array proxy type "<< value.getTypeStr() 
                   << "not compatible with DoubleVariant" << std::endl;
   }
   return DoubleVariant(std::move(value));
}

StringVariant ArrayItemProxy::toStringVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
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

BoolVariant ArrayItemProxy::toBoolVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
   Variant value(retrieveZvalPtr());
   m_implPtr->m_needCheckRequestItem = false;
   return BoolVariant(std::move(value));
}

ArrayVariant ArrayItemProxy::toArrayVariant()
{
   if (!isKeychianOk(false)) {
      throw std::bad_cast();
   }
   return ArrayVariant();
}

ArrayItemProxy ArrayItemProxy::operator [](zapi_long index)
{
   m_implPtr->m_needCheckRequestItem = false;
   // let most derived proxy object do check
   return ArrayItemProxy(nullptr, index, this);
}

ArrayItemProxy ArrayItemProxy::operator [](const std::string &key)
{
   m_implPtr->m_needCheckRequestItem = false;
   // let most derived proxy object do check
   return ArrayItemProxy(nullptr, key, this);
}

bool ArrayItemProxy::ensureArrayExistRecusive(zval *&childArrayPtr,const KeyType &childRequestKey,
                                              ArrayItemProxy *mostDerivedProxy)
{
   // if a ArrayItemProxyPrivate both m_parent and m_array is 
   // nullptr, this is a bug, let me know.
   if (m_implPtr->m_parent) {
      if (!m_implPtr->m_parent->ensureArrayExistRecusive(m_implPtr->m_array, m_implPtr->m_requestKey,
                                                    mostDerivedProxy)){
         return false;
      }
   }
   if (this != mostDerivedProxy) {
      // here we don't need check exist in destroy process
      m_implPtr->m_needCheckRequestItem = false;
      const KeyType &requestKey = m_implPtr->m_requestKey;
      // at this point m_array must be exist
      // when m_parent is nullptr the m_array is top array self
      zval *val = retrieveZvalPtr(true);
      if (nullptr == val) {
         zval temp;
         array_init(&temp);
         if (requestKey.second) {
            std::string *keyStr = requestKey.second.get();
            // @TODO here we need check status ?
            childArrayPtr = zend_hash_str_add(Z_ARR_P(m_implPtr->m_array), keyStr->c_str(), keyStr->length(), &temp);
         } else {
            childArrayPtr = zend_hash_index_add(Z_ARR_P(m_implPtr->m_array), requestKey.first, &temp);
         }
      } else {
         // if request key exists and check type compatible
         // if child request key exists we must self must be array
         if (Z_TYPE_P(val) != IS_ARRAY) {
            print_type_not_compatible_info(val);
            return false;
         } else {
            childArrayPtr = val;
         }
      }
   }
   return true;
}

void ArrayItemProxy::checkExistRecursive(bool &stop, zval *&childArrayPtr, ArrayItemProxy *mostDerivedProxy,
                                         bool quiet)
{
   if (m_implPtr->m_parent) {
      m_implPtr->m_parent->checkExistRecursive(stop, m_implPtr->m_array, mostDerivedProxy, quiet);
      m_implPtr->m_parent = nullptr;
   } 
   if (!stop) {
      // check self
      if (!m_implPtr->m_array) {
         if (!quiet) {
            print_key_not_exist_notice(m_implPtr->m_requestKey);
         }
         stop = true;
      } else {
         // check self array
         zval *valuePtr = retrieveZvalPtr(true);
         if (!valuePtr) {
            if (!quiet) {
               print_key_not_exist_notice(m_implPtr->m_requestKey);
            }
            stop = true;
         } else if (this != mostDerivedProxy){
            // if request key exists and check type compatible
            // if child request key exists we must self must be array
            if (Z_TYPE_P(valuePtr) != IS_ARRAY) {
               if (!quiet) {
                  print_type_not_compatible_info(valuePtr);
               }
               stop = true;
            } else {
               childArrayPtr = valuePtr;
            }
         }
      }
   }
   m_implPtr->m_needCheckRequestItem = false;
}

bool ArrayItemProxy::isKeychianOk(bool quiet)
{
   bool exist = false;
   if (m_implPtr->m_parent) {
      bool stop = false;
      checkExistRecursive(stop, m_implPtr->m_array, m_implPtr->m_apiPtr, quiet);
      exist = !stop;
   } else {
      exist = nullptr != retrieveZvalPtr(true);
   }
   m_implPtr->m_needCheckRequestItem = false;
   return exist;
}

zval *ArrayItemProxy::retrieveZvalPtr(bool quiet) const
{
   zval *valPtr = nullptr;
   if (m_implPtr->m_requestKey.second) {
      std::string *key = m_implPtr->m_requestKey.second.get();
      valPtr = zend_hash_str_find(Z_ARRVAL_P(m_implPtr->m_array), key->c_str(), key->length());
      if (nullptr == valPtr && !quiet) {
         zapi::notice << "Undefined offset: " << *key << std::endl;
      }
   } else {
      valPtr = zend_hash_index_find(Z_ARRVAL_P(m_implPtr->m_array), m_implPtr->m_requestKey.first);
      if (nullptr == valPtr && !quiet) {
         zapi::notice << "Undefined index: " << m_implPtr->m_requestKey.first << std::endl;
      }
   }
   return valPtr;
}

} // ds
} // zapi