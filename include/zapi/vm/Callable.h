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
#include "zapi/lang/Variant.h"

namespace zapi
{

namespace lang
{

// forward declare
class Parameters;

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

namespace internal
{

class CallablePrivate;

} // internal

using internal::CallablePrivate;

class ZAPI_DECL_EXPORT Callable
{
public:
   Callable(const char *name, zapi::ZendCallable callable, const Arguments &arguments = {});
   Callable(const Callable &other);
   Callable(Callable &&other);
   virtual ~Callable();
public:
   Callable &operator=(const Callable &other);
   Callable &operator=(Callable &&other);
public:
   virtual Variant invoke(Parameters &parameters) = 0;
   void initialize(zend_function_entry *entry, const char *className = nullptr, int flags = 0) const;
   void initialize(zend_internal_function_info *info, const char *className = nullptr) const;
   void initialize(const std::string &prefix, zend_function_entry *entry);
protected:
   Callable(CallablePrivate &implPtr);
   void setupCallableArgInfo(zend_internal_arg_info *info, const Argument &arg) const;
   static void invoke(INTERNAL_FUNCTION_PARAMETERS);
protected:
   ZAPI_DECLARE_PRIVATE(Callable)
   std::unique_ptr<CallablePrivate> m_implPtr;
};

} // vm
} // zapi

#endif // ZAPI_VM_CALLABLE_H