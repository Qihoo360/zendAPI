#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/lang/FatalError.h"
#include "zapi/lang/Type.h"

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
