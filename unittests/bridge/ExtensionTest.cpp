#include "zapi/Global.h"
#include "zapi/bridge/Extension.h"
#include "zapi/bridge/IniEntry.h"
#include "zapi/utils/PhpFuncs.h"

#include "php/sapi/embed/php_embed.h"
#include "php/Zend/zend_types.h"
#include "php/Zend/zend_variables.h"
#include "php/main/php_ini.h"
#include "gtest/gtest.h"

#include <iostream>
#include <limits>
#include <string>
#include <cstring>

extern sapi_module_struct php_embed_module;
extern ::HashTable module_registry;

static bool dummyExtExist = false;
using zapi::bridge::Extension;
using zapi::bridge::IniEntry;

TEST(ExtensionTest, testdummyext)
{
   zend_hash_apply(&module_registry, [](zval *mzval) -> int {
      zend_module_entry *entry = static_cast<zend_module_entry *>(Z_PTR_P(mzval));
      if (0 == strcmp(entry->name, "dummyext")) {
         dummyExtExist = true;
         return ZEND_HASH_APPLY_STOP;
      }
      return ZEND_HASH_APPLY_KEEP;
   });
   ASSERT_TRUE(dummyExtExist);
}

TEST(ExtensionTest, testIniItemGet)
{
   // we set this in dummy extension, so we get this from here
   // this value we get it from zend engine
   ASSERT_STREQ(zapi::ini_get("zapi_author"), "xiuxiu");
   // define in php.ini but not regiester 
   ASSERT_EQ(zapi::ini_get("zapi_leader"), nullptr);
   ASSERT_STREQ(zapi::ini_get("zapi_team_address"), "shanghai");
   ASSERT_STREQ(zapi::ini_get("zapi_product"), "libpdk");
}

TEST(ExtensionTest, testExtConstructor)
{
   {
      Extension extension("zapi");
      ASSERT_EQ(extension.getName(), "zapi");
      ASSERT_EQ(extension.getVersion(), "1.0");
   }
   {
      Extension extension("zapi", "2.0");
      ASSERT_EQ(extension.getName(), "zapi");
      ASSERT_EQ(extension.getVersion(), "2.0");
   }
}

TEST(ExtensionTest, testRegisterIniEntry)
{
   Extension extension("zapi");
   ASSERT_EQ(extension.getIniEntryQuantity(), 0);
   IniEntry entry("zapi", "2.0");
   extension.registerIniEntry(entry);
   ASSERT_EQ(extension.getIniEntryQuantity(), 1);
}

int main(int argc, char **argv)
{
   int retCode = 0;
   php_embed_module.php_ini_path_override = const_cast<char *>(static_cast<const char *>(PHP_INI_DIR));
   PHP_EMBED_START_BLOCK(argc, argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}
