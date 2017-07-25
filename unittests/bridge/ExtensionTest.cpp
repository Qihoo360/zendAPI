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

int main(int argc, char **argv)
{
   int retCode = 0;
   php_embed_module.php_ini_path_override = PHP_INI_DIR;
   std::cout << php_embed_module.php_ini_path_override << std::endl;
   PHP_EMBED_START_BLOCK(argc, argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}
