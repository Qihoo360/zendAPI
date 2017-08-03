#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H
#include "zapi/ZendApi.h"

namespace dummyext
{

using zapi::lang::Variant;
using zapi::lang::Parameters;

void ZAPI_DECL_EXPORT show_something();
Variant ZAPI_DECL_EXPORT get_name();
void ZAPI_DECL_EXPORT print_name(Parameters &params);
void ZAPI_DECL_EXPORT print_name_and_age(Parameters &params);
Variant add_two_number(Parameters &params);
} // dummyext

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H