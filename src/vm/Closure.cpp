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
// Created by zzu_softboy on 2017/09/11.

#include "zapi/vm/Closure.h"
#include "zapi/vm/AbstractClass.h"
#include "zapi/lang/Class.h"

namespace zapi
{
namespace vm
{

using zapi::lang::Class;
using zapi::lang::ClassType;

zend_class_entry *Closure::m_entry = nullptr;

Closure::Closure(const ClosureCallableType &callable)
   : m_callable(callable)
{}

Variant Closure::__invoke(Parameters &params) const
{
   return m_callable(params);
}

void Closure::registerToZendNg(int moduleNumber)
{
   // here we register ourself to zend engine
   if (m_entry) {
      return;
   }
   // @mark we save meta class as local static is really ok ?
   static std::unique_ptr<AbstractClass> closureWrapper(new Class<Closure>("ZapiClosure", ClassType::Final));
   m_entry = closureWrapper->initialize(moduleNumber);
}

void Closure::unregisterFromZendNg()
{
   m_entry = nullptr;
}

zend_class_entry *Closure::getClassEntry()
{
   return m_entry;
}

Closure::~Closure()
{}

} // vm
} // zapi
