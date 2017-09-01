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
// Created by zzu_softboy on 2017/09/01.

#ifndef ZAPI_VM_ZVAL_MEMBER_H
#define ZAPI_VM_ZVAL_MEMBER_H

#include "zapi/vm/AbstractMember.h"

namespace zapi
{
namespace vm 
{

namespace internal
{
class ZvalMemberPrivate;
} // internal

using zapi::vm::AbstractMember;
using zapi::vm::internal::ZvalMemberPrivate;

class ZAPI_DECL_EXPORT ZvalMember : public AbstractMember
{
public:
   ZvalMember(const char *name,  zval *value, Modifier flags);
   ZvalMember(const ZvalMember &other);
   ZvalMember &operator=(const ZvalMember &other);
   virtual ~ZvalMember();
protected:
   virtual void setupConstant(zend_class_entry *entry) override;
   virtual void setupProperty(zend_class_entry *entry) override;
   ZAPI_DECLARE_PRIVATE(ZvalMember)
};


} // vm
} // zapi

#endif // ZAPI_VM_ZVAL_MEMBER_H