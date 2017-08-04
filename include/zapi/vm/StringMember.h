// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// Copyright 2013, 2014 Copernica BV
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
// Created by softboy on 2017/08/02.

#ifndef ZAPI_LANG_STRING_MEMBER_H
#define ZAPI_LANG_STRING_MEMBER_H

#include "zapi/Global.h"
#include "zapi/vm/AbstractMember.h"

namespace zapi
{
namespace vm
{

namespace internal
{
class StringMemberPrivate;
} // internal

using zapi::vm::AbstractMember;
using zapi::vm::internal::StringMemberPrivate;

class ZAPI_DECL_EXPORT StringMember : public AbstractMember
{
public:
   StringMember(const char *name, const char *value, size_t size, Modifier flags);
   StringMember(const char *name, const char *value, Modifier flags);
   StringMember(const char *name, const std::string &value, Modifier flags);
   StringMember(const StringMember &other);
   StringMember &operator=(const StringMember &other);
   virtual ~StringMember();
protected:
   virtual void setupConstant(zend_class_entry *entry) override;
   virtual void setupProperty(zend_class_entry *entry) override;
   ZAPI_DECLARE_PRIVATE(StringMember)
};

} // vm
} // zapi

#endif // ZAPI_LANG_STRING_MEMBER_H