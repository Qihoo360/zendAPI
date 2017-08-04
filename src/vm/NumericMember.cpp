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
// Created by zzu_softboy on 2017/08/04.

#include "zapi/vm/NumericMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm
{
namespace internal
{
class NumericMemberPrivate : public AbstractMemberPrivate
{
public:
   NumericMemberPrivate(const char *name, double value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value)
   {}
   NumericMemberPrivate(const NumericMemberPrivate &other) = default;
   long m_value;
};

} // internal

using internal::NumericMemberPrivate;

NumericMember::NumericMember(const char *name, double value, Modifier flags)
   : AbstractMember(new NumericMemberPrivate(name, value, flags))
{}

NumericMember::NumericMember(const NumericMember &other)
   : AbstractMember(new NumericMemberPrivate(*static_cast<NumericMemberPrivate *>(other.m_implPtr.get())))
{}

NumericMember &NumericMember::operator=(const NumericMember &other)
{
   m_implPtr.reset(new NumericMemberPrivate(*static_cast<NumericMemberPrivate *>(other.m_implPtr.get())));
   return *this;
}

void NumericMember::setupConstant(zend_class_entry *entry)
{
   ZAPI_D(NumericMember);
   zend_declare_class_constant_long(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                                      implPtr->m_value);
}

void NumericMember::setupProperty(zend_class_entry *entry)
{
   ZAPI_D(NumericMember);
   zend_declare_property_long(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                                implPtr->m_value, static_cast<int>(implPtr->m_flags));
}

NumericMember::~NumericMember()
{}

} // vm
} // zapi