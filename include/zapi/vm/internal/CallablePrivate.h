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
// Created by zzu_softboy on 27/07/2017.

#ifndef ZAPI_VM_INTERNAL_CALLABLE_PRIVATE_H
#define ZAPI_VM_INTERNAL_CALLABLE_PRIVATE_H

#include "zapi/Global.h"
#include "zapi/lang/Argument.h"

namespace zapi
{
namespace lang
{
class Argument;
} // lang

namespace vm
{
namespace internal
{

using zapi::lang::Type;
using zapi::lang::Argument;
using zapi::lang::Arguments;

class CallablePrivate
{
public:
   CallablePrivate(const char *name, ZendCallable callable, const Arguments &arguments);
   CallablePrivate(const CallablePrivate &other);
   CallablePrivate(CallablePrivate &&other);
   void setupCallableArgInfo(zend_internal_arg_info *info, const Argument &arg) const;
   void initialize(zend_function_entry *entry, const char *className = nullptr, int flags = 0) const;
   void initialize(zend_internal_function_info *info, const char *className = nullptr) const;
   void initialize(const std::string &prefix, zend_function_entry *entry);
   CallablePrivate &operator=(const CallablePrivate &other);
   CallablePrivate &operator=(CallablePrivate &&other);
   zapi::ZendCallable m_callable;
   std::string m_name;
   Type m_return = Type::Undefined;
   uint32_t m_required = 0;
   int m_argc = 0;
   std::unique_ptr<zend_internal_arg_info[]> m_argv;
};

} // internal
} // vm
} // zapi

#endif // ZAPI_VM_INTERNAL_CALLABLE_PRIVATE_H