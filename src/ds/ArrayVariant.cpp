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

#if ZEND_DEBUG

#define HASH_MASK_CONSISTENCY	0xc0
#define HT_OK					0x00
#define HT_IS_DESTROYING		0x40
#define HT_DESTROYED			0x80
#define HT_CLEANING				0xc0

static void _zend_is_inconsistent(const HashTable *ht, const char *file, int line)
{
   if ((ht->u.flags & HASH_MASK_CONSISTENCY) == HT_OK) {
      return;
   }
   switch ((ht->u.flags & HASH_MASK_CONSISTENCY)) {
   case HT_IS_DESTROYING:
      zend_output_debug_string(1, "%s(%d) : ht=%p is being destroyed", file, line, ht);
      break;
   case HT_DESTROYED:
      zend_output_debug_string(1, "%s(%d) : ht=%p is already destroyed", file, line, ht);
      break;
   case HT_CLEANING:
      zend_output_debug_string(1, "%s(%d) : ht=%p is being cleaned", file, line, ht);
      break;
   default:
      zend_output_debug_string(1, "%s(%d) : ht=%p is inconsistent", file, line, ht);
      break;
   }
   zend_bailout();
}
#define IS_CONSISTENT(a) _zend_is_inconsistent(a, __FILE__, __LINE__);
#define SET_INCONSISTENT(n) do { \
   (ht)->u.flags |= n; \
   } while (0)
#else
#define IS_CONSISTENT(a)
#define SET_INCONSISTENT(n)
#endif

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
   array_init(self);
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

bool ArrayVariant::operator ==(const ArrayVariant &other) const
{
   return this == &other ? true 
                         : 0 == zend_compare_arrays(const_cast<zval *>(getZvalPtr()), const_cast<zval *>(other.getZvalPtr()));
}

bool ArrayVariant::operator !=(const ArrayVariant &other) const
{
   return !operator ==(other);
}

bool ArrayVariant::strictEqual(const Variant &other) const
{
   
}

bool ArrayVariant::strictNotEqual(const Variant &other) const
{
   
}

ArrayIterator ArrayVariant::insert(zapi_ulong index, const Variant &value)
{
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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
   SEPARATE_ZVAL_NOREF(getZvalPtr());
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

void ArrayVariant::clear() ZAPI_DECL_NOEXCEPT
{
   zend_hash_clean(getZendArrayPtr());
}

bool ArrayVariant::remove(zapi_ulong index) ZAPI_DECL_NOEXCEPT
{
   return zend_hash_index_del(getZendArrayPtr(), index) == ZAPI_SUCCESS;
}

bool ArrayVariant::remove(const std::string &key) ZAPI_DECL_NOEXCEPT
{
   return zend_hash_str_del(getZendArrayPtr(), key.c_str(), key.length()) == ZAPI_SUCCESS;
}

ArrayVariant::Iterator ArrayVariant::erase(ConstIterator &iter)
{
   zend_array *array = getZendArrayPtr();
   // here we need check the iter check whether iter pointer this array
   if (iter == cend() || array != iter.m_array) {
      return iter;
   }
   Iterator nextIter = iter;
   ++nextIter;
   KeyType key = iter.getKey();
   int deleteStatus;
   if (key.second) {
      std::string *keyStr = key.second.get();
      deleteStatus = zend_hash_str_del(array, keyStr->c_str(), keyStr->length());
   } else {
      deleteStatus = zend_hash_index_del(array, key.first);
   }
   if (ZAPI_FAILURE == deleteStatus) {
      return iter;
   }
   return nextIter;
}

ArrayVariant::Iterator ArrayVariant::erase(Iterator &iter)
{
   return erase(static_cast<ConstIterator &>(iter));
}

Variant ArrayVariant::take(const std::string &key)
{
   Iterator iter = find(key);
   Variant ret(iter.getValue());
   if (iter != end()) {
      remove(key);
   }
   return ret;
}

Variant ArrayVariant::take(zapi_ulong index)
{
   Iterator iter = find(index);
   Variant ret(iter.getValue());
   if (iter != end()) {
      remove(index);
   }
   return ret;
}

bool ArrayVariant::isEmpty() const ZAPI_DECL_NOEXCEPT
{
   return 0 == getSize();
}

bool ArrayVariant::isNull() const ZAPI_DECL_NOEXCEPT
{
   return false;
}

ArrayVariant::SizeType ArrayVariant::getSize() const ZAPI_DECL_NOEXCEPT
{
   // @TODO here we just use zend_hash_num_elements or zend_array_count
   return zend_hash_num_elements(getZendArrayPtr());
}

ArrayVariant::SizeType ArrayVariant::count() const ZAPI_DECL_NOEXCEPT
{
   return getSize();
}

ArrayVariant::SizeType ArrayVariant::getCapacity() const ZAPI_DECL_NOEXCEPT
{
   return getZendArrayPtr()->nTableSize;
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
   return zend_hash_index_exists(getZendArrayPtr(), index) == 1;
}

bool ArrayVariant::contains(const std::string &key) const
{
   return zend_hash_str_exists(getZendArrayPtr(), key.c_str(), key.length()) == 1;
}

zapi_long ArrayVariant::getNextInsertIndex() const
{
   return getZendArrayPtr()->nNextFreeElement;
}

std::list<ArrayVariant::KeyType> ArrayVariant::getKeys() const
{
   std::list<KeyType> keys;
   zapi_ulong index;
   zend_string *key;
   zval *entry;
   if (0 == getSize()) {
      return keys;
   }
   ZEND_HASH_FOREACH_KEY_VAL_IND(getZendArrayPtr(), index, key, entry) {
      if (key) {
         keys.push_back(KeyType(-1, std::shared_ptr<std::string>(new std::string(ZSTR_VAL(key), ZSTR_LEN(key)))));
      } else {
         keys.push_back(KeyType(index, nullptr));
      }
   } ZEND_HASH_FOREACH_END();
   return keys;
}

std::list<ArrayVariant::KeyType> ArrayVariant::getKeys(const Variant &value, bool strict) const
{
   std::list<KeyType> keys;
   zapi_ulong index;
   zend_string *key;
   zval *entry;
   zval *searchValue = const_cast<zval *>(value.getZvalPtr());
   if (0 == getSize()) {
      return keys;
   }
   if (strict) {
      ZEND_HASH_FOREACH_KEY_VAL_IND(getZendArrayPtr(), index, key, entry) {
         ZVAL_DEREF(entry);
         if (fast_is_identical_function(searchValue, entry)) {
            if (key) {
               keys.push_back(KeyType(-1, std::shared_ptr<std::string>(new std::string(ZSTR_VAL(key), ZSTR_LEN(key)))));
            } else {
               keys.push_back(KeyType(index, nullptr));
            }
         }
      } ZEND_HASH_FOREACH_END();
   } else {
      ZEND_HASH_FOREACH_KEY_VAL_IND(getZendArrayPtr(), index, key, entry) {
         if (fast_equal_check_function(searchValue, entry)) {
            if (key) {
               keys.push_back(KeyType(-1, std::shared_ptr<std::string>(new std::string(ZSTR_VAL(key), ZSTR_LEN(key)))));
            } else {
               keys.push_back(KeyType(index, nullptr));
            }
         }
      } ZEND_HASH_FOREACH_END();
   }
   return keys;
}

std::list<Variant> ArrayVariant::getValues() const
{
   std::list<Variant> values;
   zval *entry;
   if (0 == getSize()) {
      return values;
   }
   ZEND_HASH_FOREACH_VAL(getZendArrayPtr(), entry) {
      if (UNEXPECTED(Z_ISREF_P(entry) && Z_REFCOUNT_P(entry) == 1)) {
         entry = Z_REFVAL_P(entry);
      }
      values.emplace_back(entry);
   } ZEND_HASH_FOREACH_END();
   return values;
}

ArrayIterator ArrayVariant::find(zapi_ulong index)
{
   return static_cast<ArrayIterator>(static_cast<const ArrayVariant>(*this).find(index));
}

ArrayIterator ArrayVariant::find(const std::string &key)
{
   return static_cast<ArrayIterator>(static_cast<const ArrayVariant>(*this).find(key));
}

ConstArrayIterator ArrayVariant::find(zapi_ulong index) const
{
   zend_array *array = getZendArrayPtr();
   Bucket *p;
   IS_CONSISTENT(array);
   uint32_t idx = HT_INVALID_IDX;
   if (array->u.flags & HASH_FLAG_PACKED) {
      if (index < array->nNumUsed) {
         p = array->arData + index;
         if (Z_TYPE(p->val) != IS_UNDEF) {
            idx = index;
         }
      }
   }
   if (idx == HT_INVALID_IDX) {
      idx = findArrayIdx(index);
   }
   if (idx == HT_INVALID_IDX) {
      return end();
   }
   return ConstArrayIterator(array, &idx);
}

ConstArrayIterator ArrayVariant::find(const std::string &key) const
{
   zend_array *array = getZendArrayPtr();
   IS_CONSISTENT(array);
   uint32_t idx = findArrayIdx(key);
   if (idx == HT_INVALID_IDX) {
      return end();
   }
   return ConstArrayIterator(array, &idx);
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

uint32_t ArrayVariant::calculateIdxFromZval(zval *val) const ZAPI_DECL_NOEXCEPT
{
   zend_array *arr = getZendArrayPtr();
   return (reinterpret_cast<char *>(val) - reinterpret_cast<char *>(arr->arData)) / sizeof(Bucket);
}

uint32_t ArrayVariant::findArrayIdx(const std::string &key) const ZAPI_DECL_NOEXCEPT
{
   const char *keyStr = key.c_str();
   size_t length = key.length();
   zapi_ulong h = zend_inline_hash_func(key.c_str(), key.length());
   uint32_t nIndex;
   uint32_t idx;
   zend_array *ht = getZendArrayPtr();
   Bucket *p, *arData;
   arData = ht->arData;
   nIndex = h | ht->nTableMask;
   idx = HT_HASH_EX(arData, nIndex);
   while (idx != HT_INVALID_IDX) {
      ZEND_ASSERT(idx < HT_IDX_TO_HASH(ht->nTableSize));
      p = HT_HASH_TO_BUCKET_EX(arData, idx);
      if ((p->h == h)
          && p->key
          && (ZSTR_LEN(p->key) == length)
          && !memcmp(ZSTR_VAL(p->key), keyStr, length)) {
         return idx;
      }
      idx = Z_NEXT(p->val);
   }
   return HT_INVALID_IDX;
}

uint32_t ArrayVariant::findArrayIdx(zapi_ulong index) const ZAPI_DECL_NOEXCEPT
{
   uint32_t nIndex;
   uint32_t idx;
   Bucket *p, *arData;
   zend_array *ht = getZendArrayPtr();
   arData = ht->arData;
   nIndex = index | ht->nTableMask;
   idx = HT_HASH_EX(arData, nIndex);
   while (idx != HT_INVALID_IDX) {
      ZEND_ASSERT(idx < HT_IDX_TO_HASH(ht->nTableSize));
      p = HT_HASH_TO_BUCKET_EX(arData, idx);
      if (p->h == index && !p->key) {
         return idx;
      }
      idx = Z_NEXT(p->val);
   }
   return HT_INVALID_IDX;
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
   other.m_isEnd = true;
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
   zend_string *keyStr;
   zapi_ulong index;
   HashPosition pos = getCurrentPos();
   int keyType = zend_hash_get_current_key_ex(m_array, &keyStr, &index, &pos);
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "ArrayVariant::Iterator::getKey", "Key can't not exist");
   if (HASH_KEY_IS_STRING == keyType) {
      return KeyType(-1, std::shared_ptr<std::string>(new std::string(ZSTR_VAL(keyStr), ZSTR_LEN(keyStr))));
   } else {
      return KeyType(index, nullptr);
   }
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