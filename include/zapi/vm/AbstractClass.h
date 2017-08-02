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
// Created by zzu_softboy on 08/06/2017.

#ifndef ZAPI_VM_STDCLASSIMPL_H
#define ZAPI_VM_STDCLASSIMPL_H

#include "zapi/Global.h"

#include "php/Zend/zend_types.h"
#include "php/Zend/zend_alloc.h"
#include "php/Zend/zend_objects_API.h"

namespace zapi
{

namespace lang
{
class StdClass;
} // lang

namespace vm
{

namespace internal
{
class AbstractClassPrivate;
} // internal

using zapi::vm::internal::AbstractClassPrivate;
using zapi::lang::Modifier;
using zapi::lang::ClassType;
using zapi::lang::StdClass;

class ZAPI_DECL_EXPORT AbstractClass
{
public:
   AbstractClass(const char *className, Modifier flags);
   AbstractClass(const char *className, ClassType type);
   AbstractClass(const AbstractClass &other);
   AbstractClass(AbstractClass &&other);
   AbstractClass &operator=(const AbstractClass &other);
   AbstractClass &operator=(AbstractClass &&other);
   virtual ~AbstractClass();
public:
   virtual StdClass *construct() const;
   virtual StdClass *clone() const;
private:
   ZAPI_DECLARE_PRIVATE(AbstractClass)
   std::shared_ptr<AbstractClassPrivate> m_implPtr;
};

} // vm
} // zapi

#endif // ZAPI_VM_STDCLASSIMPL_H
