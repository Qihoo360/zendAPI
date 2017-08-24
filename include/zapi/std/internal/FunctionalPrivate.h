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

template <typename RetType, typename T1, bool IsFunc, bool IsBase>
struct enable_invoke_impl;

template <typename RetType, typename T1>
struct enable_invoke_impl <RetType, T1, true, true>
{
   using Bullet1 = RetType;
   using Type = Bullet1;
};

template <typename RetType, typename T1>
struct enable_invoke_impl<RetType, T1, true, false>
{
   using Bullet2 = RetType;
   using Type = Bullet2;
};

template <typename RetType, typename T1>
struct enable_invoke_impl<RetType, T1, false, true>
{
   using Bullet3 = typename langstd::add_lvalue_reference<
      zapi::std::apply_cv<T1, RetType>::type
   >::type;
   using type = Bullet3;
};

template <typename RetType, typename T1>
struct enable_invoke_impl<RetType, T1, false, false>
{
   using Bullet4 = typename langstd::add_lvalue_reference<
      zapi::std::apply_cv<decltype(*langstd::declval<T1>()), RetType>::type
   >::type;
   using type = Bullet4;
};

template <typename RetType, typename T1>
struct enable_invoke_impl<RetType, T1 *, false, false>
{
   using Bullet4 = typename langstd::add_lvalue_reference<
      zapi::std::apply_cv<T1, RetType>::type
   >::type;
   using type = Bullet4;
};

} // internal
} // std
} // zapi

#endif // ZAPI_STD_INTERNAL_FUNCTIONAL_PRIVATE_H