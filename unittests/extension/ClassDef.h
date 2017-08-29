#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H

#include "zapi/ZendApi.h"
#include <string>

using zapi::lang::StdClass;
using zapi::lang::Variant;

class Person : public StdClass
{
public:
   Person();
//   Variant __get(const std::string &name);
   void __construct();
   void __destruct();
   void showName();
private:
   /**
     *  The initial value
     *  @var    int
     */
   std::string name;
};

class Address : public StdClass
{
public:
   Address();
protected:
   std::string address;
};

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
