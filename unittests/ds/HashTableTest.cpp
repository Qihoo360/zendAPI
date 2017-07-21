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
// Created by zzu_softboy on 14/06/2017.

#include "php/sapi/embed/php_embed.h"

#include "gtest/gtest.h"
#include "zapi/Global.h"
#include <limits>
#include "zapi/ds/HashTable.h"
#include "zapi/lang/Variant.h"
#include <iostream>
#include <vector>
#include <string>

using ZapiHashTable = zapi::ds::HashTable;
using zapi::lang::Variant;

namespace
{

class HashTableTest : public ::testing::Test
{};

} // end of namespace

TEST_F(HashTableTest, testConstructors) 
{
   {
      // default constructor
      ZapiHashTable table;
      ASSERT_EQ(table.getSize(), 0);
   }
}

TEST_F(HashTableTest, testInsertItem)
{
   {
      // default constructor
      ZapiHashTable table;
      ASSERT_EQ(table.getSize(), 0);
      table.insert("name", Variant("zapi"));
      ASSERT_EQ(table.getSize(), 1);
      table.insert("age", Variant(20));
      ASSERT_EQ(table.getSize(), 2);
   }
}

TEST_F(HashTableTest, testIterator)
{
   {
      // default constructor
      ZapiHashTable table;
      ASSERT_EQ(table.getSize(), 0);
      table.insert("name", Variant("zapi"));
      ASSERT_EQ(table.getSize(), 1);
      table.insert("age", Variant(20));
      ASSERT_EQ(table.getSize(), 2);
      table.insert("height", Variant(123));
      ASSERT_EQ(table.getSize(), 3);
      ZapiHashTable::iterator iter = table.begin();
      std::vector<std::string> expectedKeys{"name", "age", "height"};
      std::vector<std::string> actualKeys;
      std::vector<std::string> expectedValueStrs{"zapi"};
      std::vector<int64_t> expectedValueInts{20, 123};
      std::vector<std::string> actualValueStrs;
      std::vector<int64_t> actualValueInts;
      while (iter != table.end()) {
         ZapiHashTable::HashKeyType keyType = iter.getKeyType();
         if (keyType == ZapiHashTable::HashKeyType::String) {
            actualKeys.push_back(iter.getStrKey());
         }
         Variant value = *iter;
         if (value.getType() == zapi::lang::Type::String) {
            actualValueStrs.push_back(value.toString());
         } else if (value.getType() == zapi::lang::Type::Long) {
            actualValueInts.push_back(value.toLong());
         }
         iter++;
      }
      ASSERT_EQ(expectedKeys, actualKeys);
      ASSERT_EQ(expectedValueStrs, actualValueStrs);
      ASSERT_EQ(expectedValueInts, actualValueInts);
   }
}

TEST_F(HashTableTest, testGetValue)
{
   {
      ZapiHashTable table;
      table.insert("name", Variant("zapi"));
      table.insert("city", Variant("beijing"));
      table.insert("height", Variant(123));
      ASSERT_EQ(table.getSize(), 3);
      ASSERT_EQ(table.getValue("name").toString(), "zapi");
      ASSERT_EQ(table.getValue("city").toString(), "beijing");
      ASSERT_EQ(table.getValue("height").toLong(), 123);
      ASSERT_EQ(table["name"].toString(), "zapi");
      ASSERT_EQ(table["city"].toString(), "beijing");
      ASSERT_EQ(table["height"].toLong(), 123);
   }
}

TEST_F(HashTableTest, testAssignValue)
{
   ZapiHashTable table;
   table.insert("num", Variant(123));
   table["num"] = Variant(213);
   ASSERT_EQ(table["num"].toLong(), 213);
   table["num"] = Variant("zapi");
   ASSERT_EQ(table["num"].toString(), "zapi");
   table["name"] = Variant("zzu_softboy");
   ASSERT_EQ(table["name"].toString(), "zzu_softboy");
   table["age"] = Variant(123);
   ASSERT_EQ(table["age"].toLong(), 123);
   table.append(Variant(1234));
   ASSERT_EQ(table[0].toLong(), 1234);
   table.append(Variant(4321));
   ASSERT_EQ(table[1].toLong(), 4321);
   table.append(Variant("zapi"));
   ASSERT_EQ(table[2].toString(), "zapi");
}

TEST_F(HashTableTest, testDeleteItem)
{
   ZapiHashTable table;
   table.insert("item1", Variant(123));
   table.insert("item2", Variant("softboy"));
   table.insert("item3", Variant(true));
   ASSERT_EQ(table.getSize(), 3);
   ASSERT_FALSE(table.remove("notExist"));
   ASSERT_TRUE(table.remove("item1"));
   ASSERT_EQ(table.getSize(), 2);
   ASSERT_TRUE(table.remove("item2"));
   ASSERT_TRUE(table.remove("item3"));
   ASSERT_EQ(table.getSize(), 0);
   table.insert(0, Variant(true));
   table.insert(1, Variant(false));
   ASSERT_EQ(table.getSize(), 2);
   ASSERT_FALSE(table.remove(3));
   ASSERT_TRUE(table.remove(1));
   ASSERT_TRUE(table.remove(0));
   ASSERT_EQ(table.getSize(), 0);
}

TEST_F(HashTableTest, testContains) 
{
   ZapiHashTable table;
   ASSERT_FALSE(table.contains(1));
   table.insert(0, Variant("zapi"));
   ASSERT_FALSE(table.contains(1));
   table.insert(1, Variant("zapi"));
   ASSERT_TRUE(table.contains(1));
   ASSERT_FALSE(table.contains("name"));
   table.insert("name", Variant("zapi"));
   ASSERT_TRUE(table.contains("name"));
}

int main(int argc, char **argv)
{
   int retCode = 0;
   PHP_EMBED_START_BLOCK(argc,argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}
