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
// Created by softboy on 2017/07/27.

#include "zapi/lang/Function.h"
#include "zapi/vm/internal/CallablePrivate.h"

namespace zapi
{
namespace lang
{
using zapi::vm::internal::CallablePrivate;

class FunctionPrivate : public CallablePrivate
{
public:
   using CallablePrivate::CallablePrivate;
};

Function::Function(const char *name, zapi::ZendCallable callable, const Arguments &arguments)
   : Callable(new FunctionPrivate(name, callable, arguments))
{}

Function::Function(const char *name, const Arguments &arguments)
   : Callable(new FunctionPrivate(name, nullptr, arguments))
{}

Function::Function(const Function &other)
   : Callable(other)
{
}

Function &Function::operator=(const Function &other)
{
   if (this != &other) {
      Callable::operator=(other);
   }
   return *this;
}

Variant Function::invoke(Parameters &parameters)
{
   // now we just do nothing
   return nullptr;
}

Function::~Function()
{}

} // lang
} // zapi