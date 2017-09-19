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

#ifndef ZAPI_STDEXT_TYPETRAITS_H
#define ZAPI_STDEXT_TYPETRAITS_H

#include "zapi/CompilerDetection.h"
#include <type_traits>
#include <tuple>

namespace zapi
{
namespace stdext
{

namespace internal
{
template <typename SourceType, typename TargetType, 
          bool = std::is_const<typename std::remove_reference<TargetType>::type>::value,
          bool = std::is_volatile<typename std::remove_reference<TargetType>::type>::value>
struct apply_cv
{
   using type = TargetType;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType, TargetType, true, false>
{
   using type = const TargetType;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType, TargetType, false, true>
{
   using type = volatile TargetType;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType, TargetType, true, true>
{
   using type = const volatile TargetType;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType &, TargetType, false, false>
{
   using type = TargetType &;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType &, TargetType, true, false>
{
   using type = const TargetType &;
};

template <typename SourceType, typename TargetType>
struct apply_cv<SourceType &, TargetType, false, true>
{
   using type = volatile TargetType &;
};

} // internal

template <typename CallableType>
struct CallableInfoTrait
{
   constexpr const static bool isMemberCallable = false;
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (&)(ParamTypes ...args)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (*)(ParamTypes ...args)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (ParamTypes ...args)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (&)(ParamTypes ...args, ...)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (*)(ParamTypes ...args, ...)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename ...ParamTypes>
struct CallableInfoTrait<RetType (ParamTypes ...args, ...)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = false;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...)>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) volatile>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) volatile>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const volatile>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const volatile>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) &>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) &>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) volatile&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) volatile&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const volatile&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const volatile&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) &&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) &&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) volatile&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) volatile&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args) const volatile&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = false;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct CallableInfoTrait<RetType (Class::*)(ParamTypes... args, ...) const volatile&&>
{
   using ReturnType = RetType;
   constexpr const static size_t argNum = sizeof...(ParamTypes);
   constexpr const static bool hasVaridicParams = true;
   constexpr const static bool hasParamDef = argNum != 0 || hasVaridicParams;
   constexpr const static bool isMemberCallable = true;
   constexpr const static bool hasReturn = !std::is_same<RetType, void>::value;
   template <size_t index>
   struct arg
   {
      using type = typename std::tuple_element<index, std::tuple<ParamTypes...>>::type;
   };
};

namespace internal
{

template <typename MemberPointerType, bool IsMemberFunctionPtr, bool IsMemberObjectPtr>
struct member_pointer_traits_imp
{  // forward declaration; specializations later
   using ClassType = void; // ? is this ok ?
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args), true, false>
{
   using ClassType = Class;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
   // TODO equal ?
   // typedef RetType (FuncType) (ParamTypes...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...), true, false>
{
   using ClassType = Class;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const, true, false>
{
   using ClassType = Class const;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const, true, false>
{
   using ClassType = Class const;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) volatile, true, false>
{
   using ClassType = Class volatile;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) volatile, true, false>
{
   using ClassType = Class volatile;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const volatile, true, false>
{
   using ClassType = Class const volatile;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const volatile, true, false>
{
   using ClassType = Class const volatile;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) &, true, false>
{
   using ClassType = Class &;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) &, true, false>
{
   using ClassType = Class &;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const&, true, false>
{
   using ClassType = Class const&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const&, true, false>
{
   using ClassType = Class const&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) volatile&, true, false>
{
   using ClassType = Class volatile&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) volatile&, true, false>
{
   using ClassType = Class volatile&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const volatile&, true, false>
{
   using ClassType = Class const volatile&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const volatile&, true, false>
{
   using ClassType = Class const volatile&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) &&, true, false>
{
   using ClassType = Class &&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) &&, true, false>
{
   using ClassType = Class &&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const&&, true, false>
{
   using ClassType = Class const&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const&&, true, false>
{
   using ClassType = Class const&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) volatile&&, true, false>
{
   using ClassType = Class volatile&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) volatile&&, true, false>
{
   using ClassType = Class volatile&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args) const volatile&&, true, false>
{
   using ClassType = Class const volatile&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args);
};

template <typename RetType, typename Class, typename ...ParamTypes>
struct member_pointer_traits_imp<RetType (Class::*)(ParamTypes... args, ...) const volatile&&, true, false>
{
   using ClassType = Class const volatile&&;
   using ReturnType = RetType;
   using FuncType = RetType (&) (ParamTypes... args, ...);
};

template <typename UnderType, typename Class>
struct member_pointer_traits_imp<UnderType Class::*, false, true>
{
   using ClassType = Class;
   using ReturnType = UnderType;
};

template <typename DecayedFuncType>
struct member_pointer_class_type 
{};

template <typename ReturnType, typename ClassType>
struct member_pointer_class_type<ReturnType ClassType::*>
{
   using type = ClassType;
};

template <typename TargetType>
struct is_reference_wrapper_impl : public std::false_type 
{};

template <typename TargetType>
struct is_reference_wrapper_impl<std::reference_wrapper<TargetType>> : public std::true_type
{};

template <typename TargetType>
struct is_reference_wrapper
      : public is_reference_wrapper_impl<typename std::remove_cv<TargetType>::type>
{};

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet1 = typename std::enable_if
<
zapi::stdext::CallableInfoTrait<DecayFuncType>::isMemberCallable &&
std::is_base_of<ClassType, DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type>
using enable_if_bullet2 = typename std::enable_if
<
zapi::stdext::CallableInfoTrait<DecayFuncType>::isMemberCallable &&
is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet3 = typename std::enable_if
<
zapi::stdext::CallableInfoTrait<DecayFuncType>::isMemberCallable &&
!std::is_base_of<ClassType, DecayA0>::value &&
!is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet4 = typename std::enable_if
<
std::is_member_object_pointer<DecayFuncType>::value &&
std::is_base_of<ClassType, DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type>
using enable_if_bullet5 = typename std::enable_if
<
std::is_member_object_pointer<DecayFuncType>::value &&
is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename std::decay<FuncType>::type,
          typename DecayA0 = typename std::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet6 = typename std::enable_if
<
std::is_member_object_pointer<DecayFuncType>::value &&
!std::is_base_of<ClassType, DecayA0>::value &&
!is_reference_wrapper<DecayA0>::value
>::type;

struct any
{
   any(...);
};

struct nat
{
   nat() = delete;
   nat(const nat&) = delete;
   nat& operator=(const nat&) = delete;
   ~nat() = delete;
};

template <typename ...ArgTypes>
auto invoke(any, ArgTypes&&... args) -> nat;

template <typename ...ArgTypes>
auto invoke_constexpr(any, ArgTypes&&... args) -> nat;

// bullets 1, 2 and 3
template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet1<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...)))
-> decltype((std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...))
{
   return (std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes, 
          typename = enable_if_bullet1<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...)))
-> decltype((std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...))
{
   return (std::forward<A0>(a0).*func)(std::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet2<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((a0.get().*func)(std::forward<ArgTypes>(args)...)))
-> decltype((a0.get().*func)(std::forward<ArgTypes>(args)...))
{
   return (a0.get().*func)(std::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet2<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((a0.get().*func)(std::forward<ArgTypes>(args)...)))
-> decltype((a0.get().*func)(std::forward<ArgTypes>(args)...))
{
   return (a0.get().*func)(std::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet3<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept(((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...)))
-> decltype(((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...))
{
   return ((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet3<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept(((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...)))
-> decltype(((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...))
{
   return ((*std::forward<A0>(a0)).*func)(std::forward<ArgTypes>(args)...);
}

// bullets 4, 5 and 6
template <typename FuncType, typename A0,
          typename = enable_if_bullet4<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0)
noexcept(noexcept(std::forward<A0>(a0).*func))
-> decltype(std::forward<A0>(a0).*func)
{
   return std::forward<A0>(a0).*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet4<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0)
noexcept(noexcept(std::forward<A0>(a0).*func))
-> decltype(std::forward<A0>(a0).*func)
{
   return std::forward<A0>(a0).*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet5<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0)
noexcept(noexcept(a0.get().*func))
-> decltype(a0.get().*func)
{
   return a0.get().*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet5<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0)
noexcept(noexcept(a0.get().*func))
-> decltype(a0.get().*func)
{
   return a0.get().*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet6<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0)
noexcept(noexcept((*std::forward<A0>(a0)).*func))
-> decltype((*std::forward<A0>(a0)).*func)
{
   return (*std::forward<A0>(a0)).*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet6<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0)
noexcept(noexcept((*std::forward<A0>(a0)).*func))
-> decltype((*std::forward<A0>(a0)).*func)
{
   return (*std::forward<A0>(a0)).*func;
}

template <typename FuncType, typename ...Args>
inline auto invoke(FuncType &&func, Args && ...args)
noexcept(noexcept(std::forward<FuncType>(func)(std::forward<Args>(args)...)))
-> decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...))
{
   return std::forward<FuncType>(func)(std::forward<Args>(args)...);
}

template <typename FuncType, typename ...Args>
inline constexpr auto invoke_constexpr(FuncType &&func, Args && ...args)
noexcept(noexcept(std::forward<FuncType>(func)(std::forward<Args>(args)...)))
-> decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...))
{
   return std::forward<FuncType>(func)(std::forward<Args>(args)...);
}

} // internal

template <typename CallableType>
struct CallableHasReturn
{
   constexpr const static bool value = !std::is_same<typename CallableInfoTrait<CallableType>::ReturnType, void>::value;
};

template <typename CallableType>
struct CallableHasNoReturn
{
   constexpr const static bool value = std::is_same<typename CallableInfoTrait<CallableType>::ReturnType, void>::value;
};

template<typename TargetType>
struct is_function_pointer
{
   static const bool value = std::is_pointer<TargetType>::value ?
            std::is_function<typename std::remove_pointer<TargetType>::type>::value :
            false;
};

template <class TypePointer> 
struct IsMemberObjectPointer
      : public std::integral_constant<bool, std::is_member_pointer<TypePointer>::value &&
      !CallableInfoTrait<TypePointer>::isMemberCallable> 
{};

template <typename MemberPointer>
struct member_pointer_traits 
      : public internal::member_pointer_traits_imp<typename std::remove_cv<MemberPointer>::type,
      zapi::stdext::CallableInfoTrait<MemberPointer>::isMemberCallable,
      zapi::stdext::IsMemberObjectPointer<MemberPointer>::value>
{};

} // stdext
} // zapi

#endif // ZAPI_STDEXT_TYPETRAITS_H
