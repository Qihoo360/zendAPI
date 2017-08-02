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

#include "zapi/lang/StringMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace lang
{

using zapi::vm::internal::AbstractMemberPrivate;

namespace internal
{

class StringMemberPrivate : public AbstractMemberPrivate
{
public:
   StringMemberPrivate(const StringMemberPrivate &other) = default;
   StringMemberPrivate(const char *name, const char *value, size_t size, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value, size)
   {}
   StringMemberPrivate(const char *name, const char *value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value, std::strlen(value))
   {}
   StringMemberPrivate(const char *name, const std::string &value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value)
   {}
   std::string m_value;
};

} // internal

using zapi::vm::AbstractMember;
using zapi::lang::internal::StringMemberPrivate;

StringMember::StringMember(const char *name, const char *value, size_t size, Modifier flags)
   : AbstractMember(new StringMemberPrivate(name, value, size, flags))
{}

StringMember::StringMember(const char *name, const char *value, Modifier flags)
   : AbstractMember(new StringMemberPrivate(name, value, flags))
{}

StringMember::StringMember(const char *name, const std::string &value, Modifier flags)
   : AbstractMember(new StringMemberPrivate(name, value, flags))
{}

StringMember::StringMember(const StringMember &other)
{
   m_implPtr.reset(new StringMemberPrivate(*static_cast<StringMemberPrivate *>(other.m_implPtr.get())));
}

StringMember &StringMember::operator=(const StringMember &other)
{
   m_implPtr.reset(new StringMemberPrivate(*static_cast<StringMemberPrivate *>(other.m_implPtr.get())));
   return *this;
}

StringMember::~StringMember()
{}

void StringMember::setupConstant(zend_class_entry *entry)
{
   ZAPI_D(StringMember);
   zend_declare_class_constant_stringl(entry, implPtr->m_name.c_str(), implPtr->m_name.size(), 
                                       implPtr->m_value.c_str(), implPtr->m_value.size());
}

void StringMember::declare(zend_class_entry *entry)
{
   ZAPI_D(StringMember);
   zend_declare_property_stringl(entry, implPtr->m_name.c_str(), implPtr->m_name.size(), 
                         implPtr->m_value.c_str(), implPtr->m_value.size(), static_cast<int>(implPtr->m_flags));
}

} // lang
} // zapi