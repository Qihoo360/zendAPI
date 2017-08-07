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
// Created by softboy on 2017/08/01.

#include <list>
#include <string>
#include "zapi/lang/Namespace.h"
#include "zapi/lang/internal/NamespacePrivate.h"
#include "zapi/lang/Constant.h"
#include "zapi/lang/Function.h"

namespace zapi
{
namespace lang
{

using zapi::lang::Constant;
using zapi::lang::Function;
using zapi::lang::internal::NamespacePrivate;

namespace internal
{

void NamespacePrivate::initialize(const std::string &ns)
{
   initializeConstants(ns);
   initializeClasses(ns);
   // recursive initialize
   for (std::shared_ptr<Namespace> &subns : m_namespaces) {
      subns->m_implPtr->initialize(ns + "\\" + subns->m_implPtr->m_name);
   }
}

void NamespacePrivate::iterateFunctions(const std::function<void(const std::string &ns, Function &func)> &callback)
{
   for (std::shared_ptr<Function> &func : m_functions) {
      callback(m_name, *func);
   }
   for (std::shared_ptr<Namespace> &subns : m_namespaces) {
      subns->m_implPtr->iterateFunctions([this, callback](const std::string &ns, Function &func){
         callback(m_name + "\\" + ns, func);
      });
   }
}

void NamespacePrivate::initializeConstants(const std::string &ns)
{
   
}

void NamespacePrivate::initializeClasses(const std::string &ns)
{
   
}

size_t NamespacePrivate::calculateFunctionQuantity() const
{
   size_t ret = m_functions.size();
   for (const std::shared_ptr<Namespace> &ns : m_namespaces) {
      ret += ns->m_implPtr->calculateFunctionQuantity();
   }
   return ret;
}

} // internal

Namespace::Namespace(const std::string &name)
   : m_implPtr(new NamespacePrivate(name))
{}

Namespace::Namespace(const char *name)
   : m_implPtr(new NamespacePrivate(name))
{}

Namespace::Namespace(NamespacePrivate *implPtr)
   : m_implPtr(implPtr)
{}

Namespace::Namespace(const Namespace &other)
   : m_implPtr(other.m_implPtr)
{}

Namespace::Namespace(Namespace &&other) ZAPI_DECL_NOEXCEPT
   : m_implPtr(std::move(other.m_implPtr))
{}

Namespace &Namespace::operator=(const Namespace &other)
{
   m_implPtr = other.m_implPtr;
   return *this;
}

Namespace &Namespace::operator=(Namespace &&other) ZAPI_DECL_NOEXCEPT
{
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

void Namespace::initialize()
{
   ZAPI_D(Namespace);
   implPtr->initialize(implPtr->m_name);
}

Namespace &Namespace::registerFunction(const char *name, zapi::ZendCallable function, const Arguments &arguments)
{
   ZAPI_D(Namespace);
   implPtr->m_functions.push_back(std::make_shared<Function>(name, function, arguments));
   return *this;
}

size_t Namespace::getFunctionQuantity() const
{
   ZAPI_D(const Namespace);
   return implPtr->calculateFunctionQuantity();
}

Namespace &Namespace::registerNamespace(const Namespace &ns)
{
   ZAPI_D(Namespace);
   implPtr->m_namespaces.push_back(std::make_shared<Namespace>(ns));
   return *this;
}

Namespace &Namespace::registerNamespace(Namespace &&ns)
{
    ZAPI_D(Namespace);
    implPtr->m_namespaces.push_back(std::make_shared<Namespace>(std::move(ns)));
    return *this;
}

Namespace::~Namespace()
{}

} // zapi
} // zapi