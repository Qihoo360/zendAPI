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

using zapi::ds::StringVariant;
using zapi::ds::Variant;

TEST(StringVariantTest, testConstructors)
{
   StringVariant str("xiuxiu");
   StringVariant emptyStr;
   ASSERT_TRUE(emptyStr.isEmpty());
   ASSERT_EQ(emptyStr.getCapacity(), 199);
   ASSERT_EQ(emptyStr.getSize(), 0);
   emptyStr.append('C');
   ASSERT_EQ(emptyStr.getSize(), 1);
   ASSERT_EQ(emptyStr.getCapacity(), 199);
   ASSERT_EQ(emptyStr.at(0), 'C');
}

TEST(StringVariantTest, testConstructFromVariant)
{
   Variant strVariant("zapi is the best!");
   Variant numericVariant(123);
   StringVariant str(strVariant);
   StringVariant str1(numericVariant);
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
}

TEST(StringVariantTest, testToLowerCaseAndToUpperCase)
{
   StringVariant str("ZZU_softboy");
   ASSERT_STREQ(str.toLowerCase().c_str(), "zzu_softboy");
   ASSERT_STREQ(str.toUpperCase().c_str(), "ZZU_SOFTBOY");
}