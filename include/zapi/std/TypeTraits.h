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

namespace zapi
{
namespace std
{

template <typename SourceType, typename TargetType, 
          bool = std::is_const<typename std::remove_reference<TargetType>::type>::value,
          bool = std::is_volatile<typename std::remove_reference<TargetType>::type>::value>
struct apply_cv
{
   using type = SourceType;
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

} // std
} // zapi

#endif // ZAPI_STD_TYPETRAITS_H