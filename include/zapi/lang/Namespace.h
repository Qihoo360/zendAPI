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
#include "zapi/lang/Class.h"
#include "zapi/lang/internal/NamespacePrivate.h"

// forward declare with namespace
namespace zapi
{

namespace lang
{
namespace internal
{
class ExtensionPrivate;
} // internal
} // lang

} // zapi
// end forward declare

namespace zapi
{
namespace lang
{

class Constant;

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
   template <typename CallableType, typename std::decay<CallableType>::type callable>
   Namespace &registerFunction(const char *name, const Arguments &args = {});
   
   Namespace &registerNamespace(const Namespace &ns);
   Namespace &registerNamespace(Namespace &&ns);
   Namespace &registerConstant(const Constant &constant);
   Namespace &registerConstant(Constant &&constant);
   
   template <typename T>
   Namespace &registerClass(const Class<T> &nativeClass);
   template <typename T>
   Namespace &registerClass(Class<T> &&nativeClass);

   size_t getFunctionQuantity() const;
   size_t getConstantQuanlity() const;
   size_t getClassQuanlity() const;
protected:
   Namespace(NamespacePrivate *implPtr);
   Namespace &registerFunction(const char *name, zapi::ZendCallable function, const Arguments &arguments = {});
   void initialize(int moduleNumber);
   
private:
   ZAPI_DECLARE_PRIVATE(Namespace)
   std::shared_ptr<NamespacePrivate> m_implPtr;
   friend class zapi::lang::internal::ExtensionPrivate;
};

template <typename T>
Namespace &Namespace::registerClass(const Class<T> &nativeClass)
{
   ZAPI_D(Namespace);
   implPtr->m_classes.push_back(std::shared_ptr<AbstractClass>(new Class<T>(nativeClass)));
   return *this;
}

template <typename T>
Namespace &Namespace::registerClass(Class<T> &&nativeClass)
{
   ZAPI_D(Namespace);
   implPtr->m_classes.push_back(std::shared_ptr<AbstractClass>(new Class<T>(std::move(nativeClass))));
   return *this;
}

template <typename CallableType, typename std::decay<CallableType>::type callable>
Namespace &Namespace::registerFunction(const char *name, const Arguments &args)
{
   return registerFunction(name, &InvokeBridge<CallableType, callable>::invoke, args);
}

} // lang
} // zapi

#endif // ZAPI_LANG_NAMESPACE_H
