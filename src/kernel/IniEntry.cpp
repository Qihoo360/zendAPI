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
// Created by softboy on 7/25/17.

#include "zapi/kernel/IniEntry.h"
#include "php/Zend/zend_ini.h"
#include <iostream>

namespace zapi
{
namespace kernel
{

void IniEntry::fill(_zend_ini_entry_def *iniEntry, int moduleNumber)
{
   iniEntry->modifiable = static_cast<int>(m_cfgType);
   iniEntry->name = m_name.data();
   iniEntry->name_length = m_name.size();
   iniEntry->on_modify = OnUpdateString;
   iniEntry->mh_arg1 = nullptr;
#ifdef ZTS
   iniEntry->mh_arg2 = (void *) &zapi_globals_id;
#else
   iniEntry->mh_arg2 = (void *) &zapi_globals;
#endif
   iniEntry->mh_arg3 = nullptr;
   iniEntry->value = m_value.data();
   iniEntry->value_length = m_value.size();
   iniEntry->displayer = nullptr;
}

std::string IniValue::getStringValue() const
{
   const char *value = getRawValue();
   return std::string(value ? value : "");
}

/**
 * Cost to a number
 * 
 * @return int64_t
 */
int64_t IniValue::getNumericValue() const
{
   return zend_ini_long(const_cast<char *>(m_name.c_str()), m_name.size(), m_isOrig);
}

const char* IniValue::getRawValue() const
{
   return zend_ini_string(const_cast<char *>(m_name.c_str()), m_name.size(), m_isOrig);
}

IniValue::operator double() const
{
   return zend_ini_double(const_cast<char *>(m_name.c_str()), m_name.size(), m_isOrig);
}

std::ostream &operator<<(std::ostream &stream, const IniValue &iniValue)
{
   return stream << static_cast<const char *>(iniValue);
}

} // kernel
} // zapi