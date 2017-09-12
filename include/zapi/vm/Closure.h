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
// Created by zzu_softboy on 2017/09/11.

#ifndef ZAPI_VM_CLOSURE_H
#define ZAPI_VM_CLOSURE_H

#include <functional>
#include "zapi/Global.h"
#include "zapi/lang/StdClass.h"
#include "zapi/stdext/TypeTraits.h"

// forward declare class with namespace
namespace zapi
{
namespace ds
{
class Variant;
} // ds

namespace lang
{
class Parameters;
namespace internal
{
class ExtensionPrivate;
} // internal
} // lang
} // zapi

namespace zapi
{
namespace vm
{

using zapi::ds::Variant;
using zapi::lang::internal::ExtensionPrivate;
using zapi::lang::Parameters;
using zapi::stdext::is_function_pointer;
using ClosureCallableType = std::function<Variant(Parameters &)>;

class Closure final : public zapi::lang::StdClass
{
public:
   Closure(const ClosureCallableType &callable);
   Variant __invoke(Parameters &params) const;
   virtual ~Closure();
   static zend_class_entry *getClassEntry();
private:
   static void registerToZendNg(int moduleNumber);
   static void unregisterFromZendNg();
private:
   friend class ExtensionPrivate;
   static zend_class_entry *m_entry;
   const ClosureCallableType m_callable;
};

} // vm
} // zapi

#endif // ZAPI_VM_CLOSURE_H
