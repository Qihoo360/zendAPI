#include "ExtFuncs.h"

namespace dummyext
{

using zapi::ds::Variant;
using zapi::ds::StringVariant;
using zapi::ds::NumericVariant;
using zapi::lang::Parameters;
using zapi::lang::Arguments;
using zapi::lang::ValueArgument;
using zapi::lang::Constant;

void show_something()
{
   zapi::out << "hello world, zapi" << std::flush;
}

Variant get_name()
{
   return "zzu_softboy";
}

void print_name(Parameters &params)
{
   zapi::out << params.at(0).toString() << std::flush;
}

void print_name_and_age(Parameters &params)
{
   std::string name = StringVariant(params.at(0)).getCStr();
   int age = NumericVariant(params.at(1)).toLong();
   zapi::out << "name: " << name << " age: " << age << std::flush;
}

Variant add_two_number(Parameters &params)
{
   int number1 = NumericVariant(params.at(0)).toLong();
   int number2 = NumericVariant(params.at(1)).toLong();
   return number1 + number2;
}

void register_funcs(Extension &extension)
{
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
}

void register_ns_io(Namespace &io)
{
   io.registerFunction<dummyext::print_name>
         ("print_name", {
             ValueArgument("name", zapi::lang::Type::String)
          });
   io.registerFunction<dummyext::show_something>("show_something");
   io.registerConstant(Constant("IO_TYPE", "ASYNC"));
   io.registerConstant(Constant("NATIVE_STREAM", true));
}

void register_ns_zapi(Namespace &zapi)
{
   zapi.registerFunction<dummyext::get_name>("get_name");
   zapi.registerConstant(Constant("SYS_VERSION", "0.1.1-alpha"));
   zapi.registerFunction<dummyext::show_something>("show_something");
}


} // dummyext