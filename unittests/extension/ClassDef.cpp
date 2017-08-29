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

Address::Address()
   : address("beijing")
{}