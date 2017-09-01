#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H
#include "zapi/ZendApi.h"

namespace dummyext
{

using zapi::lang::Variant;
using zapi::lang::Parameters;
using zapi::lang::Extension;
using zapi::lang::Namespace;
using zapi::ds::StringVariant;
using zapi::ds::NumericVariant;

ZAPI_DECL_EXPORT Variant return_arg(Parameters &params);
ZAPI_DECL_EXPORT void show_something();
ZAPI_DECL_EXPORT Variant get_name();
ZAPI_DECL_EXPORT void get_value_ref(NumericVariant &number);
ZAPI_DECL_EXPORT void passby_value(NumericVariant &number);
ZAPI_DECL_EXPORT void print_sum(NumericVariant argQuantity, ...);
ZAPI_DECL_EXPORT void print_name(const StringVariant &name);
ZAPI_DECL_EXPORT void print_name_and_age(Parameters &params);
ZAPI_DECL_EXPORT Variant calculate_sum(NumericVariant argQuantity, ...);
ZAPI_DECL_EXPORT Variant add_two_number(const NumericVariant &num1, const NumericVariant &num2);
ZAPI_DECL_EXPORT void say_hello(StringVariant &name);
ZAPI_DECL_EXPORT void register_funcs(Extension &extension);
ZAPI_DECL_EXPORT void register_ns_io(Namespace &io);
ZAPI_DECL_EXPORT void register_ns_zapi(Namespace &zapi);

} // dummyext

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_FUNCS_H
