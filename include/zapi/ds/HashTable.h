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
// Created by softboy on 5/26/17.

#ifndef ZAPI_DS_HASHTABLE_H
#define ZAPI_DS_HASHTABLE_H

#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/ds/String.h"

#include "php/Zend/zend_hash.h"
#include <string>

namespace zapi
{
namespace ds
{

enum class HashKeyType : unsigned char
{
   String      = HASH_KEY_IS_STRING,
   Long        = HASH_KEY_IS_LONG,
   NotExistent = HASH_KEY_NON_EXISTENT
};

enum class HashActionType : unsigned char
{
   Update         = HASH_UPDATE,
   Add            = HASH_ADD,
   UpdateIndirect = HASH_UPDATE_INDIRECT,
   AddNew         = HASH_ADD_NEW,
   AddNext        = HASH_ADD_NEXT
};

enum class HashFlagType : unsigned char
{
   Persisent        = HASH_FLAG_PERSISTENT,
   ApplyProtection  = HASH_FLAG_APPLY_PROTECTION,
   Packed           = HASH_FLAG_PACKED,
   Initialized      = HASH_FLAG_INITIALIZED,
   StaticKeys       = HASH_FLAG_STATIC_KEYS,
   HasEmptyIndirect = HASH_FLAG_HAS_EMPTY_IND
};

using zapi::lang::Variant;

extern zapi::lang::HashTableDataDeleter zValDataDeleter;

class ZAPI_DECL_EXPORT HashTable
{
public:
   constexpr static uint32_t DEFAULT_HASH_SIZE = 8;
   using IndexType = zend_ulong;
private:
   ::HashTable m_hashTable;
public:
   HashTable(uint32_t tableSize = DEFAULT_HASH_SIZE,
             zapi::lang::HashTableDataDeleter defaultDeleter = zValDataDeleter, 
             bool persistent = false)
   {
      zend_hash_init(&m_hashTable, tableSize, nullptr, defaultDeleter, persistent ? 1 : 0);
   }
   
   HashTable(uint32_t tableSize, bool persistent = false)
      : HashTable(tableSize, nullptr, persistent)
   {}
   
   HashTable(const ::HashTable &hashTable)
   {
      m_hashTable = hashTable;
      GC_REFCOUNT(&m_hashTable)++;
   }
   
   ~HashTable()
   {
      zend_hash_destroy(&m_hashTable);
   }

//   HashTable(HashTable *hashTable);
   
   uint32_t getSize() const
   {
      return zend_hash_num_elements(&m_hashTable);
   }
   
//   void find();
   
   bool isEmpty() const
   {
      return 0 != getSize();
   }
   
   HashTable &insert(const char *key, const Variant &value, bool forceNew = false)
   {
      return insert(String(key), value, forceNew);
   }

   HashTable &insert(const std::string &key, const Variant &value, bool forceNew = false)
   {
      return insert(String(key.c_str(), key.size()), value, forceNew);
   }

   HashTable &insert(const String &key, const Variant &value, bool forceNew = false)
   {
      if (forceNew) {
         zend_hash_add_new(&m_hashTable, key, value);
      } else {
         zend_hash_add(&m_hashTable, key, value);
      }
      return *this;
   }
   
//   void getKey();
   const Variant getValue(const char *key) const;
   const Variant getValue(const std::string &key) const;
   const Variant getValue(const String &key) const;
   const Variant getValue(IndexType key);
   
   class Iterator 
   {
      
   };
   
   class ConstInterator
   {
      
   };
};

} // ds
} // zapi

#endif //ZAPI_DS_HASHTABLE_H
