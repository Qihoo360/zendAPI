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

#ifndef ZAPI_STDEXT_TUPLE_PRIVATE_H
#define ZAPI_STDEXT_TUPLE_PRIVATE_H

#include "zapi/stdext/internal/TuplePrivate.h"
#include <tuple>

namespace zapi
{
namespace stdext
{

template <typename FuncType, class Tuple>
inline constexpr auto apply(FuncType &&func, Tuple &&tuple)
noexcept(noexcept(internal::apply_tuple_impl(
                     langstd::forward<FuncType>(func), langstd::forward<Tuple>(tuple),
                     typename internal::make_tuple_indices<langstd::tuple_size<typename langstd::decay<Tuple>::type>::value>::type {})))
-> decltype(internal::apply_tuple_impl(
               langstd::forward<FuncType>(func), langstd::forward<Tuple>(tuple),
               typename internal::make_tuple_indices<langstd::tuple_size<typename langstd::decay<Tuple>::type>::value>::type {}))
{
   return internal::apply_tuple_impl(
            langstd::forward<FuncType>(func), langstd::forward<Tuple>(tuple),
            typename internal::make_tuple_indices<langstd::tuple_size<typename langstd::decay<Tuple>::type>::value>::type {});
}

} // stdext
} // zapi

#endif // ZAPI_STDEXT_TUPLE_PRIVATE_H