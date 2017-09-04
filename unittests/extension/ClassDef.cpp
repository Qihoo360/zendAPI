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
   //zapi::out << "Person construct" << std::endl;
}

void Person::__destruct()
{
   //zapi::out << "Person destruct" << std::endl;
}

void Person::showName()
{
   zapi::out << "my name is zapi" << std::endl;   
}

//Variant Person::__call(const std::string &method, Parameters &params) const
//{
//}

void Person::staticShowName()
{
   zapi::out << "static my name is zapi" << std::endl;   
}

StringVariant Person::concatStr(const StringVariant &lhs, const StringVariant &rhs)
{
   return lhs + rhs;
}

void Person::print_sum(zapi::ds::NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   zapi::out << "the sum is " << result << std::endl;
}

int Person::addSum(NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
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

void ConstructAndDestruct::__construct()
{
   zapi::out << "constructor been invoked" << std::endl;
}

void ConstructAndDestruct::__destruct()
{
   zapi::out << "destructor been invoked" << std::endl;;
}

void PropsTestClass::setAge(const Variant &value)
{
   NumericVariant age(value);
   age += 1;
   m_age = age.toLong();
}

Variant PropsTestClass::getAge()
{
   return m_age;
}

void PropsTestClass::setName(const Variant &name)
{
   StringVariant str(name);
   str.prepend("zapi:");
   m_name = str.toString();
}

Variant PropsTestClass::getName()
{
   return m_name;
}

Variant MagicMethodClass::__call(const std::string &method, Parameters &params) const
{
   if (method == "calculateSum") {
      NumericVariant sum;
      for (int i = 0; i < params.size(); i++) {
         sum += NumericVariant(params.at(i));
      }
      return sum;
   } else {
      return nullptr;
   }
}
