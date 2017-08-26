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

#ifndef ZAPI_STD_TYPETRAITS_H
#define ZAPI_STD_TYPETRAITS_H

#include <type_traits>

namespace langstd = std;

namespace zapi
{
namespace std
{
namespace internal
{
template <typename SourceType, typename TargetType, 
          bool = langstd::is_const<typename langstd::remove_reference<TargetType>::type>::value,
          bool = langstd::is_volatile<typename langstd::remove_reference<TargetType>::type>::value>
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

template <typename MemberPointerType, bool IsMemberFunctionPtr, bool IsMemberObjectPtr>
struct member_pointer_traits_imp
{  // forward declaration; specializations later
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

template <typename MemberPointer>
struct member_pointer_traits 
      : public member_pointer_traits_imp<typename langstd::remove_cv<MemberPointer>::type,
      langstd::is_member_function_pointer<MemberPointer>::value,
      langstd::is_member_object_pointer<MemberPointer>::value>
{};

template <typename DecayedFuncType>
struct member_pointer_class_type 
{};

template <typename ReturnType, typename ClassType>
struct member_pointer_class_type<ReturnType ClassType::*>
{
   using type = ClassType;
};

template <typename TargetType>
struct is_reference_wrapper_impl : public langstd::false_type 
{};

template <typename TargetType>
struct is_reference_wrapper_impl<langstd::reference_wrapper<TargetType>> : public langstd::true_type
{};

template <typename TargetType>
struct is_reference_wrapper
      : public is_reference_wrapper_impl<typename langstd::remove_cv<TargetType>::type>
{};

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet1 = typename langstd::enable_if
<
langstd::is_member_function_pointer<DecayFuncType>::value &&
langstd::is_base_of<ClassType, DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type>
using enable_if_bullet2 = typename langstd::enable_if
<
langstd::is_member_function_pointer<DecayFuncType>::value &&
is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet3 = typename langstd::enable_if
<
langstd::is_member_function_pointer<DecayFuncType>::value &&
!langstd::is_base_of<ClassType, DecayA0>::value &&
!is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet4 = typename langstd::enable_if
<
langstd::is_member_object_pointer<DecayFuncType>::value &&
langstd::is_base_of<ClassType, DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type>
using enable_if_bullet5 = typename langstd::enable_if
<
langstd::is_member_object_pointer<DecayFuncType>::value &&
is_reference_wrapper<DecayA0>::value
>::type;

template <typename FuncType, typename A0,
          typename DecayFuncType = typename langstd::decay<FuncType>::type,
          typename DecayA0 = typename langstd::decay<A0>::type,
          typename ClassType = typename member_pointer_class_type<DecayFuncType>::type>
using enable_if_bullet6 = typename langstd::enable_if
<
langstd::is_member_object_pointer<DecayFuncType>::value &&
!langstd::is_base_of<ClassType, DecayA0>::value &&
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
noexcept(noexcept((langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype((langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...))
{
   return (langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes, 
          typename = enable_if_bullet1<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype((langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...))
{
   return (langstd::forward<A0>(a0).*func)(langstd::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet2<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((a0.get().*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype((a0.get().*func)(langstd::forward<ArgTypes>(args)...))
{
   return (a0.get().*func)(langstd::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet2<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept((a0.get().*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype((a0.get().*func)(langstd::forward<ArgTypes>(args)...))
{
   return (a0.get().*func)(langstd::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet3<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept(((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype(((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...))
{
   return ((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...);
}

template <typename FuncType, typename A0, typename ...ArgTypes,
          typename = enable_if_bullet3<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0, ArgTypes && ...args)
noexcept(noexcept(((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...)))
-> decltype(((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...))
{
   return ((*langstd::forward<A0>(a0)).*func)(langstd::forward<ArgTypes>(args)...);
}

// bullets 4, 5 and 6
template <typename FuncType, typename A0,
          typename = enable_if_bullet4<FuncType, A0>>
inline auto invoke(FuncType &&func, A0 &&a0)
noexcept(noexcept(langstd::forward<A0>(a0).*func))
-> decltype(langstd::forward<A0>(a0).*func)
{
   return langstd::forward<A0>(a0).*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet4<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0)
noexcept(noexcept(langstd::forward<A0>(a0).*func))
-> decltype(langstd::forward<A0>(a0).*func)
{
   return langstd::forward<A0>(a0).*func;
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
noexcept(noexcept((*langstd::forward<A0>(a0)).*func))
-> decltype((*langstd::forward<A0>(a0)).*func)
{
   return (*langstd::forward<A0>(a0)).*func;
}

template <typename FuncType, typename A0,
          typename = enable_if_bullet6<FuncType, A0>>
inline constexpr auto invoke_constexpr(FuncType &&func, A0 &&a0)
noexcept(noexcept((*langstd::forward<A0>(a0)).*func))
-> decltype((*langstd::forward<A0>(a0)).*func)
{
   return (*langstd::forward<A0>(a0)).*func;
}

template <typename FuncType, typename ...Args>
inline auto invoke(FuncType &&func, Args && ...args)
noexcept(noexcept(langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...)))
-> decltype(langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...))
{
   return langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...);
}

template <typename FuncType, typename ...Args>
inline constexpr auto invoke_constexpr(FuncType &&func, Args && ...args)
noexcept(noexcept(langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...)))
-> decltype(langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...))
{
   return langstd::forward<FuncType>(func)(langstd::forward<Args>(args)...);
}

} // internal

} // std
} // zapi

#endif // ZAPI_STD_TYPETRAITS_H