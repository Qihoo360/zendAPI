#include <iostream>
#include <string>

#include "zapi/ZendApi.h"
#include "ExtFuncs.h"
#include "ClassDef.h"

using zapi::lang::Constant;
using zapi::lang::ValueArgument;
using zapi::lang::Interface;
using zapi::lang::Modifier;
using zapi::lang::Namespace;
using zapi::ds::StringVariant;
using zapi::lang::Extension;

namespace dummyext
{

extern void register_funcs(Extension &extension);

} // dummyext

extern "C" {

ZAPI_DECL_EXPORT void *get_module() 
{
   static zapi::lang::Extension extension("dummyext", "1.0");
   // this have no effect we write this in php.ini
   extension.registerIniEntry(zapi::lang::IniEntry("zapi_author", "xiuxiu"));
   // rewrite in php.ini
   extension.registerIniEntry(zapi::lang::IniEntry("zapi_team_address", "beijing"));
   // register but empty value
   extension.registerIniEntry(zapi::lang::IniEntry("zapi_product", ""));
   extension.registerConstant(Constant("MY_CONST", 12333));
   Constant nameConst("ZAPI_NAME", "zapi");
   extension.registerConstant(nameConst);
   extension.registerConstant(Constant("ZAPI_VERSION", "v0.0.1"));
   extension.registerConstant(Constant("QIHOO", "beijing qihoo"));
   dummyext::register_funcs(extension);
   zapi::lang::Class<Person> personClass("Person");
   personClass.registerMethod<&Person::__construct>("__construct");
   personClass.registerConstant("QIHOO", "beijing qihoo asdasd");
   personClass.registerConstant("MY_CONST", "MY_CONST_VALUE");
   personClass.registerConstant(Constant("PI", 3.1415926));
   personClass.registerProperty("name", "zzu_softboy");
   personClass.registerProperty("staticProp", "beijing", Modifier::Public | Modifier::Static);
   Interface infoInterface("InfoProvider");
   infoInterface.registerMethod("getName");
   personClass.registerInterface(infoInterface);
   extension.registerInterface(infoInterface);
   extension.registerClass(personClass);
   zapi::lang::Class<Address> addressClass("Address");
   Namespace zapi("zapi");
   Namespace io("io");
   io.registerFunction<dummyext::print_name>("print_name", {
                                                ValueArgument("name", zapi::lang::Type::String)
                                             });
   io.registerFunction<dummyext::show_something>("show_something");
   zapi.registerClass(addressClass);
   zapi.registerNamespace(io);
   zapi.registerFunction<dummyext::get_name>("get_name");
   zapi.registerConstant(Constant("SYS_VERSION", "0.1.1-alpha"));
   extension.registerNamespace(zapi);
   return extension;
}

}