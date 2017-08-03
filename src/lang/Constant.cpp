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
// Created by softboy on 2017/07/26.

#include <string>
#include "zapi/lang/Constant.h"
#include "php/Zend/zend_constants.h"
#include <iostream>

namespace zapi
{
namespace lang
{

namespace internal
{

class ConstantPrivate
{
public:
   ConstantPrivate(const char *name)
      : m_name(name)
   {}
   ConstantPrivate(const ConstantPrivate &other);
   ConstantPrivate(ConstantPrivate &&other);
   ~ConstantPrivate();
   void initialize(const std::string &prefix, int moduleNumber);
   std::string m_name;
   zend_constant m_constant;
   bool m_initialized = false;
};

ConstantPrivate::ConstantPrivate(const ConstantPrivate &other)
   : m_name(other.m_name)
{
   if (m_initialized) {
      m_constant.name = zend_string_dup(other.m_constant.name, 1);
      m_constant.flags = other.m_constant.flags;
      m_constant.module_number = other.m_constant.module_number;
   }
   if (Z_TYPE_P(&other.m_constant.value) == IS_STRING) {
      ZVAL_COPY_VALUE(&m_constant.value, &other.m_constant.value);
      ZVAL_NEW_STR(&m_constant.value, zend_string_dup(Z_STR(other.m_constant.value), 1));
   } else {
       ZVAL_DUP(&m_constant.value, &other.m_constant.value);
   }
}

ConstantPrivate::ConstantPrivate(ConstantPrivate &&other)
   : m_name(std::move(other.m_name))
{
   if (m_initialized) {
      std::swap(m_constant.name, other.m_constant.name);
      m_constant.flags = other.m_constant.flags;
      m_constant.module_number = other.m_constant.module_number;
   }
   std::swap(m_constant.value, other.m_constant.value);
}

void ConstantPrivate::initialize(const std::string &prefix, int moduleNumber)
{
   if (!prefix.empty()) {
      m_constant.name = zend_string_alloc(prefix.size() + 1 + m_name.size(), 1);
      std::strncpy(ZSTR_VAL(m_constant.name), prefix.c_str(), prefix.size());
      std::strncpy(ZSTR_VAL(m_constant.name) + prefix.size(), "\\", 1);
      std::strncpy(ZSTR_VAL(m_constant.name) + prefix.size() + 1, m_name.c_str(), m_name.size() + 1);
   } else {
      m_constant.name = zend_string_init(m_name.c_str(), m_name.size(), 1);
   }
   m_constant.flags = CONST_CS | CONST_PERSISTENT;
   m_constant.module_number = moduleNumber;
   zval_add_ref(&m_constant.value);
   zend_register_constant(&m_constant);
   m_initialized = true;
}

ConstantPrivate::~ConstantPrivate()
{
   zval_ptr_dtor(&m_constant.value);
}

} // internal

using internal::ConstantPrivate;

Constant::Constant(const char *name, std::nullptr_t)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_NULL(&implPtr->m_constant.value);
}

Constant::Constant(const char *name, bool value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_BOOL(&implPtr->m_constant.value, value);
}

Constant::Constant(const char *name, int32_t value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_LONG(&implPtr->m_constant.value, value);
}

Constant::Constant(const char *name, int64_t value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_LONG(&implPtr->m_constant.value, value);
}

Constant::Constant(const char *name, double value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_DOUBLE(&implPtr->m_constant.value, value);
}

Constant::Constant(const char *name, const char *value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_STRINGL(&implPtr->m_constant.value, value, ::strlen(value));
}

Constant::Constant(const char *name, const char *value, size_t size)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_STRINGL(&implPtr->m_constant.value, value, size);
}

Constant::Constant(const char *name, const std::string &value)
   : m_implPtr(new ConstantPrivate(name))
{
   ZAPI_D(Constant);
   ZVAL_STRINGL(&implPtr->m_constant.value, value.c_str(), value.size());
}

Constant::Constant(const Constant &other)
   : m_implPtr(new ConstantPrivate(*other.m_implPtr))
{}

Constant::Constant(Constant &&other) ZAPI_DECL_NOEXCEPT
   : m_implPtr(std::move(other.m_implPtr))
{}

Constant& Constant::operator=(const Constant &other)
{
   m_implPtr.reset(new ConstantPrivate(*other.m_implPtr));
   return *this;
}

Constant& Constant::operator=(Constant &&other) ZAPI_DECL_NOEXCEPT
{
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

Constant::~Constant()
{}

void Constant::initialize(const std::string &prefix, int moduleNumber)
{
   getImplPtr()->initialize(prefix, moduleNumber);
}

void Constant::initialize(int moduleNumber)
{
   getImplPtr()->initialize("", moduleNumber);
}

const zend_constant &Constant::getZendConstant() const
{
   ZAPI_D(const Constant);
   return implPtr->m_constant;
}

} // lang
} // zapi