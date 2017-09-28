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
// Created by zzu_softboy on 2017/08/01.

#include "zapi/lang/Method.h"
#include "zapi/lang/Parameters.h"
#include "zapi/lang/StdClass.h"
#include "zapi/vm/internal/CallablePrivate.h"

namespace zapi
{
namespace lang
{

namespace internal
{

using zapi::vm::internal::CallablePrivate;
using zapi::lang::Modifier;
using zapi::lang::StdClass;

class MethodPrivate : public CallablePrivate
{
public:
   MethodPrivate(const char *name, ZendCallable callback, Modifier flags, const Arguments &args)
      : CallablePrivate(name, callback, args),
        m_flags(flags)
   {
   }
   
   MethodPrivate(const char *name, Modifier flags, const Arguments &args)
      : CallablePrivate(name, args),
        m_flags(flags)
   {
   }
   
   void initialize(zend_function_entry *entry, const std::string &className);
   Modifier m_flags;
};

void MethodPrivate::initialize(zend_function_entry *entry, const std::string &className)
{
   if ((m_flags & (Modifier::Public | Modifier::Private | Modifier::Protected)) == 0) {
      m_flags |= Modifier::Public;
   }
   CallablePrivate::initialize(entry, className.c_str(), static_cast<int>(m_flags));
}

} // internal

using internal::MethodPrivate;

Method::Method(const char *name, zapi::ZendCallable callback, Modifier flags, const Arguments &args)
   : Callable(new MethodPrivate(name, callback, flags, args))
{}

Method::Method(const char *name, Modifier flags, const Arguments &args)
   : Callable(new MethodPrivate(name, flags, args))
{}

Method::Method(const Method &other)
   : Callable(other)
{
}

Method &Method::operator=(const Method &other)
{
   if (this != &other) {
      Callable::operator=(other);
   }
   return *this;
}

Variant Method::invoke(Parameters &parameters)
{
   // now we just do nothing
   return nullptr;
}

Method::~Method()
{}

void Method::initialize(zend_function_entry *entry, const char *className)
{
   ZAPI_D(Method);
   implPtr->initialize(entry, className);
}

} // lang
} // zapi