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
#include "zapi/ds/NumericVariant.h"
#include <tuple>

template<typename T> 
struct function_traits;  

template<typename R, typename ...Args> 
struct function_traits<std::function<R(Args...)>>
{
    static const size_t nargs = sizeof...(Args);

    typedef R result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

using zapi::ds::NumericVariant;

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

void get_value_ref(NumericVariant number)
{
   
}

} // dummyext

class TestClass
{
public:
   void getName()
   {
      
   }
   void static getVersion()
   {
      
   }
};

TEST(TypeTraitTest, test)
{
   bool ret = std::is_same<zapi::stdext::CallableInfoTrait<decltype(func2)>::ReturnType, int>::value;
   ASSERT_TRUE(ret);
   auto func3 = []()->int{};
   ret = std::is_same<zapi::stdext::CallableInfoTrait<decltype(func2)>::ReturnType, int>::value;
   ASSERT_TRUE(ret);
   ret = std::is_same<zapi::stdext::CallableInfoTrait<decltype(&O::method1)>::ReturnType, char>::value;
   ASSERT_TRUE(ret);
   ret = std::is_same<zapi::stdext::CallableInfoTrait<decltype(&dummyext::print_name)>::ReturnType, void>::value;
   ASSERT_TRUE(ret);
   int paramNumber = zapi::stdext::CallableInfoTrait<decltype(&func1)>::argNum;
   ASSERT_EQ(paramNumber, 0);
   paramNumber = zapi::stdext::CallableInfoTrait<decltype(&func2)>::argNum;
   ASSERT_EQ(paramNumber, 2);
//   ret = std::is_member_function_pointer<std::decay<decltype(&TestClass::getVersion)>::type>::value;
//   ASSERT_TRUE(ret);
   ret = std::is_function<std::decay<decltype(&TestClass::getVersion)>::type>::value;
   //ASSERT_TRUE(ret);
   ///int xx = zapi::stdext::function_traits<void(NumericVariant)>::nargs;
   //ret = std::is_same<zapi::stdext::function_traits<typename std::remove_pointer<decltype(&dummyext::get_value_ref)>::type>::arg<0>::type, NumericVariant>::value;
   std::cout << std::is_same<NumericVariant, function_traits<std::function<decltype(dummyext::get_value_ref)>>::arg<0>::type>::value << std::endl;
}
