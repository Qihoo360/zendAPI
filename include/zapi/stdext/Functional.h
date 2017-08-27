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

#ifndef ZAPI_STDEXT_FUNCTIONAL_H
#define ZAPI_STDEXT_FUNCTIONAL_H

#include "zapi/stdext/internal/FunctionalPrivate.h"

namespace langstd = std;

namespace zapi
{
namespace stdext
{

template<typename FuncType, typename ...ArgTypes>
typename langstd::result_of<FuncType&& (ArgTypes&&...)>::type
invoke(FuncType&& func, ArgTypes&& ...args)
noexcept(noexcept(internal::invoke(langstd::forward<FuncType>(func), langstd::forward<ArgTypes>(args)...)))
{
   return internal::invoke(langstd::forward<FuncType>(func), langstd::forward<ArgTypes>(args)...);
}

} // stdext
} // zapi

#endif // ZAPI_STDEXT_FUNCTIONAL_H