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
// Created by softboy on 2017/08/24.
//

#ifndef ZAPI_STD_INTERNAL_FUNCTIONAL_PRIVATE_H
#define ZAPI_STD_INTERNAL_FUNCTIONAL_PRIVATE_H

#include <type_traits>
#include <functional>
#include "zapi/std/TypeTraits.h"

namespace langstd = std;

namespace zapi
{
namespace std
{
namespace internal
{

template <typename TypePointer>
struct has_result_type
{
private:
   struct TwoMark {
      char c;
      char c2;
   };
   template <typename SourcePointer>
   static TwoMark test(...);
   template <typename SourcePointer>
   static char test(typename SourcePointer::result_type * = nullptr);
public:
   static const bool value = sizeof(test<TypePointer>(nullptr)) == 1;
};

// for weak_result_type
template <typename TypePointer>
struct derives_from_unary_function
{
private:
   struct TwoMark {
      char c;
      char c2;
   };
   static TwoMark test(...);
   template <typename ArgumentType, typename ReturnType>
   static langstd::unary_function<ArgumentType, ReturnType>
   test(const volatile langstd::unary_function<ArgumentType, ReturnType> *);
public:
   static const bool value = !langstd::is_same<decltype(test(reinterpret_cast<TypePointer *>(0))), TwoMark>::value;
   using type = decltype(test(reinterpret_cast<TypePointer *>(0)));
};

template <typename TypePointer>
struct derives_from_binary_function
{
private:
   struct TwoMark {
      char c;
      char c2;
   };
   static TwoMark test(...);
   template <typename Arg1Type, typename Arg2Type, class ReturnType>
   static langstd::binary_function<Arg1Type, Arg2Type, ReturnType> 
   test(const volatile langstd::binary_function<Arg1Type, Arg2Type, ReturnType> *);
public:
   static const bool value = !langstd::is_same<decltype(test(reinterpret_cast<TypePointer *>(0))), TwoMark>::value;
   using type = decltype(test(reinterpret_cast<TypePointer *>(0)));
};

template <typename TypePointer, bool = derives_from_unary_function<TypePointer>::value>
struct maybe_derive_from_unary_function // bool is true
      : public derives_from_unary_function<TypePointer>::type
{};

template <typename TypePointer>
struct maybe_derive_from_unary_function<TypePointer, false> 
{};

template <typename TypePointer, bool = derives_from_binary_function<TypePointer>::value>
struct maybe_derive_from_binary_function // bool is true
      : public derives_from_binary_function<TypePointer>::type
{};

template <typename TypePointer>
struct maybe_derive_from_binary_function<TypePointer, false> 
{};

template <typename TypePointer, bool = has_result_type<TypePointer>::value>
struct weak_result_type_impl // bool is true
      : public maybe_derive_from_unary_function<TypePointer>,
      public maybe_derive_from_binary_function<TypePointer>
{
   using result_type = typename TypePointer::result_type;
};

template <typename TypePointer>
struct weak_result_type_impl<TypePointer, false>
      : public maybe_derive_from_unary_function<TypePointer>,
      public maybe_derive_from_binary_function<TypePointer>
{};

template <typename TypePointer>
struct weak_result_type : public weak_result_type_impl<TypePointer>
{};

// 0 argument case
template <typename ReturnType>
struct weak_result_type<ReturnType ()>
{
   using result_type = ReturnType;
};

template <typename ReturnType>
struct weak_result_type<ReturnType (&)()>
{
   using result_type = ReturnType;
};

template <typename ReturnType>
struct weak_result_type<ReturnType (*)()>
{
   using result_type = ReturnType;
};

// 1 argument case
template <typename ReturnType, typename A1>
struct weak_result_type<ReturnType (A1)>
   : public langstd::unary_function<A1, ReturnType>
{};

template <typename ReturnType, typename A1>
struct weak_result_type<ReturnType (&)(A1)>
   : public langstd::unary_function<A1, ReturnType>
{};

template <typename ReturnType, typename A1>
struct weak_result_type<ReturnType (*)(A1)>
   : public langstd::unary_function<A1, ReturnType>
{};

// if class, of course object it self count as a operator
template <typename ReturnType, typename ClassType>
struct weak_result_type<ReturnType (ClassType::*)()>
   : public langstd::unary_function<ClassType *, ReturnType>
{};

template <typename ReturnType, typename ClassType>
struct weak_result_type<ReturnType (ClassType::*)() const>
   : public langstd::unary_function<const ClassType *, ReturnType>
{};

template <typename ReturnType, typename ClassType>
struct weak_result_type<ReturnType (ClassType::*)() volatile>
   : public langstd::unary_function<volatile ClassType *, ReturnType>
{};

template <typename ReturnType, typename ClassType>
struct weak_result_type<ReturnType (ClassType::*)() const volatile>
   : public langstd::unary_function<const volatile ClassType *, ReturnType>
{};

// 2 arguments case
template <typename ReturnType, typename A1, typename A2>
struct weak_result_type<ReturnType (A1, A2)>
   : public langstd::binary_function<A1, A2, ReturnType>
{};

template <typename ReturnType, typename A1, typename A2>
struct weak_result_type<ReturnType (*)(A1, A2)>
   : public langstd::binary_function<A1, A2, ReturnType>
{};

template <typename ReturnType, typename A1, typename A2>
struct weak_result_type<ReturnType (&)(A1, A2)>
   : public langstd::binary_function<A1, A2, ReturnType>
{};

// class type when binary
template <typename ReturnType, typename ClassType, typename A1>
struct weak_result_type<ReturnType (ClassType::*)(A1)>
   : public langstd::binary_function<ClassType *, A1, ReturnType>
{};

template <typename ReturnType, typename ClassType, typename A1>
struct weak_result_type<ReturnType (ClassType::*)(A1) const>
   : public langstd::binary_function<const ClassType *, A1, ReturnType>
{};

template <typename ReturnType, typename ClassType, typename A1>
struct weak_result_type<ReturnType (ClassType::*)(A1) volatile>
   : public langstd::binary_function<volatile ClassType *, A1, ReturnType>
{};

template <typename ReturnType, typename ClassType, typename A1>
struct weak_result_type<ReturnType (ClassType::*)(A1) const volatile>
   : public langstd::binary_function<const volatile ClassType *, A1, ReturnType>
{};

// 3 arguments or more case
template <typename ReturnType, typename A1, typename A2, typename A3, typename ...args>
struct weak_result_type<ReturnType (A1, A2, A3, args...)>
{
   using result_type = ReturnType;
};

template <typename ReturnType, typename A1, typename A2, typename A3, typename ...args>
struct weak_result_type<ReturnType (&)(A1, A2, A3, args...)>
{
   using result_type = ReturnType;
};

template <typename ReturnType, typename A1, typename A2, typename A3, typename ...args>
struct weak_result_type<ReturnType (*)(A1, A2, A3, args...)>
{
   using result_type = ReturnType;
};

// class type
template <typename ReturnType, typename ClassType, typename A1, typename A2, typename ...args>
struct weak_result_type<ReturnType (ClassType::*)(A1, A2, args...)>
{
   using result_type = ReturnType;
};

template <typename ReturnType, typename ClassType, typename A1, typename A2, typename ...args>
struct weak_result_type<ReturnType (ClassType::*)(A1, A2, args...) const>
{
   using result_type = ReturnType;
};

template <typename ReturnType, typename ClassType, typename A1, typename A2, typename ...args>
struct weak_result_type<ReturnType (ClassType::*)(A1, A2, args...) volatile>
{
   using result_type = ReturnType;
};

template <typename ReturnType, typename ClassType, typename A1, typename A2, typename ...args>
struct weak_result_type<ReturnType (ClassType::*)(A1, A2, args...) const volatile>
{
   using result_type = ReturnType;
};

template <typename TargetType, typename ...ArgTypes>
struct invoke_return
{
   using type = decltype(invoke(langstd::declval<TargetType>(), langstd::declval<ArgTypes>()...));
};

//template <typename RetType, typename T1, bool IsFuncPointer, bool IsBase>
//struct enable_invoke_impl;

//template <typename RetType, typename T1>
//struct enable_invoke_impl <RetType, T1, true, true>
//{
//   using Bullet1 = RetType;
//   using Type = Bullet1;
//};

//template <typename RetType, typename T1>
//struct enable_invoke_impl<RetType, T1, true, false>
//{
//   using Bullet2 = RetType;
//   using Type = Bullet2;
//};

//template <typename RetType, typename T1>
//struct enable_invoke_impl<RetType, T1, false, true>
//{
//   using Bullet3 = typename langstd::add_lvalue_reference<
//   typename zapi::std::apply_cv<T1, RetType>::type
//   >::type;
//   using type = Bullet3;
//};

//template <typename RetType, typename T1>
//struct enable_invoke_impl<RetType, T1, false, false>
//{
//   using Bullet4 = typename langstd::add_lvalue_reference<
//   typename zapi::std::apply_cv<decltype(*langstd::declval<T1>()), RetType>::type
//   >::type;
//   using type = Bullet4;
//};

//template <typename RetType, typename T1>
//struct enable_invoke_impl<RetType, T1 *, false, false>
//{
//   using Bullet4 = typename langstd::add_lvalue_reference<
//   typename zapi::std::apply_cv<T1, RetType>::type
//   >::type;
//   using type = Bullet4;
//};

//template <typename FuncType, class T1,
//          typename Traits = zapi::std::member_pointer_traits<FuncType>,
//          typename RetType = typename Traits::ReturnType,
//          typename Class = typename Traits::ClassType>
//struct enable_invoke : public enable_invoke_impl<
//      RetType, T1,
//      langstd::is_member_function_pointer<FuncType>::value,
//      langstd::is_base_of<Class, typename langstd::remove_reference<T1>::type>::value>
//{};

//nat invoke(any, ...);

//template <typename FuncType, typename T1>
//inline typename enable_invoke<FuncType, T1>::Bullet1
//invoke(FuncType func, T1 &t1) {
//   return (t1.*func)();
//}

//template <typename FuncType, typename T1, typename A0>
//inline typename enable_invoke<FuncType, T1>::Bullet1
//invoke(FuncType func, T1 &t1, A0 &a0) {
//   return (t1.*func)(a0);
//}

//template <typename FuncType, typename T1, typename A0, typename A1>
//inline typename enable_invoke<FuncType, T1>::Bullet1
//invoke(FuncType func, T1 &t1, A0 &a0, A1 &a1) {
//   return (t1.*func)(a0, a1);
//}

//template <typename FuncType, typename T1, typename A0, typename A1, typename A2>
//inline typename enable_invoke<FuncType, T1>::Bullet1
//invoke(FuncType func, T1 &t1, A0 &a0, A1 &a1, A2 &a2) {
//   return (t1.*func)(a0, a1, a2);
//}

//template <typename FuncType, typename T1>
//inline typename enable_invoke<FuncType, T1>::Bullet2
//invoke(FuncType func, T1 &t1)
//{
//   return ((*t1).*func)();
//}

//template <typename FuncType, typename T1, typename A0>
//inline typename enable_invoke<FuncType, T1>::Bullet2
//invoke(FuncType func, T1 &t1, A0 &a0)
//{
//   return ((*t1).*func)(a0);
//}

//template <typename FuncType, typename T1, typename A0, typename A1>
//inline typename enable_invoke<FuncType, T1>::Bullet2
//invoke(FuncType func, T1 &t1, A0 &a0, A1 &a1)
//{
//   return ((*t1).*func)(a0, a1);
//}

//template <typename FuncType, typename T1, typename A0, typename A1, typename A2>
//inline typename enable_invoke<FuncType, T1>::Bullet2
//invoke(FuncType func, T1 &t1, A0 &a0, A1 &a1, A2 &a2)
//{
//   return ((*t1).*func)(a0, a1, a2);
//}

//template <typename FuncType, typename T1>
//inline typename enable_invoke<FuncType, T1>::Bullet3
//invoke(FuncType func, T1 &t1)
//{
//   return t1.*func;
//}

//template <typename FuncType, typename T1>
//inline typename enable_invoke<FuncType, T1>::Bullet4
//invoke(FuncType func, T1 &t1)
//{
//   return (*t1).*func;
//}

//template <typename FuncPointer>
//inline decltype(langstd::declval<FuncPointer&>()())
//invoke(FuncPointer& func)
//{
//   return func();
//}

//template <typename FuncPointer, typename A0>
//inline decltype(langstd::declval<FuncPointer&>()(langstd::declval<A0&>()))
//invoke(FuncPointer& func, A0 &a0)
//{
//   return func(a0);
//}

//template <typename FuncPointer, typename A0, typename A1>
//inline decltype(langstd::declval<FuncPointer&>()(langstd::declval<A0&>(),
//                                                 langstd::declval<A1&>()))
//invoke(FuncPointer& func, A0 &a0, A1 &a1)
//{
//   return func(a0, a1);
//}

//template <typename FuncPointer, typename A0, typename A1, typename A2>
//inline decltype(langstd::declval<FuncPointer&>()(langstd::declval<A0&>(),
//                                                 langstd::declval<A1&>(),
//                                                 langstd::declval<A2&>()))
//invoke(FuncPointer& func, A0 &a0, A1 &a1, A2 &a2)
//{
//   return func(a0, a1, a2);
//}

} // internal
} // std
} // zapi

#endif // ZAPI_STD_INTERNAL_FUNCTIONAL_PRIVATE_H