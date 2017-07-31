#include "zapi/ZendApi.h"
#include <iostream>
#include <string>

using zapi::lang::Constant;

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
   extension.registerConstant(Constant("ZAPI_NAME", "zapi"));
   extension.registerConstant(Constant("ZAPI_VERSION", "v0.0.1"));
   return extension;
}

}