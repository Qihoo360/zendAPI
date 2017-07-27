#include "zapi/ZendApi.h"
#include <iostream>

extern "C" {

ZAPI_DECL_EXPORT void *get_module() 
{
    static zapi::bridge::Extension extension("dummyext", "1.0");
    return extension;
}

}