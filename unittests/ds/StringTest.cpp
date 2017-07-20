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
// Created by softboy on 2017/06/13.

#include "php/sapi/embed/php_embed.h"

#include "gtest/gtest.h"

#include "zapi/ds/String.h"

using zapi::ds::String;

namespace
{

class StringTest : public ::testing::Test
{};

} // end of namespace

TEST_F(StringTest, testConstructor)
{
   {
      // test empty string
      String str;
      ASSERT_TRUE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 0);
   }
   {
      // test String(const std::string &string)
      String str(std::string("zapi"));
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
      str.~String();
      ASSERT_EQ(str.getSize(), 0);
   }
   {
      // test String(const char *string)
      String str("zapi");
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
      str.~String();
      ASSERT_EQ(str.getSize(), 0);
   }
   {
      // test String(const char *string, size_t size)
      String str("zapi hello", 4);
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
      str.~String();
      ASSERT_EQ(str.getSize(), 0);
   }
   {
      // test String(String &&other)
      String str("zapi");
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
      String str1(std::move(str));
      ASSERT_EQ(str.getSize(), 0);
      ASSERT_EQ(str1.getSize(), 4);
      ASSERT_EQ(std::strncmp(str1.getData(), "zapi", 4), 0);
   }
   {
      // test String(const String &other)
      String str("zapi");
      String str1(str);
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
      ASSERT_EQ(str1.getSize(), 4);
      ASSERT_EQ(std::strncmp(str1.getData(), "zapi", 4), 0);
   }
   {
      char strArr[] = "zapi";
      String str(strArr);
      ASSERT_FALSE(str.isEmpty());
      ASSERT_EQ(str.getSize(), 4);
      ASSERT_EQ(std::strncmp(str.getData(), "zapi", 4), 0);
   }
}

TEST_F(StringTest, testGetData)
{
   String str("zapi");
   char *rawStrPtr = str.getData();
   ASSERT_EQ(std::strncmp(rawStrPtr, "zapi", 4), 0);
   const char *constRawPtr = str.getData();
   ASSERT_EQ(std::strncmp(constRawPtr, "zapi", 4), 0);
}

TEST_F(StringTest, testRefCount)
{
   String str("zapi");
   ASSERT_EQ(str.getRefCount(), 1);
   {
      String str1(str);
      ASSERT_EQ(str.getRefCount(), 2);
      ASSERT_EQ(str1.getRefCount(), 2);
      {
         String str2(str1);
         ASSERT_EQ(str.getRefCount(), 3);
         ASSERT_EQ(str1.getRefCount(), 3);
         ASSERT_EQ(str2.getRefCount(), 3);
      }
      ASSERT_EQ(str.getRefCount(), 2);
      ASSERT_EQ(str1.getRefCount(), 2);
   }
   ASSERT_EQ(str.getRefCount(), 1);
}
