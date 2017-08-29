#include "zapi/Global.h"
#include "zapi/lang/Extension.h"
#include "zapi/lang/Ini.h"
#include "zapi/lang/Constant.h"
#include "zapi/utils/PhpFuncs.h"
#include "zapi/vm/Engine.h"

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
using zapi::lang::Extension;
using zapi::lang::Ini;
using zapi::lang::Constant;
using zapi::vm::Engine;

static std::string phpOutput;

//TEST(ExtensionTest, testdummyext)
//{
//   zend_hash_apply(&module_registry, [](zval *mzval) -> int {
//      zend_module_entry *entry = static_cast<zend_module_entry *>(Z_PTR_P(mzval));
//      if (0 == strcmp(entry->name, "dummyext")) {
//         dummyExtExist = true;
//         return ZEND_HASH_APPLY_STOP;
//      }
//      return ZEND_HASH_APPLY_KEEP;
//   });
//   ASSERT_TRUE(dummyExtExist);
//}

//TEST(ExtensionTest, testIniItemGet)
//{
//   // we set this in dummy extension, so we get this from here
//   // this value we get it from zend engine
//   ASSERT_STREQ(zapi::ini_get("zapi_author"), "xiuxiu");
//   // define in php.ini but not regiester 
//   ASSERT_EQ(zapi::ini_get("zapi_leader"), nullptr);
//   ASSERT_STREQ(zapi::ini_get("zapi_team_address"), "shanghai");
//   ASSERT_STREQ(zapi::ini_get("zapi_product"), "libpdk");
//}

//TEST(ExtensionTest, testExtConstructor)
//{
//   {
//      Extension extension("zapi");
//      ASSERT_EQ(extension.getName(), "zapi");
//      ASSERT_EQ(extension.getVersion(), "1.0");
//   }
//   {
//      Extension extension("zapi", "2.0");
//      ASSERT_EQ(extension.getName(), "zapi");
//      ASSERT_EQ(extension.getVersion(), "2.0");
//   }
//}

//TEST(ExtensionTest, testRegisterIni)
//{
//   Extension extension("zapi");
//   ASSERT_EQ(extension.getIniQuantity(), 0);
//   Ini entry("zapi", "2.0");
//   extension.registerIni(entry);
//   ASSERT_EQ(extension.getIniQuantity(), 1);
//}

//TEST(ExtensionTest, testRegisterContsant)
//{
//   Extension extension("zapi");
//   Constant const1("MY_CONST", "CONST_VALUE1");
//   ASSERT_EQ(extension.getConstantQuantity(), 0);
//   extension.registerConstant(const1);
//   zend_module_entry *moduleEntry = static_cast<zend_module_entry *>(extension.getModule());
//   ASSERT_EQ(extension.getConstantQuantity(), 1);
//   std::string code;
//   code += "$name = \"zapi\";"
//           "echo ZAPI_NAME;echo \"\\n\";echo \"xiuxiux\";";
//   Engine::eval(code);
//   ASSERT_EQ(phpOutput, "zapi\nxiuxiux");
//   phpOutput.clear();
//   code = "echo ZAPI_VERSION;";
//   Engine::eval(code);
//   ASSERT_EQ(phpOutput, "v0.0.1");
//   phpOutput.clear();
//}

TEST(ExtensionTest, testFuncRegister)
{
   std::string code("show_something();");
   //   Engine::eval(code);
   //   ASSERT_EQ(phpOutput, "hello world, zapi");
   //   phpOutput.clear();
   //   code = "print_name(\"zapi\");";
   //   Engine::eval(code);
   //   ASSERT_EQ(phpOutput, "zapi");
   //   phpOutput.clear();
   //   code = "print_name_and_age(\"zzu_softboy\", 27);";
   //   Engine::eval(code);
   //   ASSERT_EQ(phpOutput, "name: zzu_softboy age: 27");
   //   phpOutput.clear();
   // code = "zapi\\io\\print_name(\"zapi\");";
   //Engine::eval(code);
   //   ASSERT_EQ(phpOutput, "bool(true)\n");
   
   /// code = "zapi\\io\\print_sum(1, 2, 3, 4, 5, 9, 10, 11, 12);";
   //   code = "echo \\get_name();";
//   code = "echo \\add_two_number(1, 2);";
//   code = "echo \\zapi\\io\\calculate_sum(1, 2, 3, 4, 5);";
   code = "$p = new Person;$p->showName();";
   Engine::eval(code);
   std::cout << phpOutput << std::endl;
}

//TEST(ExtensionTest, testClassRegister)
//{
//   phpOutput.clear();
//   std::string code("var_dump(class_exists('\\Person'));");
//   Engine::eval(code);
//   std::cout << phpOutput << std::endl;
//   //ASSERT_EQ(phpOutput, "hello world, zapi");
//   code = "$person = new Person();var_dump($person);";
//   Engine::eval(code);
//   std::cout << phpOutput << std::endl;
//}

size_t buffer_write(const char *str, size_t str_length)
{
   phpOutput += std::string(str, str_length);
   return str_length;
}

int main(int argc, char **argv)
{
   int retCode = 0;
   php_embed_module.php_ini_path_override = const_cast<char *>(static_cast<const char *>(PHP_INI_FILENAME));
   php_embed_module.ub_write = buffer_write;
   PHP_EMBED_START_BLOCK(argc, argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}
