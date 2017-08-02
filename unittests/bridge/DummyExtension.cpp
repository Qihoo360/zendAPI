#include <iostream>
#include <string>

#include "zapi/ZendApi.h"
#include "ExtFuncs.h"

using zapi::lang::Constant;
using zapi::lang::ValueArgument;

extern "C" {

ZAPI_DECL_EXPORT void *get_module() 
{
   static zapi::bridge::Extension extension("dummyext", "1.0");
   // this have no effect we write this in php.ini
   extension.registerIniEntry(zapi::bridge::IniEntry("zapi_author", "xiuxiu"));
   // rewrite in php.ini
   extension.registerIniEntry(zapi::bridge::IniEntry("zapi_team_address", "beijing"));
   // register but empty value
   extension.registerIniEntry(zapi::bridge::IniEntry("zapi_product", ""));
  // extension.registerConstant(Constant("MY_CONST", 12333));
   Constant nameConst("ZAPI_NAME", "zapi");
   extension.registerConstant(nameConst);
   extension.registerConstant(Constant("ZAPI_VERSION", "v0.0.1"));
   extension.registerFunction<dummyext::show_something>("show_something");
   extension.registerFunction<dummyext::get_name>("get_name");
   extension.registerFunction<dummyext::print_name>("print_name", {
                                                       ValueArgument("name", zapi::lang::Type::String)
                                                    });
   extension.registerFunction<dummyext::print_name_and_age>("print_name_and_age", {
                                                       ValueArgument("name", zapi::lang::Type::String),
                                                       ValueArgument("age", zapi::lang::Type::Long)
                                                    });
   extension.registerFunction<dummyext::add_two_number>("add_two_number", {
                                                       ValueArgument("number1", zapi::lang::Type::Long),
                                                       ValueArgument("number2", zapi::lang::Type::Long)
                                                    });
   return extension;
}

}