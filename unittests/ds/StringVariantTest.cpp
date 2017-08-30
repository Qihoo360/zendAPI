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
// Created by softboy on 2017/08/08.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/StringVariant.h"
#include <iostream>
#include <string>
#include <vector>

using zapi::ds::StringVariant;
using zapi::ds::Variant;

TEST(StringVariantTest, testConstructors)
{
   StringVariant str("xiuxiu");
   StringVariant emptyStr;
   ASSERT_TRUE(emptyStr.isEmpty());
   ASSERT_EQ(emptyStr.getCapacity(), 0);
   ASSERT_EQ(emptyStr.getSize(), 0);
   emptyStr = 1;
   //std::cout << emptyStr << std::endl;
   emptyStr.append('C');
   ASSERT_EQ(emptyStr.getSize(), 2);
   ASSERT_EQ(emptyStr.getCapacity(), 199);
   ASSERT_EQ(emptyStr.at(0), '1');
   emptyStr.clear();
   ASSERT_EQ(emptyStr.getSize(), 0);
   ASSERT_EQ(emptyStr.getCapacity(), 0);
   emptyStr = str;
   ASSERT_EQ(emptyStr.getSize(), 6);
   ASSERT_EQ(emptyStr.getCapacity(), 199);
   ASSERT_EQ(emptyStr.getRefCount(), 2);
   ASSERT_EQ(str.getRefCount(), 2);
   emptyStr.clear();
   ASSERT_EQ(emptyStr.getSize(), 0);
   ASSERT_EQ(emptyStr.getCapacity(), 0);
   emptyStr = Variant("zapi");
   ASSERT_EQ(emptyStr.getSize(), 4);
   ASSERT_EQ(emptyStr.getCapacity(), 32);
   ASSERT_EQ(emptyStr.getRefCount(), 1);
   emptyStr.clear();
   Variant gvar("zapi");
   emptyStr = gvar;
   ASSERT_EQ(emptyStr.getSize(), 4);
   ASSERT_EQ(emptyStr.getCapacity(), 32);
   ASSERT_EQ(emptyStr.getRefCount(), 2);
}

TEST(StringVariantTest, testRefConstruct)
{
   zval rawStrVar;
   ZVAL_STRING(&rawStrVar, "zapi");
   ASSERT_STREQ(Z_STRVAL_P(&rawStrVar), "zapi");
   StringVariant strVariant(rawStrVar, false);
   ASSERT_EQ(strVariant.getLength(), 4);
   ASSERT_EQ(strVariant.getRefCount(), 1);
   StringVariant refStrVariant(rawStrVar, true);
   zval *rval = &rawStrVar;
   ZVAL_DEREF(rval);
   ASSERT_TRUE(Z_TYPE_P(rval) == IS_STRING);
   ASSERT_TRUE(Z_TYPE_P(&rawStrVar) == IS_REFERENCE);
   ASSERT_EQ(refStrVariant.getRefCount(), 2);
   ASSERT_STREQ(refStrVariant.getCStr(), "zapi");
   ASSERT_EQ(refStrVariant.getCapacity(), 32);
   ASSERT_EQ(refStrVariant.getSize(), 4);
   zval_dtor(&rawStrVar);
}

TEST(StringVariantTest, testConstructFromVariant)
{
   Variant strVariant("zapi is the best!");
   Variant numericVariant(123);
   StringVariant str(strVariant);
   StringVariant str1(numericVariant);
   ASSERT_EQ(str.getRefCount(), 2);
   ASSERT_EQ(strVariant.getRefCount(), 2);
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(numericVariant.getRefCount(), 0);
}

TEST(StringVariantTest, testConstructFromStringVariant)
{
   StringVariant str1("hello zapi");
   StringVariant strCopy(str1);
   StringVariant strRef(str1, true);
   ASSERT_FALSE(strCopy.isReference());
   ASSERT_TRUE(strRef.isReference());
   str1.append(", beijing");
   ASSERT_STREQ(str1.getCStr(), "hello zapi, beijing");
   ASSERT_STREQ(strCopy.getCStr(), "hello zapi");
   ASSERT_STREQ(strRef.getCStr(), "hello zapi, beijing");
   strCopy.append('X');
   ASSERT_STREQ(strCopy.getCStr(), "hello zapiX");
   ASSERT_STREQ(str1.getCStr(), "hello zapi, beijing");
   ASSERT_STREQ(strRef.getCStr(), "hello zapi, beijing");
   strRef.append("BB");
   ASSERT_STREQ(strCopy.getCStr(), "hello zapiX");
   ASSERT_STREQ(str1.getCStr(), "hello zapi, beijingBB");
   ASSERT_STREQ(strRef.getCStr(), "hello zapi, beijingBB");
}

TEST(StringVariantTest, testMoveConstruct)
{
   StringVariant strVariant(Variant("zapi"));
   ASSERT_STREQ(strVariant.getCStr(), "zapi");
   Variant gvar("hello zzu_softboy");
   StringVariant str1(std::move(gvar));
   ASSERT_STREQ(str1.getCStr(), "hello zzu_softboy");
   StringVariant str2(std::move(str1));
   ASSERT_STREQ(str2.getCStr(), "hello zzu_softboy");
   StringVariant str3(str2, true);
   StringVariant str4(std::move(str3));
   ASSERT_STREQ(str4.getCStr(), "hello zzu_softboy");
   str4.append("XX");
   ASSERT_STREQ(str4.getCStr(), "hello zzu_softboyXX");
   ASSERT_STREQ(str2.getCStr(), "hello zzu_softboyXX");
   StringVariant str5 = str2.makeReference();
   ASSERT_STREQ(str5.getCStr(), "hello zzu_softboyXX");
   str5.append("CC");
   ASSERT_STREQ(str5.getCStr(), "hello zzu_softboyXXCC");
   ASSERT_STREQ(str2.getCStr(), "hello zzu_softboyXXCC");
}

TEST(StringVariantTest, testAssignOperators)
{
   StringVariant str1("zapi");
   // test same type
   ASSERT_EQ(str1.getRefCount(), 1);
   StringVariant str2 = str1;
   ASSERT_EQ(str1.getRefCount(), 2);
   ASSERT_EQ(str2.getRefCount(), 2);
   ASSERT_STREQ(str2.getCStr(), "zapi");
   str1.append('X');
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(str2.getRefCount(), 1);
   ASSERT_STREQ(str1.getCStr(), "zapiX");
   ASSERT_STREQ(str2.getCStr(), "zapi");
   str2 = str1;
   ASSERT_EQ(str1.getRefCount(), 2);
   ASSERT_EQ(str2.getRefCount(), 2);
   ASSERT_STREQ(str2.getCStr(), "zapiX");
   StringVariant str3("xxx"); 
   ASSERT_EQ(str3.getRefCount(), 1);
   str3 = str2;
   ASSERT_EQ(str1.getRefCount(), 3);
   ASSERT_EQ(str2.getRefCount(), 3);
   ASSERT_EQ(str3.getRefCount(), 3);
   str1.append("C");
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(str2.getRefCount(), 2);
   ASSERT_EQ(str3.getRefCount(), 2);
   str3 = str1;
   ASSERT_EQ(str1.getRefCount(), 2);
   ASSERT_EQ(str2.getRefCount(), 1);
   ASSERT_EQ(str3.getRefCount(), 2);
   // test Variant type
   Variant gvar("zzu_softboy");
   str1 = gvar;
   ASSERT_EQ(str1.getRefCount(), 2);
   ASSERT_EQ(gvar.getRefCount(), 2);
   ASSERT_EQ(str2.getRefCount(), 1);
   ASSERT_EQ(str3.getRefCount(), 1);
   ASSERT_STREQ(str1.getCStr(), "zzu_softboy");
   str1.append("XX");
   ASSERT_EQ(gvar.getRefCount(), 1);
   ASSERT_EQ(str1.getRefCount(), 1);
   Variant numVar(123);
   str1 = numVar;
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(numVar.getRefCount(), 0);
   str1 = Variant("312");
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_STREQ(str1.getCStr(), "312");
   str1 = std::move(numVar);
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_STREQ(str1.getCStr(), "123");
   str1 = StringVariant("zzu_softboy");
   ASSERT_STREQ(str1.getCStr(), "zzu_softboy");
   ASSERT_EQ(str1.getRefCount(), 1);
   str1 = 123456778;
   ASSERT_STREQ(str1.getCStr(), "123456778");
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(str1.getLength(), 9);
   str1 = std::string("zzu_softboy");
   ASSERT_STREQ(str1.getCStr(), "zzu_softboy");
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(str1.getLength(), 11);
   str1 = 'c';
   ASSERT_STREQ(str1.getCStr(), "c");
   ASSERT_EQ(str1.getRefCount(), 1);
   ASSERT_EQ(str1.getLength(), 1);
}

TEST(StringVariantTest, testplusAssignOperators)
{
   StringVariant str;
   ASSERT_EQ(str.getSize(), 0);
   str += "zapi";
   ASSERT_STREQ(str.getCStr(), "zapi");
   str += std::string("--");
   ASSERT_STREQ(str.getCStr(), "zapi--");
   str += StringVariant("php");
   ASSERT_STREQ(str.getCStr(), "zapi--php");
   char append[] = {'z', 'z', 'u'};
   str += append;
   ASSERT_STREQ(str.getCStr(), "zapi--phpzzu");
   std::cout << str << std::endl;
}

TEST(StringVariantTest, testEqOperators)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str == "zapi");
   ASSERT_FALSE(str == "zapi1");
   ASSERT_TRUE(str == std::string("zapi"));
   ASSERT_TRUE(str == StringVariant("zapi"));
   char buffer[] = "zapi";
   ASSERT_TRUE(str == buffer);
   char buffer1[] = {'z', 'a', 'p', 'i'};
   ASSERT_TRUE(str == buffer1);
   
   ASSERT_TRUE("zapi" == str);
   ASSERT_FALSE("zapi1" == str);
   ASSERT_TRUE(std::string("zapi") == str);
   ASSERT_TRUE(StringVariant("zapi") == str);
   char buffer2[] = "zapi";
   ASSERT_TRUE(buffer2 == str);
   char buffer3[] = {'z', 'a', 'p', 'i'};
   ASSERT_TRUE(buffer3 == str);
}

TEST(StringVariantTest, testNotEqOperators)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str != "php");
   ASSERT_FALSE(str != "zapi");
   ASSERT_TRUE(str != std::string("php"));
   ASSERT_TRUE(str != StringVariant("php"));
   char buffer[] = "php";
   ASSERT_TRUE(str != buffer);
   char buffer1[] = {'p', 'h', 'p'};
   ASSERT_TRUE(str != buffer1);
   
   ASSERT_TRUE("zapiphp" != str);
   ASSERT_FALSE("zapi" != str);
   ASSERT_TRUE(std::string("php") != str);
   ASSERT_TRUE(StringVariant("zapiphp") != str);
   ASSERT_TRUE(buffer != str);
   ASSERT_TRUE(buffer1 != str);
}

TEST(StringVariantTest, testLtOperator)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str < "zbpi");
   ASSERT_FALSE(str < "abc");
   ASSERT_TRUE(str < std::string("zbpi"));
   ASSERT_TRUE(str < StringVariant("zbpi"));
   char buffer[] = "zbpi";
   ASSERT_TRUE(str < buffer);
   char buffer1[] = {'z', 'b', 'p', 'i'};
   ASSERT_TRUE(str < buffer1);
   
   ASSERT_TRUE("zbpi" > str);
   ASSERT_TRUE("zapix" > str);
   ASSERT_FALSE("abc" > str);
   ASSERT_TRUE(std::string("zbpi") > str);
   ASSERT_TRUE(StringVariant("zbpi") > str);
   ASSERT_TRUE(buffer > str);
   ASSERT_TRUE(buffer1 > str);
}

TEST(StringVariantTest, testLtEqOperator)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str <= "zbpi");
   ASSERT_TRUE(str <= "zapi");
   ASSERT_TRUE(str <= std::string("zbpi"));
   ASSERT_TRUE(str <= std::string("zapi"));
   ASSERT_TRUE(str <= StringVariant("zbpi"));
   ASSERT_TRUE(str <= StringVariant("zapi"));
   char buffer[] = "zapi";
   ASSERT_FALSE(str <= "abc");
   ASSERT_TRUE(str <= buffer);
   ASSERT_TRUE("zapix" >= str);
   ASSERT_TRUE("zbpi" >= str);
   ASSERT_FALSE("abc" >= str);
   ASSERT_TRUE(std::string("zbpi") >= str);
   ASSERT_TRUE(StringVariant("zbpi") >= str);
   ASSERT_TRUE(StringVariant("zapi") >= str);
   ASSERT_TRUE(buffer >= str);
}

TEST(StringVariantTest, testGtOperator)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str > "abc");
   ASSERT_TRUE(str > std::string("abc"));
   ASSERT_TRUE(str > StringVariant("abc"));
   ASSERT_FALSE(str > "zbpi");
   char buffer[] = "abc";
   ASSERT_TRUE(str > buffer);
   
   ASSERT_TRUE("abcbdf" < str);
   ASSERT_TRUE(std::string("abc") < str);
   ASSERT_TRUE(StringVariant("abc") < str);
   ASSERT_TRUE(buffer < str);
}

TEST(StringVariantTest, testGtEqOperator)
{
   StringVariant str("zapi");
   ASSERT_TRUE(str >= "abc");
   ASSERT_TRUE(str >= std::string("abc"));
   ASSERT_TRUE(str >= StringVariant("abc"));
   ASSERT_FALSE(str >= "zbpi");
   char buffer[] = "abc";
   ASSERT_TRUE(str >= buffer);
   
   ASSERT_TRUE("abc" <= str);
   ASSERT_TRUE(std::string("abc") <= str);
   ASSERT_TRUE(StringVariant("abc") <= str);
   ASSERT_TRUE(buffer <= str);
}

TEST(StringVariantTest, testAccessOperators)
{
   StringVariant str("zapi");
   char &ch1 = str[0];
   ASSERT_EQ(ch1, 'z');
   ch1 = 'x';
   ASSERT_STREQ(str.getCStr(), "xapi");
   const StringVariant str1("zapi");
   const char &ch2 = str1[0];
   ASSERT_EQ(ch2, 'z');
}

TEST(StringVariantTest, testClear)
{
   StringVariant str("0123456789a123456789b1234A56789c");
   ASSERT_STREQ(str.getCStr(), "0123456789a123456789b1234A56789c");
   str.clear();
   ASSERT_EQ(str.getLength(), 0);
   ASSERT_EQ(str.getCapacity(), 0);
   str.append('c');
   ASSERT_STREQ(str.getCStr(), "c");
   ASSERT_EQ(str.getLength(), 1);
   ASSERT_EQ(str.getCapacity(), 199);
}

TEST(StringVariantTest, testResize)
{
   StringVariant str("my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
   ASSERT_EQ(str.getCapacity(), 199);
   ASSERT_EQ(str.getSize(), 99);
   str.resize(32);
   ASSERT_EQ(str.getCapacity(), 64);
   ASSERT_EQ(str.getSize(), 32);
   StringVariant str1 = "zapi";
   ASSERT_EQ(str1.getRefCount(), 1);
   str = str1;
   ASSERT_EQ(str.getRefCount(), 2);
   ASSERT_EQ(str1.getRefCount(), 2);
   str.resize(32);
   ASSERT_EQ(str.getRefCount(), 1);
   ASSERT_EQ(str1.getRefCount(), 1);
   str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   str.resize(12);
   ASSERT_STREQ(str.getCStr(), "my name is z");
   str.clear();
   ASSERT_EQ(str.getCapacity(), 0);
   ASSERT_EQ(str.getSize(), 0);
   str.resize(12);
   ASSERT_EQ(str.getCapacity(), 40);
   ASSERT_EQ(str.getSize(), 12);
   str = "zapi";
   ASSERT_EQ(str.getCapacity(), 40);
   ASSERT_EQ(str.getSize(), 4);
   str.resize(12, '-');
   ASSERT_STREQ(str.getCStr(), "zapi--------");
   //std::cout << str << std::endl;
}

TEST(StringVariantTest, testContains)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_TRUE(str.contains("name"));
   char searchArray[4] = {'b', 'e', 's', 't'};
   ASSERT_TRUE(str.contains(searchArray, 4));
   ASSERT_TRUE(str.contains(searchArray));
   ASSERT_FALSE(str.contains("PHP"));
   ASSERT_TRUE(str.contains("PHP", false));
}

TEST(StringVariantTest, testIndexOf)
{
   StringVariant str("my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
   char subStr[] = {'p','h', 'p', 'i', 's', 't', 'h', 'e'};
   int pos = str.indexOf(subStr, 3);
   ASSERT_EQ(pos, 32);
   pos = str.indexOf(subStr, 4);
   ASSERT_EQ(pos, -1);
   pos = str.indexOf("php");
   ASSERT_EQ(pos, 32);
   pos = str.indexOf("PhP");
   ASSERT_EQ(pos, -1);
   pos = str.indexOf(std::string("php"));
   ASSERT_EQ(pos, 32);
   pos = str.indexOf('n');
   ASSERT_EQ(pos, 3);
   pos = str.indexOf("php", 33);
   ASSERT_EQ(pos, 83);
   pos = str.indexOf("PhP", 0, false);
   ASSERT_EQ(pos, 32);
   pos = str.indexOf("pHP", 0, false);
   ASSERT_EQ(pos, 32);
   pos = str.indexOf("ZZU_SOFTBOY", 0, false);
   ASSERT_EQ(pos, 11);
   char phpArr[] = {'p', 'h', 'p'};
   pos = str.indexOf(phpArr);
   ASSERT_EQ(pos, 32);
}

TEST(StringVariantTest, testLastIndexOf)
{
   // from php online manual
   StringVariant str("0123456789a123456789b1234A56789c");
   int pos = str.lastIndexOf('7', -5);
   ASSERT_EQ(pos, 17);
   pos = str.lastIndexOf('7', 20);
   ASSERT_EQ(pos, 28);
   pos = str.lastIndexOf('7', 29);
   ASSERT_EQ(pos, -1);
   pos = str.lastIndexOf('a', 0, false);
   ASSERT_EQ(pos, 25);
   pos = str.lastIndexOf('a', -7, false);
   ASSERT_EQ(pos, 25);
   pos = str.lastIndexOf('A', 0, false);
   ASSERT_EQ(pos, 25);
   pos = str.lastIndexOf('A', -7, false);
   ASSERT_EQ(pos, 25);
   pos = str.lastIndexOf('A', -8, false);
   ASSERT_EQ(pos, 10);
   pos = str.lastIndexOf('a', -8, false);
   ASSERT_EQ(pos, 10);
   char arr[] = {'4', '5', '6'};
   pos = str.lastIndexOf(arr);
   ASSERT_EQ(pos, 14);
}

TEST(StringVariantTest, testStartWiths)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_TRUE(str.startsWith("my name is zzu"));
   ASSERT_FALSE(str.startsWith("my name is zzu_softboy"));
   ASSERT_TRUE(str.startsWith("my name is zzu_softboy", false));
   char search[7] = {'m', 'y', ' ', 'n', 'a', 'm', 'e'};
   ASSERT_TRUE(str.startsWith(search, 7));
   char search1[7] = {'m', 'y', ' ', 'N', 'a', 'm', 'e'};
   ASSERT_FALSE(str.startsWith(search1, 7));
   ASSERT_TRUE(str.startsWith(search1, 7, false));
   
}

TEST(StringVariantTest, testEndWiths)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_TRUE(str.endsWith("php is the best!"));
   ASSERT_FALSE(str.endsWith("php Is The best!"));
   ASSERT_TRUE(str.endsWith("php Is The best!", false));
   char endSearch[5] = {'b', 'e', 's', 't', '!'};
   ASSERT_TRUE(str.endsWith(endSearch, 5));
   char endSearch1[5] = {'b', 'e', 's', 'T', '!'};
   ASSERT_FALSE(str.endsWith(endSearch1, 5));
   ASSERT_TRUE(str.endsWith(endSearch1, 5, false));
}

TEST(StringVariantTest, testLeft)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_STREQ(str.left(2).c_str(), "my");
   ASSERT_STREQ(str.left(111).c_str(), "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
}

TEST(StringVariantTest, testRight)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_STREQ(str.right(2).c_str(), "t!");
   ASSERT_STREQ(str.right(12).c_str(), "is the best!");
   ASSERT_STREQ(str.right(111).c_str(), "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
}

TEST(StringVariantTest, testJustify)
{
   StringVariant str("zapi");
   ASSERT_STREQ(str.leftJustified(2, '.').c_str(), "za");
   ASSERT_STREQ(str.leftJustified(8, '.').c_str(), "zapi....");
   //std::cout << str.leftJustified(8, '.').c_str() << std::endl;
   ASSERT_STREQ(str.rightJustified(2, '.').c_str(), "za");
   ASSERT_STREQ(str.rightJustified(8, '.').c_str(), "....zapi");
   //std::cout << str.rightJustified(8, '.').c_str() << std::endl;
}

TEST(StringVariantTest, testSubString)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   ASSERT_STREQ(str.substring(0, 6).c_str(), "my nam");
   ASSERT_STREQ(str.substring(3, 6).c_str(), "name i");
   ASSERT_STREQ(str.substring(20).c_str(), "oy, i think php is the best programming language in the world. php is the best!");
   ASSERT_THROW(str.substring(222), std::out_of_range);
   //std::cout << str.substring(3, 6).c_str() << std::endl;
}

TEST(StringVariantTest, testToLowerCaseAndToUpperCase)
{
   StringVariant str("ZZU_softboy");
   ASSERT_STREQ(str.toLowerCase().c_str(), "zzu_softboy");
   ASSERT_STREQ(str.toUpperCase().c_str(), "ZZU_SOFTBOY");
}

TEST(StringVariantTest, testAppendAndPrepend)
{
   {
      StringVariant str("zapi");
      ASSERT_STREQ(str.getCStr(), "zapi");
      ASSERT_EQ(str.getLength(), 4);
      str.append(1);
      ASSERT_EQ(str.getLength(), 5);
      ASSERT_STREQ(str.getCStr(), "zapi1");
      char needAppend[] = {'p', 'h', 'p'};
      str.append(needAppend, 3);
      ASSERT_EQ(str.getLength(), 8);
      ASSERT_STREQ(str.getCStr(), "zapi1php");
      str.append("cpp");
      ASSERT_EQ(str.getLength(), 11);
      ASSERT_STREQ(str.getCStr(), "zapi1phpcpp");
      StringVariant str1 = "hello";
      str.append(str1);
      ASSERT_EQ(str.getLength(), 16);
      ASSERT_STREQ(str.getCStr(), "zapi1phpcpphello");
      str.append(needAppend);
      ASSERT_STREQ(str.getCStr(), "zapi1phpcpphellophp");
   }
   {
      StringVariant str("zapi");
      ASSERT_STREQ(str.getCStr(), "zapi");
      ASSERT_EQ(str.getLength(), 4);
      str.prepend(1);
      ASSERT_EQ(str.getLength(), 5);
      ASSERT_STREQ(str.getCStr(), "1zapi");
      char needPrepend[] = {'p', 'h', 'p'};
      str.prepend(needPrepend, 3);
      ASSERT_EQ(str.getLength(), 8);
      ASSERT_STREQ(str.getCStr(), "php1zapi");
      str.prepend("cpp");
      ASSERT_EQ(str.getLength(), 11);
      ASSERT_STREQ(str.getCStr(), "cppphp1zapi");
      StringVariant str1 = "hello";
      str.prepend(str1);
      ASSERT_EQ(str.getLength(), 16);
      ASSERT_STREQ(str.getCStr(), "hellocppphp1zapi");
      str.prepend(needPrepend);
      ASSERT_STREQ(str.getCStr(), "phphellocppphp1zapi");
   }
}

TEST(StringVariantTest, testRemove)
{
   StringVariant str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best!";
   size_t oldLength = str.getLength();
   str.remove(2, 4);
   ASSERT_EQ(str.getLength(), oldLength - 4);
   ASSERT_STREQ(str.getCStr(), "mye is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
   ASSERT_THROW(str.remove(111, 4), std::out_of_range);
   oldLength = str.getLength();
   str.remove(0);
   ASSERT_STREQ(str.getCStr(), "ye is zzu_Softboy, i think php is the best programming language in the world. php is the best!");
   ASSERT_EQ(str.getLength(), oldLength - 1);
   // test negative pos
   oldLength = str.getLength();
   str.remove(-1);
   ASSERT_STREQ(str.getCStr(), "ye is zzu_Softboy, i think php is the best programming language in the world. php is the best");
   ASSERT_EQ(str.getLength(), oldLength - 1);
   oldLength = str.getLength();
   str.remove(-4, 4);
   ASSERT_STREQ(str.getCStr(), "ye is zzu_Softboy, i think php is the best programming language in the world. php is the ");
   ASSERT_EQ(str.getLength(), oldLength - 4);
   ASSERT_THROW(str.remove(-100, 4), std::out_of_range);
   str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best! But PHP a little slow";
   str.remove("php");
   ASSERT_STREQ(str.getCStr(), "my name is zzu_Softboy, i think  is the best programming language in the world.  is the best! But PHP a little slow");
   str.remove("php", false);
   ASSERT_STREQ(str.getCStr(), "my name is zzu_Softboy, i think  is the best programming language in the world.  is the best! But  a little slow");
   str.remove('z');
   ASSERT_STREQ(str.getCStr(), "my name is u_Softboy, i think  is the best programming language in the world.  is the best! But  a little slow");
   StringVariant emptyStr;
   ASSERT_THROW(emptyStr.remove(1, 1), std::out_of_range);
   emptyStr = str;
   ASSERT_EQ(emptyStr.getRefCount(), 2);
   ASSERT_EQ(str.getRefCount(), 2);
   emptyStr.remove(1);
   ASSERT_STREQ(emptyStr.getCStr(), "m name is u_Softboy, i think  is the best programming language in the world.  is the best! But  a little slow");
   ASSERT_EQ(emptyStr.getRefCount(), 1);
   ASSERT_EQ(str.getRefCount(), 1);
}

TEST(StringVariantTest, testStrInsert)
{
   StringVariant str("zapi");
   ASSERT_STREQ(str.getCStr(), "zapi");
   ASSERT_EQ(str.getLength(), 4);
   str.insert(1, "x");
   ASSERT_EQ(str.getLength(), 5);
   ASSERT_STREQ(str.getCStr(), "zxapi");
   str.insert(0, "x");
   ASSERT_EQ(str.getLength(), 6);
   ASSERT_STREQ(str.getCStr(), "xzxapi");
   str.insert(6, "ab");
   ASSERT_EQ(str.getLength(), 8);
   ASSERT_STREQ(str.getCStr(), "xzxapiab");
   ASSERT_THROW(str.insert(9, "ab"), std::out_of_range);
   str.insert(8, 123);
   ASSERT_STREQ(str.getCStr(), "xzxapiab123");
   str.clear();
   ASSERT_EQ(str.getLength(), 0);
   ASSERT_EQ(str.getCapacity(), 0);
   str.insert(0, "abc");
   ASSERT_EQ(str.getLength(), 3);
   ASSERT_STREQ(str.getCStr(), "abc");
   // negative pos
   str.insert(-1, 'x');
   str.insert(-1, 123);
   ASSERT_EQ(str.getLength(), 7);
   ASSERT_STREQ(str.getCStr(), "abx123c");
   ASSERT_THROW(str.insert(-8, "xx"), std::out_of_range);
   str.insert(-7, std::string("x"));
   ASSERT_EQ(str.getLength(), 8);
   int8_t pos1 = -2;
   str.insert(pos1, StringVariant("vv"));
   ASSERT_STREQ(str.getCStr(), "xabx12vv3c");
   // insert array
   char arr[] = {'p', 'h', 'p'};
   str.insert(1, arr, 3);
   ASSERT_STREQ(str.getCStr(), "xphpabx12vv3c");
   str.insert(1, arr);
   ASSERT_STREQ(str.getCStr(), "xphpphpabx12vv3c");
   str.insert(-1, arr);
   ASSERT_STREQ(str.getCStr(), "xphpphpabx12vv3phpc");
   int8_t pos2 = -2;
   str.insert(pos2, arr);
   ASSERT_STREQ(str.getCStr(), "xphpphpabx12vv3phphppc");
   str.insert(0, arr, -1);
   ASSERT_STREQ(str.getCStr(), "phpxphpphpabx12vv3phphppc");
   //   std::cout << str << std::endl;
}

TEST(StringVariantTest, testRepeated)
{
   StringVariant str;
   std::string repeatedStr = str.repeated(1);
   ASSERT_STREQ(repeatedStr.c_str(), "");
   str = "zapi";
   repeatedStr = str.repeated(1);
   ASSERT_STREQ(repeatedStr.c_str(), "zapi");
   repeatedStr = str.repeated(3);
   ASSERT_STREQ(repeatedStr.c_str(), "zapizapizapi");
}

TEST(StringVariantTest, testSplits)
{
   std::vector<std::string> expected;
   StringVariant text("aaa||bbb||ccc||ddd||eee");
   std::vector<std::string> parts = text.split("||");
   expected = {"aaa", "bbb", "ccc", "ddd", "eee"};
   ASSERT_EQ(parts, expected);
   text = "||aaa||bbb||||ccc||ddd||";
   expected = {"", "aaa", "bbb", "", "ccc", "ddd", ""};
   parts = text.split("||");
   ASSERT_EQ(parts, expected);
   expected = {"", "", "", "", "", ""};
   text = "||||||||||";
   parts = text.split("||");
   ASSERT_EQ(parts, expected);
   text = "ashgdahsd";
   expected = {"ashgdahsd"};
   parts = text.split("||");
   ASSERT_EQ(parts, expected);
   
   text = "||aaa||bbb||||ccc||ddd||";
   expected = {"aaa", "bbb", "ccc", "ddd"};
   parts = text.split("||", false);
   ASSERT_EQ(parts, expected);
   text = "||||||||||";
   expected = {};
   parts = text.split("||", false);
   ASSERT_EQ(parts, expected);
   
   text = "aaaXXbbbxxcccXXdddXXeee";
   expected = {"aaa", "bbbxxccc", "ddd", "eee"};
   parts = text.split("XX", false, true);
   ASSERT_EQ(parts, expected);
   text = "aaaXXbbbxxcccXXdddXXeee";
   expected = {"aaa", "bbb", "ccc", "ddd", "eee"};
   parts = text.split("Xx", false, false);
   ASSERT_EQ(parts, expected);
}

TEST(StringVariantTest, testReplace)
{
   StringVariant str("my name is zzu_softboy, i love php");
   str.replace(0, 2, "MY");
   ASSERT_STREQ(str.getCStr(), "MY name is zzu_softboy, i love php");
   str.replace(3, 4, "NAME");
   ASSERT_STREQ(str.getCStr(), "MY NAME is zzu_softboy, i love php");
   str.replace(str.getLength() - 3, 4, "PHP");
   ASSERT_STREQ(str.getCStr(), "MY NAME is zzu_softboy, i love PHP");
   char replaceArr[] = {'z', 'a', 'p', 'i'};
   str.replace(0, 2, replaceArr);
   ASSERT_STREQ(str.getCStr(), "zapi NAME is zzu_softboy, i love PHP");
   str = "MY NAME is zzu_softboy, i love PHP";
   str.replace(0, 2, replaceArr, -1);
   ASSERT_STREQ(str.getCStr(), "zapi NAME is zzu_softboy, i love PHP");
   str = "MY NAME is zzu_softboy, i love PHP";
   str.replace(0, 2, replaceArr, 2);
   ASSERT_STREQ(str.getCStr(), "za NAME is zzu_softboy, i love PHP");
   //std::cout << str << std::endl;
   str.replace(-3, 4, "php");
   ASSERT_STREQ(str.getCStr(), "za NAME is zzu_softboy, i love php");
   str = "MY NAME is zzu_softboy, i love PHP";
   str.replace(-3, -4, "php");
   ASSERT_STREQ(str.getCStr(), "MY NAME is zzu_softboy, i love php");
   str.replace(-3, 3, replaceArr);
   ASSERT_STREQ(str.getCStr(), "MY NAME is zzu_softboy, i love zapi");
   str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best! pHp is very fast!";
   str.replace("php", "PHP");
   ASSERT_STREQ(str.getCStr(), "my name is zzu_Softboy, i think PHP is the best programming language in the world. PHP is the best! pHp is very fast!");
   //std::cout << str << std::endl;
   str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best! pHp is very fast!";
   str.replace("php", "PHP", false);
   ASSERT_STREQ(str.getCStr(), "my name is zzu_Softboy, i think PHP is the best programming language in the world. PHP is the best! PHP is very fast!");
   str = "my name is zzu_Softboy, i think php is the best programming language in the world. php is the best! pHp is very fast!";
   str.replace('p', '_');
   ASSERT_STREQ(str.getCStr(), "my name is zzu_Softboy, i think _h_ is the best _rogramming language in the world. _h_ is the best! _H_ is very fast!");
   //std::cout << str << std::endl;
}

TEST(StringVariantTest, testPlusOperator)
{
   StringVariant str("zapi");
   std::string ret = str + "-php";
   ASSERT_STREQ(ret.c_str(), "zapi-php");
   ret = "php-" + str;
   ASSERT_STREQ(ret.c_str(), "php-zapi");
   ret = str + std::string("-php");
   ASSERT_STREQ(ret.c_str(), "zapi-php");
   ret = std::string("php-") + str;
   ASSERT_STREQ(ret.c_str(), "php-zapi");
   ret = str + StringVariant("-php");
   ASSERT_STREQ(ret.c_str(), "zapi-php");
   ret = StringVariant("php-") + str;
   ASSERT_STREQ(ret.c_str(), "php-zapi");
   char buffer[] = "-php";
   ret = str + buffer;
   ASSERT_STREQ(ret.c_str(), "zapi-php");
   ret = buffer + str;
   ASSERT_STREQ(ret.c_str(), "-phpzapi");
   char buffer1[] = {'-', 'p', 'h', 'p'};
   ret = str + buffer1;
   ASSERT_STREQ(ret.c_str(), "zapi-php");
   ret = buffer1 + str;
   ASSERT_STREQ(ret.c_str(), "-phpzapi");
   ret = str + 'c';
   ASSERT_STREQ(ret.c_str(), "zapic");
   ret = 'c' + str;
   ASSERT_STREQ(ret.c_str(), "czapi");
}

TEST(StringVariantTest, testEmptyStr)
{
   StringVariant emptyStr;
   ASSERT_EQ(emptyStr.getLength(), 0);
   ASSERT_EQ(emptyStr.getCStr(), nullptr);
   ASSERT_EQ(emptyStr.getData(), nullptr);
   ASSERT_FALSE(emptyStr.startsWith("x"));
   ASSERT_FALSE(emptyStr.endsWith("x"));
   ASSERT_EQ(emptyStr.indexOf("x"), -1);
   ASSERT_EQ(emptyStr.lastIndexOf("x"), -1);
   emptyStr.clear();
   ASSERT_EQ(emptyStr.getLength(), 0);
   ASSERT_EQ(emptyStr.getCStr(), nullptr);
   ASSERT_EQ(emptyStr.getData(), nullptr);
   ASSERT_FALSE(emptyStr.startsWith("x"));
   ASSERT_FALSE(emptyStr.endsWith("x"));
   ASSERT_EQ(emptyStr.indexOf("x"), -1);
   ASSERT_EQ(emptyStr.lastIndexOf("x"), -1);
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
