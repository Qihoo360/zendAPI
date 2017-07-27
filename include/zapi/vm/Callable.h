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
// Created by zzu_softboy on 26/07/2017.

#ifndef ZAPI_VM_CALLABLE_H
#define ZAPI_VM_CALLABLE_H

#include <string>
#include "zapi/Global.h"
#include "zapi/lang/Argument.h"

namespace zapi
{

namespace lang
{

// forward declare
class Parameters;
class Variant;

} // lang

namespace vm
{

namespace
{

using zapi::lang::Arguments;
using zapi::lang::Argument;
using zapi::lang::Type;
using zapi::lang::Variant;
using zapi::lang::Parameters;

}

class Callable
{
public:
   Callable(zapi::ZendCallable callable, const char *name, const Arguments &arguments = {});
   Callable(const Callable &other);
   Callable(Callable &&other)
      : m_name(std::move(other.m_name)),
        m_return(other.m_return),
        m_required(other.m_required),
        m_argc(other.m_argc),
        m_argv(std::move(other.m_argv))
   {}
   virtual ~Callable() = default;
   
public:
   virtual Variant invoke(Parameters &parameters) = 0;
   void initialize(zend_function_entry *entry, const char *className = nullptr, int flags = 0) const;
   void initialize(zend_internal_function_info *info, const char *className = nullptr) const;
protected:
   void setupCallableArgInfo(zend_internal_arg_info *info, const Argument &arg) const;
   static void invoke(INTERNAL_FUNCTION_PARAMETERS);
protected:
   zapi::ZendCallable m_callable;
   std::string m_name;
   Type m_return = Type::Undefined;
   uint32_t m_required = 0;
   int m_argc = 0;
   std::unique_ptr<zend_internal_arg_info[]> m_argv;
};

} // vm
} // zapi

#endif // ZAPI_VM_CALLABLE_H