#include "php.h"
#include <iostream>
#define phpext_walu_ptr &walu_module_entry


int zapi_init(int a, int b){
   return 1;
}

//module entry
zend_module_entry zenapi_module_entry = {
   #if ZEND_MODULE_API_NO >= 20010901
   STANDARD_MODULE_HEADER,
   #endif
   "dummyext", //这个地方是扩展名称，往往我们会在这个地方使用一个宏。
   NULL, /* Functions */
   zapi_init, /* MINIT */
   NULL, /* MSHUTDOWN */
   NULL, /* RINIT */
   NULL, /* RSHUTDOWN */
   NULL, /* MINFO */
   #if ZEND_MODULE_API_NO >= 20010901
   "2.1", //这个地方是我们扩展的版本
   #endif
   STANDARD_MODULE_PROPERTIES
};

extern "C" {
/**
  *  Function that is called by PHP right after the PHP process
  *  has started, and that returns an address of an internal PHP
  *  strucure with all the details and features of your extension
  *
  *  @return void*   a pointer to an address that is understood by PHP
  */
void *get_module() 
{
   return &zenapi_module_entry;
}

}