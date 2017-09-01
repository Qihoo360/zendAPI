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
extern void register_ns_io(Namespace &io);
extern void register_ns_zapi(Namespace &zapi);
extern void register_person_cls(zapi::lang::Class<Person> &meta);
extern void register_const(Extension &extension);
} // dummyext

extern "C" {

ZAPI_DECL_EXPORT void *get_module() 
{
   static zapi::lang::Extension extension("dummyext", "1.0");
   // this have no effect we write this in php.ini
   extension.registerIni(zapi::lang::Ini("zapi_author", "xiuxiu"));
   // rewrite in php.ini
   extension.registerIni(zapi::lang::Ini("zapi_team_address", "beijing"));
   // register but empty value
   extension.registerIni(zapi::lang::Ini("zapi_product", ""));
   dummyext::register_const(extension);
   dummyext::register_funcs(extension);
   zapi::lang::Class<Person> personClass("Person");
   dummyext::register_person_cls(personClass);
   personClass.registerConstant("QIHOO", "beijing qihoo asdasd");
   personClass.registerConstant("MY_CONST", "MY_CONST_VALUE");
   personClass.registerConstant(Constant("PI", 3.1415926));
   personClass.registerProperty("name", "zzu_softboy");
   personClass.registerProperty("staticProp", "beijing", Modifier::Public | Modifier::Static);
   Interface infoInterface("InfoProvider");
   //   infoInterface.registerMethod("getName");
   personClass.registerInterface(infoInterface);
   extension.registerInterface(infoInterface);
   extension.registerClass(personClass);
   zapi::lang::Class<Address> addressClass("Address");
   Namespace zapi("zapi");
   Namespace io("io");
   dummyext::register_ns_io(io);
   dummyext::register_ns_zapi(zapi);
   zapi.registerClass(addressClass);
   zapi.registerNamespace(io);
   extension.registerNamespace(zapi);
   return extension;
}

}