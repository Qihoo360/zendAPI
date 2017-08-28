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
// Created by softboy on 2017/08/25.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"

#include "zapi/stdext/TypeTraits.h"
#include <type_traits>

int func1() 
{
   return 1;
}
int func2(int arg1, char arg2)
{
   return 1;
}

struct O
{
   char method1(int arg1, char arg2)
   {
      return 'a';
   }
};

namespace dummyext
{

void print_name()
{
   
}

} // dummyext

TEST(TypeTraitTest, test)
{
   bool ret = std::is_same<zapi::stdext::function_return_type<decltype(func2)>::type, int>::value;
   ASSERT_TRUE(ret);
   auto func3 = []()->int{};
   ret = std::is_same<zapi::stdext::callable_return_type<decltype(func2)>::type, int>::value;
   ASSERT_TRUE(ret);
   ret = std::is_same<zapi::stdext::callable_return_type<decltype(&O::method1)>::type, char>::value;
   ASSERT_TRUE(ret);
   ret = std::is_same<zapi::stdext::callable_return_type<decltype(&dummyext::print_name)>::type, void>::value;
   ASSERT_TRUE(ret);
   int paramNumber = zapi::stdext::callable_params_number<decltype(&func1)>::value;
   ASSERT_EQ(paramNumber, 0);
   paramNumber = zapi::stdext::callable_params_number<decltype(&func2)>::value;
   ASSERT_EQ(paramNumber, 2);
}
