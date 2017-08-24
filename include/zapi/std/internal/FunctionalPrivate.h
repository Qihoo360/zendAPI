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
   
};

} // internal
} // std
} // zapi

#endif // ZAPI_STD_INTERNAL_FUNCTIONAL_PRIVATE_H