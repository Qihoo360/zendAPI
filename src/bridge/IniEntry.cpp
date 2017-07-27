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

#include "zapi/bridge/IniEntry.h"
#include "php/Zend/zend_ini.h"
#include <iostream>

namespace zapi
{
namespace bridge
{

class IniEntryPrivate
{
public:
   using CfgType = IniEntry::CfgType;
public:
   IniEntryPrivate(const char *name, const char *value, const CfgType cfgType = CfgType::All) 
      :m_name(name), m_value(value), m_cfgType(cfgType)
   {}
   IniEntryPrivate(const char *name, bool value, const CfgType cfgType = CfgType::All)
      :m_name(name), m_value(bool2str(value)), m_cfgType(cfgType)
   {}
   
   IniEntryPrivate(const char *name, const int16_t value, const CfgType cfgType = CfgType::All)
      :m_name(name), m_value(std::to_string(value)), m_cfgType(cfgType)
   {}
   
   IniEntryPrivate(const char *name, const int32_t value, const CfgType cfgType = CfgType::All)
      :m_name(name), m_value(std::to_string(value)), m_cfgType(cfgType)
   {}
   
   IniEntryPrivate(const char *name, const int64_t value, const CfgType cfgType = CfgType::All)
      :m_name(name), m_value(std::to_string(value)), m_cfgType(cfgType)
   {}
   
   IniEntryPrivate(const char *name, const double value, const CfgType cfgType = CfgType::All)
      :m_name(name), m_value(std::to_string(value)), m_cfgType(cfgType)
   {}
#ifdef ZAPI_CC_MSVC
   static const char *bool2str(const bool value)
#else
   static constexpr const char *bool2str(const bool value)
#endif
   {
      return (value ? "On" : "Off");
   }
   std::string m_name;
   std::string m_value;
   CfgType m_cfgType;
};

IniEntry::IniEntry(const char *name, const char *value, const CfgType cfgType) 
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}
IniEntry::IniEntry(const char *name, bool value, const CfgType cfgType)
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}

IniEntry::IniEntry(const char *name, const int16_t value, const CfgType cfgType)
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}

IniEntry::IniEntry(const char *name, const int32_t value, const CfgType cfgType)
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}

IniEntry::IniEntry(const char *name, const int64_t value, const CfgType cfgType)
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}

IniEntry::IniEntry(const char *name, const double value, const CfgType cfgType)
   : m_implPtr(new IniEntryPrivate(name, value, cfgType))
{}

IniEntry::~IniEntry()
{}

void IniEntry::setupIniEntryDef(_zend_ini_entry_def *zendIniDef, int moduleNumber)
{
   ZAPI_D(IniEntry);
   zendIniDef->modifiable = static_cast<int>(implPtr->m_cfgType);
   zendIniDef->name = implPtr->m_name.data();
   zendIniDef->name_length = implPtr->m_name.size();
   zendIniDef->on_modify = OnUpdateString;
   zendIniDef->mh_arg1 = nullptr;
#ifdef ZTS
   zendIniDef->mh_arg2 = (void *) &zapi_globals_id;
#else
   zendIniDef->mh_arg2 = (void *) &zapi_globals;
#endif
   zendIniDef->mh_arg3 = nullptr;
   zendIniDef->value = implPtr->m_value.data();
   zendIniDef->value_length = implPtr->m_value.size();
   zendIniDef->displayer = nullptr;
}

std::string IniValue::getStringValue() const
{
   const char *value = getRawValue();
   return std::string(value ? value : "");
}


class IniValuePrivate
{
public:
   IniValuePrivate(const char *name, const bool isOrig)
      : m_name(name), m_isOrig(isOrig)
   {}
   /**
    * @brief ini entry name
    * @var std::string
    */
   std::string m_name;
   
   /**
    * @brief Is the orig value
    * @var bool
    */
   bool m_isOrig = false;
};

IniValue::IniValue(const char *name, const bool isOrig)
   : m_implPtr(new IniValuePrivate(name, isOrig))
{}

IniValue::IniValue(const IniValue &value)
{
   ZAPI_D(IniValue);
   implPtr->m_name = value.m_implPtr->m_name;
   implPtr->m_isOrig = value.m_implPtr->m_isOrig;
}

IniValue::IniValue(IniValue &&value)
   : m_implPtr(std::move(value.m_implPtr))
{}

IniValue::~IniValue()
{}

/**
 * Cost to a number
 * 
 * @return int64_t
 */
int64_t IniValue::getNumericValue() const
{
   ZAPI_D(const IniValue);
   return zend_ini_long(const_cast<char *>(implPtr->m_name.c_str()), implPtr->m_name.size(), implPtr->m_isOrig);
}

const char* IniValue::getRawValue() const
{
   ZAPI_D(const IniValue);
   return zend_ini_string(const_cast<char *>(implPtr->m_name.c_str()), implPtr->m_name.size(), implPtr->m_isOrig);
}

IniValue::operator double() const
{
   ZAPI_D(const IniValue);
   return zend_ini_double(const_cast<char *>(implPtr->m_name.c_str()), implPtr->m_name.size(), implPtr->m_isOrig);
}

std::ostream &operator<<(std::ostream &stream, const IniValue &iniValue)
{
   return stream << static_cast<const char *>(iniValue);
}

} // kernel
} // zapi