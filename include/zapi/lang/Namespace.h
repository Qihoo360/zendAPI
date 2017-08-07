// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by softboy on 18/05/2017.
//


#ifndef ZAPI_LANG_NAMESPACE_H
#define ZAPI_LANG_NAMESPACE_H

#include "zapi/Global.h"
#include "zapi/vm/InvokeBridge.h"

namespace zapi
{

// forward declare with namespace
namespace bridge
{
namespace internal
{
class ExtensionPrivate;
} // internal
} // vm

namespace lang
{

namespace internal
{
class NamespacePrivate;
} // internal

using internal::NamespacePrivate;
using zapi::vm::InvokeBridge;

class ZAPI_DECL_EXPORT Namespace final
{
public:
   Namespace(const std::string &name);
   Namespace(const char *name);
   Namespace(const Namespace &other);
   Namespace(Namespace &&other) ZAPI_DECL_NOEXCEPT;
   Namespace &operator=(const Namespace &other);
   Namespace &operator=(Namespace &&other) ZAPI_DECL_NOEXCEPT;
   virtual ~Namespace();
public:
   void registerConstant();
   template <void (*func)()>
   Namespace &registerFunction(const char *name, const Arguments &arguments = {})
   {
      return registerFunction(name, &InvokeBridge::invoke<func>, arguments);
   }
   
   template <void (*func)(Parameters &parameters)>
   Namespace &registerFunction(const char *name, const Arguments &arguments = {})
   {
      return registerFunction(name, &InvokeBridge::invoke<func>, arguments);
   }
   
   template <Variant (*func)()>
   Namespace &registerFunction(const char *name, const Arguments &arguments = {})
   {
      return registerFunction(name, &InvokeBridge::invoke<func>, arguments);
   }
   
   template <Variant (*func)(Parameters &parameters)>
   Namespace &registerFunction(const char *name, const Arguments &arguments = {})
   {
      return registerFunction(name, &InvokeBridge::invoke<func>, arguments);
   }
   
   Namespace &registerNamespace(const Namespace &ns);
   Namespace &registerNamespace(Namespace &&ns);
   size_t getFunctionQuantity() const;
protected:
   Namespace(NamespacePrivate *implPtr);
   Namespace &registerFunction(const char *name, zapi::ZendCallable function, const Arguments &arguments = {});
   void initialize();
   
private:
   ZAPI_DECLARE_PRIVATE(Namespace)
   std::shared_ptr<NamespacePrivate> m_implPtr;
   friend class zapi::bridge::internal::ExtensionPrivate;
};

} // lang
} // zapi

#endif // ZAPI_LANG_NAMESPACE_H
