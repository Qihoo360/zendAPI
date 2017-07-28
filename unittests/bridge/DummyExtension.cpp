#include "zapi/ZendApi.h"
#include <iostream>

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
   return extension;
}

}