// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// @copyright 2013, 2014 Copernica BV
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by softboy on 2017/08/07.


#ifndef ZAPI_LANG_INTERNAL_NAMESPACE_PRIVATE_H
#define ZAPI_LANG_INTERNAL_NAMESPACE_PRIVATE_H

#include "php/Zend/zend_modules.h"
#include "zapi/lang/Argument.h"
#include <list>
#include <string>

namespace zapi
{

// forward declare with namespace
namespace vm
{
class AbstractClass;
} // vm

namespace lang
{

// forward declare
class Function;
class Constant;
class Namespace;

namespace internal
{
using zapi::lang::Arguments;
using zapi::lang::Function;
using zapi::lang::Constant;
using zapi::lang::Namespace;
using zapi::vm::AbstractClass;

class NamespacePrivate
{
public:
   NamespacePrivate(const char *name)
      : NamespacePrivate(std::string(name))
   {
   }
   
   NamespacePrivate(const std::string &name)
      : m_name(name)
   {
       ZAPI_ASSERT_X(!name.empty(), "NamespacePrivate", "namespace name can not be empty");
   }
   
   void iterateFunctions(const std::function<void(const std::string &ns, Function &func)> &callback);
   void initialize(const std::string &ns, int moduleName);
   void initializeConstants(const std::string &ns, int moduleName);
   void initializeClasses(const std::string &ns, int moduleName);
   size_t calculateFunctionQuantity() const;
   size_t calculateClassQuantity() const;
   size_t calculateConstantQuantity() const;
public:
   std::string m_name;
   std::list<std::shared_ptr<Function>> m_functions;
   std::list<std::shared_ptr<AbstractClass>> m_classes;
   std::list<std::shared_ptr<Constant>> m_constants;
   std::list<std::shared_ptr<Namespace>> m_namespaces;
};

} // internal
} // lang
} // zapi

#endif // ZAPI_LANG_INTERNAL_NAMESPACE_PRIVATE_H