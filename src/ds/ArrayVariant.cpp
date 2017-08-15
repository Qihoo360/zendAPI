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
#include "zapi/ds/internal/VariantPrivate.h"
#include <iostream>

namespace zapi
{
namespace ds
{

using zapi::ds::internal::VariantPrivate;

// iterator class alias
using ArrayIterator = ArrayVariant::Iterator;
using ConstArrayIterator = ArrayVariant::ConstIterator;

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

ArrayIterator ArrayVariant::insert(zapi_ulong index, const Variant &value)
{
   deployCopyOnWrite();
   zval *zvalPtr = const_cast<zval *>(value.getZvalPtr());
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY(&temp, zvalPtr);
   zend_array *selfArrPtr = getZendArrayPtr();
   zval *valPtr = zend_hash_index_update(selfArrPtr, index, &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, calculateIdxFromZval(valPtr));
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
}

ArrayIterator ArrayVariant::insert(zapi_ulong index, Variant &&value)
{
   deployCopyOnWrite();
   zval *zvalPtr = value.getZvalPtr();
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY_VALUE(&temp, zvalPtr);
   std::memset(&value.m_implPtr->m_buffer, 0, sizeof(value.m_implPtr->m_buffer));
   zend_array *selfArrPtr = getZendArrayPtr();
   zval *valPtr = zend_hash_index_update(selfArrPtr, index, &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, calculateIdxFromZval(valPtr));
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
}

ArrayIterator ArrayVariant::insert(const std::string &key, const Variant &value)
{
   deployCopyOnWrite();
   zval *zvalPtr = const_cast<zval *>(value.getZvalPtr());
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY(&temp, zvalPtr);
   zend_array *selfArrPtr = getZendArrayPtr();
   zval *valPtr = zend_hash_str_update(selfArrPtr, key.c_str(), key.length(), &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, calculateIdxFromZval(valPtr));
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
}

ArrayIterator ArrayVariant::insert(const std::string &key, Variant &&value)
{
   deployCopyOnWrite();
   zval *zvalPtr = value.getZvalPtr();
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY_VALUE(&temp, zvalPtr);
   std::memset(&value.m_implPtr->m_buffer, 0, sizeof(value.m_implPtr->m_buffer));
   zend_array *selfArrPtr = getZendArrayPtr();
   zval *valPtr = zend_hash_str_update(selfArrPtr, key.c_str(), key.length(), &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, calculateIdxFromZval(valPtr));
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
}

ArrayIterator ArrayVariant::append(const Variant &value)
{
   deployCopyOnWrite();
   zval *zvalPtr = const_cast<zval *>(value.getZvalPtr());
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY(&temp, zvalPtr);
   zend_array *selfArrPtr = getZendArrayPtr();
   zapi_long cindex = selfArrPtr->nNextFreeElement;
   zval *valPtr = zend_hash_next_index_insert(selfArrPtr, &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, cindex);
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
}

ArrayIterator ArrayVariant::append(Variant &&value)
{
   deployCopyOnWrite();
   zval *zvalPtr = value.getZvalPtr();
   zval temp;
   ZVAL_DEREF(zvalPtr);
   ZVAL_COPY_VALUE(&temp, zvalPtr);
   std::memset(&value.m_implPtr->m_buffer, 0, sizeof(value.m_implPtr->m_buffer));
   zend_array *selfArrPtr = getZendArrayPtr();
   zapi_long cindex = selfArrPtr->nNextFreeElement;
   zval *valPtr = zend_hash_next_index_insert(selfArrPtr, &temp);
   if (valPtr) {
      return ArrayIterator(selfArrPtr, cindex);
   } else {
      return ArrayIterator(selfArrPtr, HT_INVALID_IDX);
   }
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

Variant ArrayVariant::getValue(zapi_long index) const
{
   zval *val = zend_hash_index_find(getZendArrayPtr(), index);
   if (nullptr == val) {
      zapi::notice << "Undefined offset: " << index << std::endl;
   }
   return val;
}

Variant ArrayVariant::getValue(const std::string &key) const
{
   zval *val = zend_hash_str_find(getZendArrayPtr(), key.c_str(), key.length());
   if (nullptr == val) {
      zapi::notice << "Undefined index: " << key << std::endl;
   }
   return val;
}

ArrayIterator ArrayVariant::begin() ZAPI_DECL_NOEXCEPT
{
   return ArrayIterator(getZendArrayPtr(), 0);
}

ConstArrayIterator ArrayVariant::begin() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), 0);
}

ConstArrayIterator ArrayVariant::cbegin() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), 0);
}

ArrayIterator ArrayVariant::end() ZAPI_DECL_NOEXCEPT
{
   return ArrayIterator(getZendArrayPtr(), HT_INVALID_IDX);
}

ConstArrayIterator ArrayVariant::end() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), HT_INVALID_IDX);
}

ConstArrayIterator ArrayVariant::cend() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), HT_INVALID_IDX);
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
      array_init(self);
   } else {
      SEPARATE_ZVAL_NOREF(self);
   }
}

uint32_t ArrayVariant::calculateIdxFromZval(zval *val) const ZAPI_DECL_NOEXCEPT
{
   zend_array *arr = getZendArrayPtr();
   return (reinterpret_cast<char *>(val) - reinterpret_cast<char *>(arr->arData)) / sizeof(Bucket);
}

// iterator classes

ArrayIterator::Iterator(_zend_array *array, HashPosition index)
   : m_array(array),
     m_index(index)
{
   ZAPI_ASSERT_X(m_array != nullptr, "zapi::ds::ArrayVariant::Iterator", "m_array can't be nullptr");
}

Variant ArrayIterator::getValue()
{
   return zend_hash_get_current_data_ex(m_array, &m_index);
}

ArrayIterator::ZvalReference ArrayIterator::getZval()
{
   zval *valPtr = zend_hash_get_current_data_ex(m_array, &m_index);
   assert(nullptr != valPtr);
   return *valPtr;
}

ArrayIterator::ZvalPointer ArrayIterator::getZvalPtr()
{
   return zend_hash_get_current_data_ex(m_array, &m_index);
}

ArrayVariant::KeyType ArrayIterator::getKey()
{
   
}

// operators
ArrayIterator::ZvalReference ArrayIterator::operator *()
{
   
}

ArrayIterator::ZvalPointer ArrayIterator::operator->()
{
   
}

bool ArrayIterator::operator ==(const Iterator &other)
{
   
}

bool ArrayIterator::operator !=(const Iterator &other)
{
   
}

ArrayIterator &ArrayIterator::operator ++()
{
   
}

ArrayIterator ArrayIterator::operator ++(int)
{
   
}

ArrayIterator &ArrayIterator::operator --()
{
   
}

ArrayIterator ArrayIterator::operator --(int)
{
   
}

ArrayIterator ArrayIterator::operator +(int32_t step) const
{
   
}

ArrayIterator ArrayIterator::operator -(int32_t step) const
{
   
}

ArrayIterator &ArrayIterator::operator +=(int32_t step)
{
   
}

ArrayIterator &ArrayIterator::operator -=(int32_t step)
{
   
}

ConstArrayIterator::ConstIterator(_zend_array *array, HashPosition index)
   : m_array(array),
     m_index(index)
{
   ZAPI_ASSERT_X(m_array != nullptr, "zapi::ds::ArrayVariant::Iterator", "m_array can't be nullptr");
}

const Variant ConstArrayIterator::getValue() const
{
   
}

ConstArrayIterator::ZvalReference ConstArrayIterator::getZval() const
{
   
}

ConstArrayIterator::ZvalPointer ConstArrayIterator::getZvalPtr() const
{
   
}

const ArrayVariant::KeyType ConstArrayIterator::getKey() const
{
   
}

// operators
const zval &ConstArrayIterator::operator *()
{
   
}

zval *ConstArrayIterator::operator->()
{
   
}

bool ConstArrayIterator::operator ==(const Iterator &other) const
{
   
}

bool ConstArrayIterator::operator !=(const Iterator &other) const
{
   
}

ConstArrayIterator &ConstArrayIterator::operator ++()
{
   
}

ConstArrayIterator ConstArrayIterator::operator ++(int)
{
   
}

ConstArrayIterator &ConstArrayIterator::operator --()
{
   
}

ConstArrayIterator ConstArrayIterator::operator --(int)
{
   
}

ConstArrayIterator ConstArrayIterator::operator +(int32_t step) const
{
   
}

ConstArrayIterator ConstArrayIterator::operator -(int32_t step) const
{
   
}

ConstArrayIterator &ConstArrayIterator::operator +=(int32_t step)
{
   
}

ConstArrayIterator &ConstArrayIterator::operator -=(int32_t step)
{
   
}

} // ds
} // zapi