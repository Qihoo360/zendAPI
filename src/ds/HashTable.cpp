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
// Created by zzu_softboy on 15/06/2017.

#include "zapi/ds/HashTable.h"

namespace zapi
{
namespace ds
{

zapi::lang::HashTableDataDeleter zValDataDeleter = ZVAL_PTR_DTOR;

HashTable &HashTable::insert(const String &key, const Variant &value, bool forceNew)
{
   if (forceNew) {
      zend_hash_add_new(&m_hashTable, key, value);
   } else {
      zend_hash_add(&m_hashTable, key, value);
   }
   return *this;
}

HashTable &HashTable::insert(zapi_ulong index, const Variant &value, bool forceNew)
{
   if (forceNew) {
      zend_hash_index_add_new(&m_hashTable, index, value);
   } else {
      zend_hash_index_add(&m_hashTable, index, value);
   }
   return *this;
}

HashTable &HashTable::append(const Variant &value, bool forceNew)
{
   if (forceNew) {
      zend_hash_next_index_insert_new(&m_hashTable, value);
   } else {
      zend_hash_next_index_insert(&m_hashTable, value);
   }
   return *this;
}

HashTable::~HashTable()
{
   zend_hash_destroy(&m_hashTable);
}

Variant HashTable::operator[](const String &key)
{
   zval *value = zend_hash_find(&m_hashTable, key);
   if (nullptr == value) {
      value = zend_hash_add_empty_element(&m_hashTable, key);
   }
   return Variant(value, true);
}

Variant HashTable::operator[](zapi_ulong index)
{
   zval *value = zend_hash_index_find(&m_hashTable, index);
   if (nullptr == value) {
      value = zend_hash_index_add_empty_element(&m_hashTable, index);
   }
   return Variant(value, true);
}

Variant HashTable::iterator::getKey() const
{
   zend_string *key;
   IndexType index;
   int keyType = zend_hash_get_current_key_ex(const_cast<::HashTable *>(m_hashTable), 
                                              &key, 
                                              &index, 
                                              const_cast<::HashPosition *>(&m_index));
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
   // we just copy here
   if (keyType == HASH_KEY_IS_STRING) {
      return Variant(key->val, key->len);
   } else {
      return Variant(static_cast<zapi_long>(index));
   }
}

std::string HashTable::iterator::getStrKey() const
{
   zend_string *key;
   int keyType = zend_hash_get_current_key_ex(const_cast<::HashTable *>(m_hashTable), 
                                              &key, 
                                              nullptr,
                                              const_cast<::HashPosition *>(&m_index));
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
   // we just copy here
   return std::string(key->val, key->len);
}

HashTable::IndexType HashTable::iterator::getNumericKey() const
{
   IndexType index;
   int keyType = zend_hash_get_current_key_ex(const_cast<::HashTable *>(m_hashTable), 
                                              nullptr, 
                                              &index, 
                                              const_cast<::HashPosition *>(&m_index));
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
   return index;
}

Variant HashTable::getValue(zapi_ulong index, const Variant &defaultValue) const
{
   zval *result = zend_hash_index_find(&m_hashTable, index);
   if (nullptr == defaultValue) {
      return defaultValue;
   }
   return result;
}

Variant HashTable::getValue(const String &key, const Variant &defaultValue) const
{
   zval *result = zend_hash_find(&m_hashTable, key);
   if (nullptr == result) {
      return defaultValue;
   }
   return result;
}

Variant HashTable::getKey() const
{
   zend_string *key;
   zend_ulong index;
   int keyType = zend_hash_get_current_key(const_cast<::HashTable *>(&m_hashTable), &key, &index);
   ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
   return keyType == HASH_KEY_IS_STRING ? Variant(key->val, key->len) : Variant(static_cast<zapi_long>(index));
}

Variant HashTable::getKey(const Variant &value, const Variant &defaultKey) const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   zval *targetValue;
   ZEND_HASH_FOREACH_KEY_VAL(&m_hashTable, index, key, targetValue)
   if (value == targetValue) {
      if (key != nullptr) {
         return Variant(key->val, key->len);
      } else {
         // maybe overflow
         return Variant(static_cast<zapi_long>(index));
      }
   }
   ZEND_HASH_FOREACH_END();
   return defaultKey;
}

Variant HashTable::getKey(const Variant &value) const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   zval *targetValue;
   ZEND_HASH_FOREACH_KEY_VAL(&m_hashTable, index, key, targetValue)
   if (value == targetValue) {
      if (key != nullptr) {
         return Variant(key->val, key->len);
      } else {
         // maybe overflow
         return Variant(static_cast<zapi_long>(index));
      }
   }
   ZEND_HASH_FOREACH_END();
   return Variant(nullptr);
}

HashTable::iterator &HashTable::iterator::operator++()
{
   ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
   int result = zend_hash_move_forward_ex(m_hashTable, &m_index);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating beyond end()");
   return *this;
}

HashTable::iterator HashTable::iterator::operator++(int)
{
   iterator iter = *this;
   ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
   int result = zend_hash_move_forward_ex(m_hashTable, &m_index);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating beyond end()");
   return iter;
}

HashTable::iterator &HashTable::iterator::operator--()
{
   ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
   int result = zend_hash_move_backwards_ex(m_hashTable, &m_index);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating backward beyond begin()");
   return *this;
}

HashTable::iterator HashTable::iterator::operator--(int)
{
   iterator iter = *this;
   ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
   int result = zend_hash_move_backwards_ex(m_hashTable, &m_index);
   ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating backward beyond begin()");
   return iter;
}

HashTable::iterator HashTable::iterator::operator+(int32_t step)
{
   iterator iter = *this;
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

void HashTable::each(DefaultForeachVisitor visitor) const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   zval *value;
   ZEND_HASH_FOREACH_KEY_VAL(&m_hashTable, index, key, value)
   if (key != nullptr) {
      visitor(Variant(key->val, key->len), Variant(value));
   } else {
      // maybe overflow
      visitor(Variant(static_cast<zapi_long>(index)), Variant(value));
   }
   ZEND_HASH_FOREACH_END();
}

void HashTable::reverseEach(DefaultForeachVisitor visitor) const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   zval *value;
   ZEND_HASH_REVERSE_FOREACH_KEY_VAL(&m_hashTable, index, key, value)
   if (key != nullptr) {
      visitor(Variant(key->val, key->len), Variant(value));
   } else {
      // maybe overflow
      visitor(Variant(static_cast<zapi_long>(index)), Variant(value));
   }
   ZEND_HASH_FOREACH_END();
}

std::vector<Variant> HashTable::getKeys() const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   std::vector<Variant> keys;
   ZEND_HASH_FOREACH_KEY(&m_hashTable, index, key)
   if (key != nullptr) {
      keys.push_back(Variant(key->val, key->len));
   } else {
      // maybe overflow
      keys.push_back(Variant(static_cast<zapi_long>(index)));
   }
   ZEND_HASH_FOREACH_END();
   return keys;
}

std::vector<Variant> HashTable::getKeys(const Variant &value) const
{
   zend_string *key = nullptr;
   zapi_ulong index = 0;
   zval *curValue;
   std::vector<Variant> keys;
   ZEND_HASH_REVERSE_FOREACH_KEY_VAL(&m_hashTable, index, key, curValue)
   if (value == curValue) {
      if (key != nullptr) {
         keys.push_back(Variant(key->val, key->len));
      } else {
         // maybe overflow
         keys.push_back(Variant(static_cast<zapi_long>(index)));
      }
   }
   ZEND_HASH_FOREACH_END();
   return keys;
}

std::vector<Variant> HashTable::getValues() const
{
   zval *value;
   std::vector<Variant> values;
   ZEND_HASH_FOREACH_VAL(&m_hashTable, value)
   values.push_back(value);
   ZEND_HASH_FOREACH_END();
   return values;
}

HashTable::iterator::~iterator()
{}

} // ds
} // zapi