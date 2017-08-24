#include "zapi/lang/Ini.h"
#include "php/sapi/embed/php_embed.h"
#include "php/main/php_ini.h"
#include "gtest/gtest.h"

#include <iostream>
#include <limits>
#include <string>
#include <cstring>

using zapi::lang::Ini;

TEST(IniTest, testConstructor)
{
   {
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", "0.0.1", Ini::CfgType::All);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.name, "zapi_version");
      ASSERT_EQ(entryDef.name_length, std::strlen("zapi_version"));
      ASSERT_EQ(entryDef.modifiable, static_cast<int>(Ini::CfgType::All));
#ifdef ZTS
      ASSERT_EQ(entryDef.mh_arg2, (void *) &zapi_globals_id);
#else
      ASSERT_EQ(entryDef.mh_arg2, (void *) &zapi_globals);
#endif
      ASSERT_EQ(entryDef.mh_arg1, nullptr);
      ASSERT_EQ(entryDef.mh_arg3, nullptr);
      ASSERT_EQ(entryDef.displayer, nullptr);
      ASSERT_STREQ(entryDef.value, "0.0.1");
      ASSERT_EQ(entryDef.value_length, std::strlen("0.0.1"));
   }
   {
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", "0.0.1", Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_EQ(entryDef.modifiable, static_cast<int>(Ini::CfgType::System));
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", true, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "On");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", false, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "Off");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", (int16_t)12, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", (int32_t)12, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", (int64_t)12, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      Ini iniEntry("zapi_version", 3.14, Ini::CfgType::System);
      iniEntry.setupIniDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "3.140000");
   }
}
