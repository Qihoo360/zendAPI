#include "ClassDef.h"
#include <string>
Person::Person()
   : name("zzu_softboy")
{}

Variant Person::__get(const std::string &name)
{
   return "zapi";
}
