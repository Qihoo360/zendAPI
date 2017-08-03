#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H

#include "zapi/ZendApi.h"
#include <string>

using zapi::lang::StdClass;

class Person : public StdClass
{
public:
   Person();
private:
   /**
     *  The initial value
     *  @var    int
     */
   std::string name;
};

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
