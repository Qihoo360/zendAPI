// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
// @copyright 2014 Copernica BV
//
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
// Created by zzu_softboy on 2017/07/27.

#include "zapi/vm/BoolMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm
{
namespace internal
{
class BoolMemberPrivate : public AbstractMemberPrivate
{
public:
   BoolMemberPrivate(const char *name, double value, Modifier flags)
      : AbstractMemberPrivate(name, flags),
        m_value(value)
   {}
   BoolMemberPrivate(const BoolMemberPrivate &other) = default;
   bool m_value;
};

} // internal

using internal::BoolMemberPrivate;

BoolMember::BoolMember(const char *name, double value, Modifier flags)
   : AbstractMember(new BoolMemberPrivate(name, value, flags))
{}

BoolMember::BoolMember(const BoolMember &other)
   : AbstractMember(other)
{}

BoolMember &BoolMember::operator=(const BoolMember &other)
{
   if (this != &other) {
      AbstractMember::operator=(other);
   }
   return *this;
}

void BoolMember::setupConstant(zend_class_entry *entry)
{
   ZAPI_D(BoolMember);
   zend_declare_class_constant_bool(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                                    implPtr->m_value);
}

void BoolMember::setupProperty(zend_class_entry *entry)
{
   ZAPI_D(BoolMember);
   zend_declare_property_bool(entry, implPtr->m_name.c_str(), implPtr->m_name.size(),
                              implPtr->m_value, static_cast<int>(implPtr->m_flags));
}

BoolMember::~BoolMember()
{}

} // vm
} // zapi
