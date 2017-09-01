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

#include "zapi/vm/ZValMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm 
{

using zapi::vm::internal::AbstractMemberPrivate;

namespace internal
{

class ZvalMemberPrivate : public AbstractMemberPrivate
{
public:
   ZvalMemberPrivate(const char *name, zval *value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value)
   {}
   zval *m_value;
};

} // internal

using zapi::vm::AbstractMember;
using zapi::vm::internal::ZvalMemberPrivate;

ZvalMember::ZvalMember(const char *name, zval *value, Modifier flags)
   : AbstractMember(new ZvalMemberPrivate(name, value, flags))
{}

ZvalMember::ZvalMember(const ZvalMember &other)
   : AbstractMember(other)
{
   ZAPI_D(ZvalMember);
   Z_TRY_ADDREF_P(implPtr->m_value);
}

ZvalMember &ZvalMember::operator=(const ZvalMember &other)
{
   if (this != &other) {
      AbstractMember::operator=(other);
   }
   return *this;
}

ZvalMember::~ZvalMember()
{}

void ZvalMember::setupConstant(zend_class_entry *entry)
{
   ZAPI_D(ZvalMember);
   zend_declare_class_constant(entry, implPtr->m_name.c_str(), implPtr->m_name.size(), implPtr->m_value);
}

void ZvalMember::setupProperty(zend_class_entry *entry)
{
   ZAPI_D(ZvalMember);
   zend_declare_property(entry, implPtr->m_name.c_str(), implPtr->m_name.size(), 
                         implPtr->m_value, static_cast<int>(implPtr->m_flags));
}

} // vm
} // zapi