#include "ExtFuncs.h"

void show_something()
{
   zapi::out << "hello world, zapi" << std::endl;
}
namespace dummyext
{

void show_something()
{
   zapi::out << "hello world, zapi" << std::endl;
}

} // dummyext