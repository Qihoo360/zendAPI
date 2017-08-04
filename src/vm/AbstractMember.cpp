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

#include "zapi/vm/AbstractMember.h"
#include "zapi/vm/internal/AbstractMemberPrivate.h"

namespace zapi
{
namespace vm
{

using zapi::vm::internal::AbstractMemberPrivate;

AbstractMember::AbstractMember()
{}

AbstractMember::AbstractMember(const char *name, lang::Modifier flags)
   : m_implPtr(new AbstractMemberPrivate(name, flags))
{}

AbstractMember::AbstractMember(const AbstractMember &other)
   : m_implPtr(new AbstractMemberPrivate(*other.m_implPtr))
{}

AbstractMember::AbstractMember(AbstractMember &&other) ZAPI_DECL_EXPORT
   : m_implPtr(std::move(other.m_implPtr))
{}

AbstractMember::AbstractMember(AbstractMemberPrivate *implPtr)
   : m_implPtr(implPtr)
{}

AbstractMember &AbstractMember::operator=(const AbstractMember &other)
{
   m_implPtr.reset(new AbstractMemberPrivate(*other.m_implPtr));
   return *this;
}

AbstractMember &AbstractMember::operator=(AbstractMember &&other) ZAPI_DECL_EXPORT
{
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

void AbstractMember::initialize(zend_class_entry *entry)
{
   ZAPI_D(AbstractMember);
   if (implPtr->m_flags == Modifier::Const) {
      setupConstant(entry);
   } else {
      setupProperty(entry);
   }
}

AbstractMember::~AbstractMember()
{}

} // vm
} // zapi