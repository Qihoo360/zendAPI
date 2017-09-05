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
   zapi::out << "MagicMethodClass::__call is called" << std::endl;
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

Variant MagicMethodClass::__invoke(Parameters &params) const
{
   zapi::out << "MagicMethodClass::__invoke is called" << std::endl;
   NumericVariant sum;
   for (int i = 0; i < params.size(); i++) {
      sum += NumericVariant(params.at(i));
   }
   return sum;
}

void MagicMethodClass::__set(const std::string &key, const Variant &value)
{
   zapi::out << "MagicMethodClass::__set is called" << std::endl;
   if (key == "address") {
      m_address = StringVariant(value).toString();
      m_teamAddressUnset = false;
   }
}

Variant MagicMethodClass::__get(const std::string &key) const
{
   zapi::out << "MagicMethodClass::__get is called" << std::endl;
   if (key == "prop1") {
      return "zapi";
   } else if(key == "teamName" && !m_teamNameUnset) {
      return "unicornteam";
   } else if (key == "address" && !m_teamAddressUnset) {
      return m_address;
   }
   return nullptr;
}

bool MagicMethodClass::__isset(const std::string &key) const
{
   zapi::out << "MagicMethodClass::__isset is called" << std::endl;
   if (key == "prop1") {
      return true; 
   } else if (key == "teamName" && !m_teamNameUnset) {
      return true;
   } else if (key == "address" && !m_teamAddressUnset) {
      return true;
   }
   return false;
}

void MagicMethodClass::__unset(const std::string &key)
{
   zapi::out << "MagicMethodClass::__unset is called" << std::endl;
   if (key == "teamName") {
      m_teamNameUnset = true;
   } else if (key == "address") {
      m_teamAddressUnset = true;
   }
}

void MagicMethodClass::__clone()
{
   zapi::out << "MagicMethodClass::__clone is called" << std::endl;
}

Variant MagicMethodClass::__callStatic(const std::string &method, Parameters &params)
{
   zapi::out << "MagicMethodClass::__callStatic is called" << std::endl;
   if (method == "staticCalculateSum") {
      NumericVariant sum;
      for (int i = 0; i < params.size(); i++) {
         sum += NumericVariant(params.at(i));
      }
      return sum;
   } else {
      StringVariant str("hello, ");
      str += StringVariant(params.at(0));
      return str;
   }
}

std::string MagicMethodClass::serialize()
{
   zapi::out << "MagicMethodClass::serialize is called" << std::endl;
   return "serialize data";
}

void MagicMethodClass::unserialize(const char *input, size_t size)
{
   zapi::out << "MagicMethodClass::unserialize is called" << std::endl;
   zapi::out << "serialize data : " << input << std::endl;
}

MagicMethodClass::~MagicMethodClass() ZAPI_DECL_NOEXCEPT
{
   
}
