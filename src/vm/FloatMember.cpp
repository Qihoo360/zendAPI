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

#include "zapi/vm/FloatMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm
{
namespace internal
{
class FloatMemberPrivate : public AbstractMemberPrivate
{
public:
   FloatMemberPrivate(const char *name, double value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value)
   {}
   FloatMemberPrivate(const FloatMemberPrivate &other) = default;
   double m_value;
};

} // internal

using internal::FloatMemberPrivate;

FloatMember::FloatMember(const char *name, double value, Modifier flags)
   : AbstractMember(new FloatMemberPrivate(name, value, flags))
{}

FloatMember::FloatMember(const FloatMember &other)
   : AbstractMember(new FloatMemberPrivate(*static_cast<FloatMemberPrivate *>(other.m_implPtr.get())))
{}

FloatMember &FloatMember::operator=(const FloatMember &other)
{
   m_implPtr.reset(new FloatMemberPrivate(*static_cast<FloatMemberPrivate *>(other.m_implPtr.get())));
   return *this;
}

void FloatMember::setupConstant(zend_class_entry *entry)
{
   ZAPI_D(FloatMember);
   zend_declare_class_constant_double(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                                      implPtr->m_value);
}

void FloatMember::setupProperty(zend_class_entry *entry)
{
   ZAPI_D(FloatMember);
   zend_declare_property_double(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                                implPtr->m_value, static_cast<int>(implPtr->m_flags));
}

FloatMember::~FloatMember()
{}

} // vm
} // zapi