#include "ClassDef.h"
#include <string>
Person::Person()
   : name("zzu_softboy")
{}

//Variant Person::__get(const std::string &name)
//{
   
//}

void Person::__construct()
{
   zapi::out << "Person construct" << std::endl;
}

void Person::__destruct()
{
   // zapi::out << "Person destruct" << std::endl;
}

void Person::showName()
{
   zapi::out << "my name is zapi" << std::endl;   
}

void Person::staticShowName()
{
   zapi::out << "static my name is zapi" << std::endl;   
}

void Person::print_sum(zapi::ds::NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type));
   }
   zapi::out << "the sum is " << result << std::endl;
}

int Person::addSum(NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type));
   }
   return result;
}

int Person::addTwoNum(const NumericVariant &num1, const NumericVariant &num2)
{
   return num1 + num2;
}

Address::Address()
   : address("beijing")
{}