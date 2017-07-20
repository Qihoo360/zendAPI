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

//TEST_F(HashTableTest, testConstructors) 
//{
//   {
//      // default constructor
//      ZapiHashTable table;
//      ASSERT_EQ(table.getSize(), 0);
//   }
//}

//TEST_F(HashTableTest, testInsertItem)
//{
//   {
//      // default constructor
//      ZapiHashTable table;
//      ASSERT_EQ(table.getSize(), 0);
//      table.insert("name", Variant("zapi"));
//      ASSERT_EQ(table.getSize(), 1);
//      table.insert("age", Variant(20));
//      ASSERT_EQ(table.getSize(), 2);
//   }
//}

//TEST_F(HashTableTest, testIterator)
//{
//   {
//      // default constructor
//      ZapiHashTable table;
//      ASSERT_EQ(table.getSize(), 0);
//      table.insert("name", Variant("zapi"));
//      ASSERT_EQ(table.getSize(), 1);
//      table.insert("age", Variant(20));
//      ASSERT_EQ(table.getSize(), 2);
//      table.insert("height", Variant(123));
//      ASSERT_EQ(table.getSize(), 3);
//      ZapiHashTable::iterator iter = table.begin();
//      std::vector<std::string> expectedKeys{"name", "age", "height"};
//      std::vector<std::string> actualKeys;
//      std::vector<std::string> expectedValueStrs{"zapi"};
//      std::vector<int64_t> expectedValueInts{20, 123};
//      std::vector<std::string> actualValueStrs;
//      std::vector<int64_t> actualValueInts;
//      while (iter != table.end()) {
//         ZapiHashTable::HashKeyType keyType = iter.getKeyType();
//         if (keyType == ZapiHashTable::HashKeyType::String) {
//            actualKeys.push_back(iter.getStrKey());
//         }
//         Variant value = *iter;
//         if (value.getType() == zapi::lang::Type::String) {
//            actualValueStrs.push_back(value.toString());
//         } else if (value.getType() == zapi::lang::Type::Long) {
//            actualValueInts.push_back(value.toLong());
//         }
//         iter++;
//      }
//      ASSERT_EQ(expectedKeys, actualKeys);
//      ASSERT_EQ(expectedValueStrs, actualValueStrs);
//      ASSERT_EQ(expectedValueInts, actualValueInts);
//   }
//}

TEST_F(HashTableTest, testGetValue)
{
   {
      ZapiHashTable table;
      //      table.insert("name", Variant("zapi"));
      //      table.insert("city", Variant(123));
      //      table.insert("xiuxiu", Variant("beijing"));
      //ASSERT_EQ(table.getSize(), 2);
      //      ASSERT_EQ(table.getValue("name").toString(), "zapi");
      //      ASSERT_EQ(table.getValue("city").toString(), "beijing");
      //      std::cout << table.getValue("name").toString();
      //      std::cout << table.getValue("city").toLong();
      //      std::cout << table.getValue("xiuxiu").toLong();
      std::cout <<   Variant("beijing");
      //      std::cout <<   Variant("zapi");
      //      std::cout <<   Variant("xiuxiux");
      Variant var1 = Variant("zapi");
      Variant var2 = Variant("beijing");
      Variant var3 = Variant("xiuxiux");
      const zval v0 = var1.getZval();
      const zval v1 = var2.getZval();
      const zval v2 = var3.getZval();
      zval *v4 = (zval *)var1;
      zval *v5 = (zval *)var2;
      zval *v6 = (zval *)var3;
      zval *v7 = (zval *)var3;
   }
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
