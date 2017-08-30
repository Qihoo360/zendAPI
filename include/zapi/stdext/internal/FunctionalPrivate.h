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

#ifndef ZAPI_STDEXT_INTERNAL_FUNCTIONAL_PRIVATE_H
#define ZAPI_STDEXT_INTERNAL_FUNCTIONAL_PRIVATE_H

#include <type_traits>
#include <functional>
#include "zapi/stdext/TypeTraits.h"

namespace zapi
{
namespace stdext
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

} // internal
} // stdext
} // zapi

#endif // ZAPI_STDEXT_INTERNAL_FUNCTIONAL_PRIVATE_H