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

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/ArrayVariant.h"
#include "zapi/ds/ArrayItemProxy.h"
#include "zapi/ds/NumericVariant.h"
#include "zapi/ds/DoubleVariant.h"
#include "zapi/ds/StringVariant.h"
#include "zapi/ds/BoolVariant.h"

using zapi::ds::ArrayVariant;
using zapi::ds::Variant;
using zapi::ds::NumericVariant;
using zapi::ds::StringVariant;
using zapi::ds::BoolVariant;
using zapi::ds::DoubleVariant;

TEST(ArrayVariantTest, testConstructor)
{
   ArrayVariant array;
   ASSERT_TRUE(array.isNull());
   ASSERT_TRUE(array.isEmpty());
   ASSERT_TRUE(array.isArray());
   
}

TEST(ArrayVariantTest, testAppend)
{
   ArrayVariant array;
   ASSERT_TRUE(array.isNull());
   ASSERT_TRUE(array.isEmpty());
   array.append(1);
   ASSERT_FALSE(array.isNull());
   ASSERT_FALSE(array.isEmpty());
   ASSERT_EQ(array.getSize(), 1);
   array.append("zapi");
   ASSERT_EQ(array.getSize(), 2);
   NumericVariant num = array.getValue(0);
   StringVariant str = array.getValue(1);
   ASSERT_EQ(num.toLong(), 1);
   ASSERT_STREQ(str.getCStr(), "zapi");
   ASSERT_EQ(str.getRefCount(), 2);
   // std::cout << str << std::endl;
}

TEST(ArrayVariantTest, testInsert)
{
   ArrayVariant array;
   ASSERT_TRUE(array.isNull());
   ASSERT_TRUE(array.isEmpty());
   array.insert(1, "zapi");
   array.insert(5, true);
   BoolVariant boolVar = array.getValue(5);
   StringVariant strVar = array.getValue(1);
   ASSERT_EQ(array.getSize(), 2);
   ASSERT_EQ(boolVar.toBool(), true);
   ASSERT_STREQ(strVar.getCStr(), "zapi");
   ASSERT_EQ(strVar.getRefCount(), 2);
   array.insert(1, "zzu_softboy");
   ASSERT_EQ(strVar.getRefCount(), 1);
   //   array.getValue(111); echo notice msg
   //   array.getValue("name"); echo notice msg
   ArrayVariant::Iterator iter =  array.insert("name", "zzu_softboy");
   array.insert("age", 123);
   StringVariant name = array.getValue("name");
   ASSERT_EQ(array.getSize(), 4);
   ASSERT_STREQ(name.getCStr(), "zzu_softboy");
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zzu_softboy");
   ASSERT_STREQ(StringVariant(iter.getValue()).getCStr(), "zzu_softboy");
   array.insert(0, "xiuxiu");
   iter = array.begin();
   // array keep insert order
   ASSERT_STREQ(StringVariant(iter.getZvalPtr()).getCStr(), "zzu_softboy");
}

TEST(ArrayVariantTest, testIterators)
{
   ArrayVariant array;
   array.append(1);
   array.append("zapi");
   array.append("zzu_softboy");
   array.append("aaa");
   array.append("bbb");
   array.append("ccc");
   ArrayVariant::Iterator iter = array.begin();
   ArrayVariant::ConstIterator citer = array.cbegin();
   zval &item1 = iter.getZval();
   ASSERT_EQ(Z_LVAL(item1), 1);
   const zval &citem1 = citer.getZval();
   ASSERT_EQ(Z_LVAL(item1), 1);
   // Z_LVAL(citem1) = 123; compile error
   Z_LVAL(item1) = 123;
   ASSERT_EQ(Z_LVAL(citem1), 123);
   // iterator ++ operators
   iter++;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter += 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "aaa");
   iter += -2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter += 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "aaa");
   iter -= 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter++;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zzu_softboy");
   iter--;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter--;
   
   ++iter;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter += 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "aaa");
   iter += -2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   iter += 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "aaa");
   iter -= 2;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   ++iter;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zzu_softboy");
   --iter;
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   
   ArrayVariant::Iterator preIter = iter++;
   ASSERT_STREQ(Z_STRVAL_P(preIter.getZvalPtr()), "zapi");
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zzu_softboy");
   
   preIter = ++iter;
   ASSERT_STREQ(Z_STRVAL_P(preIter.getZvalPtr()), "aaa");
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "aaa");
   
   preIter = iter--;
   ASSERT_STREQ(Z_STRVAL_P(preIter.getZvalPtr()), "aaa");
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zzu_softboy");
   
   preIter = --iter;
   ASSERT_STREQ(Z_STRVAL_P(preIter.getZvalPtr()), "zapi");
   ASSERT_STREQ(Z_STRVAL_P(iter.getZvalPtr()), "zapi");
   
   // const
   citer++;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer += 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "aaa");
   citer += -2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer += 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "aaa");
   citer -= 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer++;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zzu_softboy");
   citer--;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer--;
   
   ++citer;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer += 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "aaa");
   citer += -2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   citer += 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "aaa");
   citer -= 2;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   ++citer;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zzu_softboy");
   --citer;
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
   
   ArrayVariant::ConstIterator preciter = citer++;
   ASSERT_STREQ(Z_STRVAL_P(preciter.getZvalPtr()), "zapi");
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zzu_softboy");
   
   preciter = ++citer;
   ASSERT_STREQ(Z_STRVAL_P(preciter.getZvalPtr()), "aaa");
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "aaa");
   
   preciter = citer--;
   ASSERT_STREQ(Z_STRVAL_P(preciter.getZvalPtr()), "aaa");
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zzu_softboy");
   
   preciter = --citer;
   ASSERT_STREQ(Z_STRVAL_P(preciter.getZvalPtr()), "zapi");
   ASSERT_STREQ(Z_STRVAL_P(citer.getZvalPtr()), "zapi");
}

TEST(ArrayVariantTest, testAccessOperator)
{
   ArrayVariant array;
   array.append(1);
   array.append("zapi");
   //   array.append("zzu_softboy");
   //   array.append("aaa");
   //   array.append("bbb");
   //   array.append("ccc");
   //   StringVariant str = array[1];
   //   ASSERT_STREQ(str.getCStr(), "zapi");
   //   array[0] = 123;
   //   NumericVariant num = array.getValue(0);
   //   ASSERT_EQ(num.toLong(), 123);
   //   array[10] = "unicornteam";
   //   StringVariant team = array[10];
   //   ASSERT_STREQ(team.getCStr(), "unicornteam");
   //   array["city"] = "beijing";
   //   StringVariant city = array["city"];
   //   ASSERT_STREQ(city.getCStr(), "beijing");
   array[11][1][3] = "zzu_softboy";
   StringVariant str = array[11][1][3];
   std::cout << str << std::endl;
   ASSERT_STREQ(str.getCStr(), "zzu_softboy");
   array[4][5][6][7][8][9][10][11][12]["name"] = "UnicornTeam";
   array[4][5][6][7][8][9][10][11][12]["address"] = "Qihoo360 Building";
   array[4][5][6][7][8][9][10][11][12]["offical_site"] = "http://unicorn.360.com";
   array[4][5][6][7][8][9][10][11][12][111] = 213;
   array[4][5][6][7][8][9][10][11][12][112] = 3.1415926;
   NumericVariant num = array[4][5][6][7][8][9][10][11][12][111];
   DoubleVariant doubleNum = array[4][5][6][7][8][9][10][11][12][112];
   StringVariant unicornTeamName = array[4][5][6][7][8][9][10][11][12]["name"];
   StringVariant unicornTeamAddress = array[4][5][6][7][8][9][10][11][12]["address"];
   StringVariant unicornTeamOfficalSite = array[4][5][6][7][8][9][10][11][12]["offical_site"];
   ASSERT_STREQ(unicornTeamName.getCStr(), "UnicornTeam");
   ASSERT_STREQ(unicornTeamAddress.getCStr(), "Qihoo360 Building");
   ASSERT_STREQ(unicornTeamOfficalSite.getCStr(), "http://unicorn.360.com");
   ASSERT_EQ(num.toLong(), 213);
   ASSERT_EQ(doubleNum.toDouble(), 3.1415926);
//   std::cout << unicornTeamName << std::endl;
//   std::cout << unicornTeamAddress << std::endl;
//   std::cout << unicornTeamOfficalSite << std::endl;
//   std::cout << num << std::endl;
//   std::cout << doubleNum << std::endl;
}
