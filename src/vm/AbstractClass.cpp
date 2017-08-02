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
// Created by zzu_softboy on 2017/08/01.

#include "zapi/vm/AbstractClass.h"
#include "zapi/vm/internal/AbstractClassPrivate.h"

namespace zapi
{
namespace vm
{

namespace internal
{

AbstractClassPrivate::AbstractClassPrivate(const char *classname, lang::ClassType type)
{
   
}

} // internal


AbstractClass::AbstractClass(const char *className, lang::ClassType type)
   : m_implPtr(std::make_shared<AbstractClassPrivate>(className, type))
{}

AbstractClass::AbstractClass(const AbstractClass &other)
   : m_implPtr(other.m_implPtr)
{}

AbstractClass::AbstractClass(AbstractClass &&other) ZAPI_DECL_NOEXCEPT
   : m_implPtr(std::move(other.m_implPtr))
{}

AbstractClass &AbstractClass::operator=(const AbstractClass &other)
{
   return *this;
}

AbstractClass &AbstractClass::operator=(AbstractClass &&other) ZAPI_DECL_NOEXCEPT
{
   return *this;
}

StdClass *AbstractClass::construct() const
{
   return nullptr;
}

StdClass *AbstractClass::clone() const
{
   return nullptr;
}

AbstractClass::~AbstractClass()
{}

} // vm
} // zapi