// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by zzu_softboy on 27/07/2017.

#include "gtest/gtest.h"
#include "zapi/lang/Argument.h"

using zapi::lang::ValueArgument;
using zapi::lang::Type;

TEST(ArgumentTest, testConstructor)
{
   {
      ValueArgument arg("arg1");
      ASSERT_STREQ(arg.getName(), "arg1");
      ASSERT_STREQ(arg.getClassName(), nullptr);
      ASSERT_EQ(arg.getType(), Type::Null);
      ASSERT_TRUE(arg.isRequired());
      ASSERT_FALSE(arg.isReference());
   }
   {
      ValueArgument arg("argname", Type::Array);
      ASSERT_EQ(arg.getType(), Type::Array);
   }
   {
      ValueArgument arg("argname", Type::Array, false);
      ASSERT_EQ(arg.getType(), Type::Array);
      ASSERT_FALSE(arg.isRequired());
   }
   {
      ValueArgument arg("argname", "classname", false);
      ASSERT_TRUE(arg.isRequired());
      ASSERT_STREQ(arg.getClassName(), "classname");
      ASSERT_FALSE(arg.isNullable());
   }
   {
      ValueArgument arg("argname", "classname1", false, false);
      ASSERT_FALSE(arg.isRequired());
      ASSERT_STREQ(arg.getClassName(), "classname1");
      ASSERT_FALSE(arg.isNullable());
   }
}

TEST(ArgumentTest, testCopyConstructor)
{
   {
      ValueArgument arg("argname", "classname1", false, false);
      ValueArgument copy(arg);
      ASSERT_FALSE(copy.isRequired());
      ASSERT_STREQ(copy.getClassName(), "classname1");
      ASSERT_FALSE(copy.isNullable());
   }
   {
      ValueArgument arg("arg1");
      ValueArgument copy(arg);
      ASSERT_STREQ(copy.getName(), "arg1");
      ASSERT_STREQ(copy.getClassName(), nullptr);
      ASSERT_EQ(copy.getType(), Type::Null);
      ASSERT_TRUE(copy.isRequired());
      ASSERT_FALSE(copy.isReference());
   }
}

TEST(ArgumentTest, testMoveConstructor)
{
   {
      ValueArgument arg("argname", "classname1", false, false);
      ValueArgument move(std::move(arg));
      ASSERT_FALSE(move.isRequired());
      ASSERT_STREQ(move.getClassName(), "classname1");
      ASSERT_FALSE(move.isNullable());
   }
   {
      ValueArgument arg("arg1");
      ValueArgument move(std::move(arg));
      ASSERT_STREQ(move.getName(), "arg1");
      ASSERT_STREQ(move.getClassName(), nullptr);
      ASSERT_EQ(move.getType(), Type::Null);
      ASSERT_TRUE(move.isRequired());
      ASSERT_FALSE(move.isReference());
   }
}

TEST(ArgumentTest, testAssignOperator)
{
   {
      ValueArgument arg("argname", "classname1", false, false);
      ValueArgument copy = arg;
      ASSERT_FALSE(copy.isRequired());
      ASSERT_STREQ(copy.getClassName(), "classname1");
      ASSERT_FALSE(copy.isNullable());
   }
   {
      ValueArgument arg("argname", "classname1", false, false);
      ValueArgument move = std::move(arg);
      ASSERT_FALSE(move.isRequired());
      ASSERT_STREQ(move.getClassName(), "classname1");
      ASSERT_FALSE(move.isNullable());
   }
}
