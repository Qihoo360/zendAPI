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
// Created by softboy on 2017/08/25.

#ifndef ZAPI_STDEXT_INTERNAL_TUPLE_PRIVATE_H
#define ZAPI_STDEXT_INTERNAL_TUPLE_PRIVATE_H

#include "zapi/stdext/TypeTraits.h"
#include <tuple>

namespace zapi
{
namespace stdext
{
namespace internal
{

template <size_t... seq> 
struct tuple_indices {};

template <typename IdxType, IdxType... Seq>
struct integer_sequence
{
   template <template <typename OIdxType, OIdxType...> class ToIndexSeq, typename ToIndexType>
   using convert = ToIndexSeq<ToIndexType, Seq...>;
   template <size_t SeqSize>
   using to_tuple_indices = tuple_indices<(Seq + SeqSize)...>;
};

template <typename TargetType, size_t ...Extra> 
struct repeat;
template<typename TargetType, TargetType ...Np, size_t ...Extra> 
struct repeat<integer_sequence<TargetType, Np...>, Extra...> 
{
   using type = integer_sequence<TargetType,
   Np...,
   sizeof...(Np) + Np...,
   2 * sizeof...(Np) + Np...,
   3 * sizeof...(Np) + Np...,
   4 * sizeof...(Np) + Np...,
   5 * sizeof...(Np) + Np...,
   6 * sizeof...(Np) + Np...,
   7 * sizeof...(Np) + Np...,
   Extra...>;
};

template <size_t Np> struct parity;

template <size_t Np> struct make : parity <Np % 8>::template pmake<Np>
{};

template<>
struct make<0>
{
   using type = integer_sequence<size_t>;
};

template<>
struct make<1>
{
   using type = integer_sequence<size_t, 0>;
};

template<>
struct make<2>
{
   using type = integer_sequence<size_t, 0, 1>;
};

template<>
struct make<3>
{
   using type = integer_sequence<size_t, 0, 1, 2>;
};

template<>
struct make<4>
{
   using type = integer_sequence<size_t, 0, 1, 2, 3>;
};

template<>
struct make<5>
{
   using type = integer_sequence<size_t, 0, 1, 2, 3, 4>;
};

template<>
struct make<6>
{
   using type = integer_sequence<size_t, 0, 1, 2, 3, 4, 5>;
};

template<>
struct make<7>
{
   using type = integer_sequence<size_t, 0, 1, 2, 3, 4, 5, 6>;
};

template<>
struct parity<0> 
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type> 
   {};
};

template<>
struct parity<1>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 1> 
   {};
}; 

template<>
struct parity<2>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 2, Np - 1> 
   {};
};

template<>
struct parity<3>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 3, Np - 2, Np - 1> 
   {};
};

template<>
struct parity<4>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 4, Np - 3, Np - 2, Np - 1> 
   {};
};

template<>
struct parity<5>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 5, Np - 4, Np - 3, Np - 2, Np - 1> 
   {};
};

template<>
struct parity<6>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 6, Np - 5, Np - 4, Np - 3, Np - 2, Np - 1> 
   {};
};

template<>
struct parity<7>
{
   template<size_t Np> 
   struct pmake : repeat<typename make<Np / 8>::type, Np - 7, Np - 6, Np - 5, Np - 4, Np - 3, Np - 2, Np - 1> 
   {};
};

template <size_t EndPoint, size_t StartPoint>
using make_indices_impl = 
typename make<EndPoint - StartPoint>::type::template to_tuple_indices<StartPoint>;

template <size_t EndPoint, size_t StartPoint = 0>
struct make_tuple_indices
{
   static_assert(StartPoint <= EndPoint, "zapi::std::internal::make_tuple_indices input error");
   using type = make_indices_impl<EndPoint, StartPoint>;
};

template <typename FuncType, typename Tuple, size_t ...Seq>
inline constexpr auto apply_tuple_impl(FuncType &&func, Tuple &&tuple, tuple_indices<Seq...>)
noexcept(noexcept(invoke_constexpr(std::forward<FuncType>(func),
                                   std::get<Seq>(std::forward<Tuple>(tuple))...)))
-> decltype(invoke_constexpr(std::forward<FuncType>(func),
                             std::get<Seq>(std::forward<Tuple>(tuple))...))
{
   return invoke_constexpr(std::forward<FuncType>(func),
                           std::get<Seq>(std::forward<Tuple>(tuple))...);
}

template <typename SeqItemType, SeqItemType Np>
using make_integer_sequence_unchecked = 
typename make<Np>::type::template convert<integer_sequence, SeqItemType>;

template <typename SeqItemType, SeqItemType Ep>
struct make_integer_sequence_checked
{
   static_assert(std::is_integral<SeqItemType>::value,
                 "zapi::stdext::make_integer_sequence can only be instantiated with an integral type" );
   static_assert(0 <= Ep, "zapi::stdext::make_integer_sequence must have a non-negative sequence length");
   // Workaround GCC bug by preventing bad installations when 0 <= _Ep
   // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=68929
   using type = make_integer_sequence_unchecked<SeqItemType, 0 <= Ep ? Ep : 0>;
};

template <typename SeqItemType, SeqItemType Ep>
using make_integer_sequence = typename make_integer_sequence_checked<SeqItemType, Ep>::type;

template<size_t... ItemSeq>
    using index_sequence = integer_sequence<size_t, ItemSeq...>;

template<size_t EndMark>
    using make_index_sequence = make_integer_sequence<size_t, EndMark>;


template <typename FuncType, size_t... Is>
auto gen_tuple_impl(FuncType func, index_sequence<Is...> )
-> decltype(std::make_tuple(func(Is)...))
{
   return std::make_tuple(func(Is)...);
}

template <typename CallableType, typename GeneratorType, size_t... Is>
auto gen_tuple_with_type_impl(GeneratorType generator, index_sequence<Is...> )
-> decltype(std::make_tuple(generator.template generate<typename zapi::stdext::CallableInfoTrait<CallableType>::template arg<Is>::type>(Is)...))
{
   return std::make_tuple(generator.template generate<typename zapi::stdext::CallableInfoTrait<CallableType>::template arg<Is>::type>(Is)...);
}

} // internal
} // stdext
} // zapi

#endif // ZAPI_STDEXT_INTERNAL_TUPLE_PRIVATE_H