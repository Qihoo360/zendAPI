#include "zapi/lang/IniEntry.h"
#include "php/sapi/embed/php_embed.h"
#include "php/main/php_ini.h"
#include "gtest/gtest.h"

#include <iostream>
#include <limits>
#include <string>
#include <cstring>

using zapi::lang::IniEntry;

TEST(IniEntryTest, testConstructor)
{
   {
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", "0.0.1", IniEntry::CfgType::All);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.name, "zapi_version");
      ASSERT_EQ(entryDef.name_length, std::strlen("zapi_version"));
      ASSERT_EQ(entryDef.modifiable, static_cast<int>(IniEntry::CfgType::All));
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
      IniEntry iniEntry("zapi_version", "0.0.1", IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_EQ(entryDef.modifiable, static_cast<int>(IniEntry::CfgType::System));
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", true, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "On");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", false, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "Off");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", (int16_t)12, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", (int32_t)12, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", (int64_t)12, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "12");
   }
   {
      // test value type
      zend_ini_entry_def entryDef;
      IniEntry iniEntry("zapi_version", 3.14, IniEntry::CfgType::System);
      iniEntry.setupIniEntryDef(&entryDef);
      ASSERT_STREQ(entryDef.value, "3.140000");
   }
}
