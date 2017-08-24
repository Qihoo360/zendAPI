#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"

#include "zapi/vm/Engine.h"
#include "zapi/ds/Variant.h"
#include "zapi/ds/NumericVariant.h"
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <tuple>
#include <type_traits>

using zapi::ds::Variant;
using zapi::ds::NumericVariant;

using FuncType = void (*)(const std::vector<Variant> &params);

template <typename ...ArgsTypes>
void func_tpl(ArgsTypes... args)
{}

void user_func(const NumericVariant &arg1, NumericVariant arg2)
{
   std::cout << arg1 + arg2 << std::endl;
}

template <typename>
struct ArgNums;

template <typename R, typename ...ArgTypes>
struct ArgNums<R(ArgTypes... args)>
{
   constexpr static int value = sizeof...(ArgTypes);
};
template <typename R, typename T, typename ...ArgTypes>
struct ArgNums<R(T::*)(ArgTypes... args)>
{
   constexpr static int value = sizeof...(ArgTypes);
};

class Person
{
public:
   void add(const NumericVariant &arg1, NumericVariant arg2, NumericVariant arg3)
   {
      std::cout << arg1 + arg2 + arg3 << std::endl;
   }
};

class Invoker
{
public:
   template <typename T,
             typename std::decay<T>::type f,
             //T f,
             typename Selector = typename std::enable_if<std::is_function<T>::value>::type
             >
   static void invoke(const std::vector<Variant> &params)
   {
      //      std::tuple<int> args(1);
      //      auto args1 = std::tuple_cat(args, std::make_tuple(12));
      //      std::apply(f, args1);
      //      std::cout << ArgNums<T>::value << std::endl;
      std::cout << "xiuxiux" << std::endl;
   }
   template <typename T,
             typename std::decay<T>::type f,
             //T f,
             typename Selector = typename std::enable_if<std::is_member_function_pointer<T>::value>::type,
             int unused = 1
             >
   static void invoke(const std::vector<Variant> &params)
   {
      //      std::tuple<int> args(1);
      //      auto args1 = std::tuple_cat(args, std::make_tuple(12));
      //      std::apply(f, args1);
            std::cout << ArgNums<T>::value << std::endl;
      std::cout << "xiuxiux class" << std::endl;
      ((new Person())->*f)(1, 2, 3);
   }
   
};

//FuncType ii = &Invoker::invoke<decltype(user_func), user_func>;
//FuncType ii = &Invoker::invoke<decltype(user_func), user_func>;
FuncType ii = &Invoker::invoke<decltype(user_func), user_func>;
FuncType ii1 = &Invoker::invoke<decltype(&Person::add), &Person::add>;
TEST(InvokeBridgeTest, testGeneralFuncWrapper)
{
   std::vector<Variant> p;
   ii(p);
   ii1(p);
}
