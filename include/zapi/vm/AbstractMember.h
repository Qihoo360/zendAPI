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
// Created by zzu_softboy on 2017/08/02.

#ifndef ZAPI_VM_ABSTRACT_MEMBER_H
#define ZAPI_VM_ABSTRACT_MEMBER_H

#include "zapi/Global.h"

namespace zapi
{
namespace vm
{
namespace internal
{
class AbstractMemberPrivate;
class AbstractClassPrivate;
} // internal

using zapi::lang::Modifier;
using zapi::vm::internal::AbstractMemberPrivate;

class AbstractMember
{
public:
   AbstractMember();
   AbstractMember(const char *name, Modifier flags);
   AbstractMember(const AbstractMember &other);
   AbstractMember(AbstractMember &&other) ZAPI_DECL_EXPORT;
   virtual ~AbstractMember();
   AbstractMember &operator=(const AbstractMember &other);
   AbstractMember &operator=(AbstractMember &&other) ZAPI_DECL_EXPORT;
protected:
   AbstractMember(AbstractMemberPrivate *implPtr);
   void initialize(zend_class_entry *entry);
   virtual void setupConstant(zend_class_entry *entry) = 0;
   virtual void setupProperty(zend_class_entry *entry) = 0;
   
   ZAPI_DECLARE_PRIVATE(AbstractMember)
   std::unique_ptr<AbstractMemberPrivate> m_implPtr;
   friend class zapi::vm::internal::AbstractClassPrivate;
};

} // vm
} // zapi

#endif // ZAPI_VM_ABSTRACT_MEMBER_H