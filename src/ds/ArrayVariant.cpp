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
#include <string>

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

ArrayVariant::ArrayVariant(const ArrayVariant &other)
{
   // here we just need copy array zval
   zval *self = getZvalPtr();
   zval *from = const_cast<zval *>(other.getZvalPtr());
   ZVAL_COPY(self, from);
}

ArrayVariant::ArrayVariant(ArrayVariant &&other) ZAPI_DECL_NOEXCEPT
   : Variant(std::move(other))
{}

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

ArrayItemProxy ArrayVariant::operator [](const char *key)
{
   return ArrayItemProxy(getZvalPtr(), key);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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
      HashPosition pos = calculateIdxFromZval(valPtr);
      return ArrayIterator(selfArrPtr, &pos);
   } else {
      return ArrayIterator(selfArrPtr, nullptr);
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

Variant ArrayVariant::getValue(zapi_ulong index) const
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

bool ArrayVariant::contains(zapi_ulong index) const
{
   return zend_hash_index_find(getZendArrayPtr(), index) != nullptr;
}

bool ArrayVariant::contains(const std::string &key) const
{
   return zend_hash_str_find(getZendArrayPtr(), key.c_str(), key.length());
}

ArrayIterator ArrayVariant::begin() ZAPI_DECL_NOEXCEPT
{
   HashPosition pos = 0;
   return ArrayIterator(getZendArrayPtr(), &pos);
}

ConstArrayIterator ArrayVariant::begin() const ZAPI_DECL_NOEXCEPT
{
   HashPosition pos = 0;
   return ConstArrayIterator(getZendArrayPtr(), &pos);
}

ConstArrayIterator ArrayVariant::cbegin() const ZAPI_DECL_NOEXCEPT
{
   HashPosition pos = 0;
   return ConstArrayIterator(getZendArrayPtr(), &pos);
}

ArrayIterator ArrayVariant::end() ZAPI_DECL_NOEXCEPT
{
   return ArrayIterator(getZendArrayPtr(), nullptr);
}

ConstArrayIterator ArrayVariant::end() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), nullptr);
}

ConstArrayIterator ArrayVariant::cend() const ZAPI_DECL_NOEXCEPT
{
   return ConstArrayIterator(getZendArrayPtr(), nullptr);
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

ArrayIterator::Iterator(_zend_array *array, HashPosition *pos)
   : m_array(array),
     m_isEnd(nullptr != pos ? false : true)
{
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator", "m_array can't be nullptr");
   if (!m_isEnd) {
      if (0 == *pos) {
         // auto detect
         zend_hash_internal_pointer_reset_ex(m_array, pos);
      }
      m_idx = zend_hash_iterator_add(m_array, *pos);
   }
}

ArrayIterator::Iterator(const Iterator &other)
{
   // manual copy if not end iterator
   // we create a new iterator here
   m_isEnd = other.m_isEnd;
   m_array = other.m_array;
   if (!m_isEnd) {
      HashPosition otherPos = zend_hash_iterator_pos(other.m_idx, other.m_array);
      m_idx = zend_hash_iterator_add(m_array, otherPos);
   }
}

ArrayIterator::Iterator(Iterator &&other) ZAPI_DECL_NOEXCEPT
{
   m_isEnd = other.m_isEnd;
   m_array = other.m_array;
   m_idx = other.m_idx;
   // prevent relase iterator
   other.m_isEnd = false;
}

ArrayIterator::~Iterator()
{
   if (!m_isEnd) {
      zend_hash_iterator_del(m_idx);
   }
}

Variant ArrayIterator::getValue() const
{
   return getZvalPtr();
}

ArrayIterator::ZvalReference ArrayIterator::getZval() const
{
   HashPosition pos = getCurrentPos();
   zval *valPtr = zend_hash_get_current_data_ex(m_array, &pos);
   ZAPI_ASSERT_X(valPtr != nullptr, "ArrayVariant::Iterator::getZval", "can't apply * operator on nullptr");
   return *valPtr;
}

ArrayIterator::ZvalPointer ArrayIterator::getZvalPtr() const
{
   HashPosition pos = getCurrentPos();
   return zend_hash_get_current_data_ex(m_array, &pos);
}

ArrayVariant::KeyType ArrayIterator::getKey() const
{
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator::getKey", "m_array can't be nullptr");
   KeyType key;
   zend_string *keyStr;
   zapi_ulong index;
   HashPosition pos = getCurrentPos();
   int keyType = zend_hash_get_current_key_ex(m_array, &keyStr, &index, &pos);
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "ArrayVariant::Iterator::getKey", "Key can't not exist");
   if (HASH_KEY_IS_STRING == keyType) {
      key.second.reset(new std::string(ZSTR_VAL(keyStr), ZSTR_LEN(keyStr))); 
   } else {
      key.first = index;
   }
   return key;
}

HashPosition ArrayIterator::getCurrentPos() const
{
   if (m_isEnd) {
      return HT_INVALID_IDX;
   }
   return zend_hash_iterator_pos(m_idx, m_array);
}

// operators
ArrayIterator::ZvalReference ArrayIterator::operator *()
{
   return getZval();
}

ArrayIterator::ZvalPointer ArrayIterator::operator->()
{
   return getZvalPtr();
}

ArrayIterator &ArrayIterator::operator =(const ArrayIterator &other)
{
   if (this != &other) {
      m_array = other.m_array;
      if (!other.m_isEnd && m_isEnd) {
         m_isEnd = false;
         HashPosition otherPos = zend_hash_iterator_pos(other.m_idx, other.m_array);
         m_idx = zend_hash_iterator_add(m_array, otherPos);
      } else if (!other.m_isEnd && !m_isEnd) {
         EG(ht_iterators)[m_idx].pos = zend_hash_iterator_pos(other.m_idx, other.m_array);
         EG(ht_iterators)[m_idx].ht = other.m_array;
      } else if (other.m_isEnd && !m_isEnd) {
         m_isEnd = true;
         zend_hash_iterator_del(m_idx);
         m_idx = -1;
      }
   }
   return *this;
}

ArrayIterator &ArrayIterator::operator =(ArrayIterator &&other) ZAPI_DECL_NOEXCEPT
{
   assert(this != &other);
   m_array = other.m_array;
   if (!other.m_isEnd && m_isEnd) {
      std::swap(m_isEnd, other.m_isEnd);
      std::swap(m_idx, other.m_idx);
   } else if (!other.m_isEnd && !m_isEnd) {
      EG(ht_iterators)[m_idx].pos = zend_hash_iterator_pos(other.m_idx, other.m_array);
      EG(ht_iterators)[m_idx].ht = other.m_array;
   } else if (other.m_isEnd && !m_isEnd) {
      std::swap(m_isEnd, other.m_isEnd);
      std::swap(m_idx, other.m_idx);
   }
   return *this;
}

bool ArrayIterator::operator ==(const ArrayIterator &other)
{
   return m_array == other.m_array &&
         getCurrentPos() == other.getCurrentPos();
}

bool ArrayIterator::operator !=(const ArrayIterator &other)
{
   return m_array != other.m_array ||
         getCurrentPos() != other.getCurrentPos();
}

ArrayIterator &ArrayIterator::operator ++()
{
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator", "m_array can't be nullptr");
   HashPosition pos = getCurrentPos();
   int result = zend_hash_move_forward_ex(m_array, &pos);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "ArrayVariant::Iterator", "Iterating beyond end()");
   EG(ht_iterators)[m_idx].pos = pos;
   return *this;
}

ArrayIterator ArrayIterator::operator ++(int)
{
   ArrayIterator iter = *this;
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator", "m_array can't be nullptr");
   HashPosition pos = getCurrentPos();
   int result = zend_hash_move_forward_ex(m_array, &pos);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "ArrayVariant::Iterator", "Iterating beyond end()");
   EG(ht_iterators)[m_idx].pos = pos;
   return iter;
}

ArrayIterator &ArrayIterator::operator --()
{
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator", "m_array can't be nullptr");
   HashPosition pos = getCurrentPos();
   int result = zend_hash_move_backwards_ex(m_array, &pos);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "ArrayVariant::Iterator", "Iterating beyond end()");
   EG(ht_iterators)[m_idx].pos = pos;
   return *this;
}

ArrayIterator ArrayIterator::operator --(int)
{
   ArrayIterator iter = *this;
   ZAPI_ASSERT_X(m_array != nullptr, "ArrayVariant::Iterator", "m_array can't be nullptr");
   HashPosition pos = getCurrentPos();
   int result = zend_hash_move_backwards_ex(m_array, &pos);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "ArrayVariant::Iterator", "Iterating beyond end()");
   EG(ht_iterators)[m_idx].pos = pos;
   return iter;
}

ArrayIterator ArrayIterator::operator +(int32_t step) const
{
   Iterator iter = *this;
   if (step > 0) {
      while (step--) {
         ++iter;
      }
   } else {
      while (step++) {
         --iter;
      }
   }
   return iter;
}

ArrayIterator ArrayIterator::operator -(int32_t step) const
{
   return operator +(-step);
}

ArrayIterator &ArrayIterator::operator +=(int32_t step)
{
   *this = *this + step;
   return *this;
}

ArrayIterator &ArrayIterator::operator -=(int32_t step)
{
   *this = *this - step;
   return *this;
}

ConstArrayIterator::ConstIterator(_zend_array *array, HashPosition *pos)
   : Iterator(array, pos)
{}

ConstArrayIterator::ConstIterator(const ConstIterator &other)
   : Iterator(other)
{}

ConstArrayIterator::ConstIterator(ConstIterator &&other) ZAPI_DECL_NOEXCEPT
   : Iterator(other)
{}

ConstArrayIterator::~ConstIterator()
{}

const Variant ConstArrayIterator::getValue() const
{
   return Iterator::getZvalPtr();
}

ConstArrayIterator::ZvalReference ConstArrayIterator::getZval() const
{
   return Iterator::getZval();
}

ConstArrayIterator::ZvalPointer ConstArrayIterator::getZvalPtr() const
{
   return Iterator::getZvalPtr();
}

const ArrayVariant::KeyType ConstArrayIterator::getKey() const
{
   return Iterator::getKey();
}

// operators
ConstArrayIterator &ConstArrayIterator::operator =(const ConstArrayIterator &other)
{
   if (this != &other) {
      Iterator::operator =(other);
   }
   return *this;
}

ConstArrayIterator &ConstArrayIterator::operator =(ConstArrayIterator &&other) ZAPI_DECL_NOEXCEPT
{
   assert(this != &other);
   Iterator::operator =(std::move(other));
   return *this;
}

ConstArrayIterator::ZvalReference ConstArrayIterator::operator *()
{
   return getZval();
}

ConstArrayIterator::ZvalPointer ConstArrayIterator::operator->()
{
   return getZvalPtr();
}

bool ConstArrayIterator::operator ==(const ConstIterator &other) const
{
   return m_array == other.m_array &&
         getCurrentPos() == other.getCurrentPos();
}

bool ConstArrayIterator::operator !=(const ConstIterator &other) const
{
   return m_array != other.m_array ||
         getCurrentPos() != other.getCurrentPos();
}

ConstArrayIterator &ConstArrayIterator::operator ++()
{
   Iterator::operator ++();
   return *this;
}

ConstArrayIterator ConstArrayIterator::operator ++(int)
{
   ConstIterator iter = *this;
   Iterator::operator ++(1);
   return iter;
}

ConstArrayIterator &ConstArrayIterator::operator --()
{
   Iterator::operator --();
   return *this;
}

ConstArrayIterator ConstArrayIterator::operator --(int)
{
   ConstIterator iter = *this;
   Iterator::operator --(1);
   return iter;
}

ConstArrayIterator ConstArrayIterator::operator +(int32_t step) const
{
   Iterator::operator +(step);
   return *this;
}

ConstArrayIterator ConstArrayIterator::operator -(int32_t step) const
{
   Iterator::operator -(step);
   return *this;
}

ConstArrayIterator &ConstArrayIterator::operator +=(int32_t step)
{
   Iterator::operator +=(step);
   return *this;
}

ConstArrayIterator &ConstArrayIterator::operator -=(int32_t step)
{
   Iterator::operator -=(step);
   return *this;
}

} // ds
} // zapi