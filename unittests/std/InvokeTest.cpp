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
#include "zapi/stdext/Functional.h"
#include <iostream>
#include <functional>
#include "zapi/stdext/Tuple.h"
#include <tuple>
using zapi::stdext::invoke;
using zapi::stdext::apply;

struct Foo {
   Foo(int num) : num_(num) {}
   int print_add(int i) 
   { 
      return num_+i;
   }
   
   int num_;
};

int print_num(int i)
{
   return i;
}

int get_sum(int a, int b, int c) 
{
   return a + b + c;
}

struct PrintNum {
   int operator()(int i) const
   {
      return i;
   }
};

int add(int first, int second)
{
   return first + second;    
}

template<typename T>
T add_generic(T first, T second)
{
   return first + second;    
}

TEST(InvokeTest, testInvoke)
{
   ASSERT_EQ(zapi::stdext::invoke(print_num, -9), -9);
   ASSERT_EQ(zapi::stdext::invoke(print_num, 222), 222);
   // invoke a lambda
   invoke([]() { 
      ASSERT_EQ(print_num(42), 42);
   });
   Foo foo(314159);
   // invoke (access) a data member
   //   std::cout << "num_: " << zapi::std::invoke(&Foo::num_, foo) << '\n';
   // invoke a function object
   ASSERT_EQ(zapi::stdext::invoke(&Foo::num_, foo), 314159);
   ASSERT_EQ(zapi::stdext::invoke(PrintNum(), 18), 18);
   ASSERT_EQ(zapi::stdext::invoke(&Foo::print_add, foo, 1), 314160);
   //std::cout << zapi::std::apply(get_sum, std::make_tuple(1, 2, 3));
}

TEST(InvokeTest, testApply)
{
   ASSERT_EQ(zapi::stdext::apply(get_sum, std::make_tuple(1, 2, 3)), 6);
   ASSERT_EQ(zapi::stdext::apply(add, std::make_tuple(1,2)), 3);
   // template argument deduction/substitution fails
//    std::cout << zapi::std::apply(add_generic, std::make_tuple(2.0f,3.0f)) << '\n'; 
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