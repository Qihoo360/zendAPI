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
// Created by zzu_softboy on 2017/08/04.

#include "zapi/vm/NullMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm
{

using zapi::vm::internal::AbstractMemberPrivate;

NullMember::NullMember(const char *name, Modifier flags)
   : AbstractMember(name, flags)
{}

void NullMember::setupConstant(zend_class_entry *entry)
{
   
   zend_declare_class_constant_null(entry, m_implPtr->m_name.c_str(), m_implPtr->m_name.size());
}

void NullMember::setupProperty(zend_class_entry *entry)
{
   zend_declare_property_null(entry, m_implPtr->m_name.c_str(), m_implPtr->m_name.size(), 
                              static_cast<int>(m_implPtr->m_flags));
}

NullMember::~NullMember()
{}

} // vm
} // zapi
