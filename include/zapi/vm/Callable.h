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
// Created by zzu_softboy on 26/07/2017.

#ifndef ZAPI_VM_CALLABLE_H
#define ZAPI_VM_CALLABLE_H

#include <string>
#include "zapi/Global.h"
#include "zapi/lang/Argument.h"

// forward declare with namespace
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

namespace vm
{

namespace internal
{
class CallablePrivate;
} // internal
} //vm

} // zapi
// end forward declare

namespace zapi
{
namespace vm
{

using zapi::lang::Arguments;
using zapi::lang::Argument;
using zapi::lang::Type;
using zapi::lang::Parameters;
using zapi::ds::Variant;
using internal::CallablePrivate;
using zapi::lang::internal::ExtensionPrivate;

class ZAPI_DECL_EXPORT Callable
{
public:
   Callable();
   Callable(const char *name, zapi::ZendCallable callable, const Arguments &arguments = {});
   Callable(const char *name, const Arguments &arguments = {});
   Callable(const Callable &other);
   Callable(Callable &&other) ZAPI_DECL_NOEXCEPT;
   virtual ~Callable();
public:
   Callable &operator=(const Callable &other);
   Callable &operator=(Callable &&other) ZAPI_DECL_NOEXCEPT;
   Callable &setReturnType(Type type) ZAPI_DECL_NOEXCEPT;
   Callable &setReturnType(const std::string &clsName) ZAPI_DECL_NOEXCEPT;
   Callable &setReturnType(const char *clsName) ZAPI_DECL_NOEXCEPT;
public:
   virtual Variant invoke(Parameters &parameters) = 0;
protected:
   Callable(CallablePrivate *implPtr);
   void setupCallableArgInfo(zend_internal_arg_info *info, const Argument &arg) const;
   static void invoke(INTERNAL_FUNCTION_PARAMETERS);
   void initialize(zend_function_entry *entry, const char *className = nullptr, int flags = 0) const;
   void initialize(zend_internal_function_info *info, const char *className = nullptr) const;
   void initialize(const std::string &prefix, zend_function_entry *entry);
protected:
   ZAPI_DECLARE_PRIVATE(Callable)
   std::shared_ptr<CallablePrivate> m_implPtr;
   friend class ExtensionPrivate;
};

} // vm
} // zapi

#endif // ZAPI_VM_CALLABLE_H
