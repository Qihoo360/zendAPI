#include "ExtFuncs.h"

namespace dummyext
{

using zapi::lang::Variant;

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
//   std::string name = params.at(0).toString();
//   int age = params.at(1).toLong();
//   zapi::out << "name: " << name << " age: " << age << std::flush;
}

Variant add_two_number(Parameters &params)
{
//   int number1 = params.at(0).toLong();
//   int number2 = params.at(1).toLong();
//   return number1 + number2;
}

} // dummyext