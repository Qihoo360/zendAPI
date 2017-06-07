# we detect the php execute binary path and the php version, Zend Engine API version

# we first search the newest php version
set(ZAPI_PHP_POSSIBLE_INCLUDE_PATHS
    /usr/local/php7/include
    /usr/local/php/include
    /usr/local/include
    /usr/include)

set(ZAPI_PHP_POSSIBLE_LIB_PATHS
    /usr/local/php7/lib
    /usr/local/php/lib
    /usr/lib)

set(ZAPI_PHP_POSSIBLE_BIN_PATHS
    /usr/local/php7/bin
    /usr/local/php/bin
    /usr/local/bin
    /usr/bin)

# find php include path
find_path(ZAPI_PHP_INCLUDE_PATH php/main/php.h
          PATHS ${ZAPI_PHP_POSSIBLE_INCLUDE_PATHS})

find_program(ZAPI_PHP_EXECUTABLE NAEMS php
             PATHS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

find_program(ZAPI_PHP_CONFIG_EXECUABLE php-config
             PATHS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

# if you build the unittest we will detect libphp library
if(NOT ZAPI_OPT_DISABLE_TESTS)
   find_library(ZAPI_PHP_LIB php7 NAEMS php php5
                PATHS ${ZAPI_PHP_POSSIBLE_LIB_PATHS} NO_DEFAULT_PATH)
   add_library(zapi_php_lib SHARED IMPORTED GLOBAL)
   set_target_properties(zapi_php_lib
                         PROPERTIES
                         IMPORTED_LOCATION ${ZAPI_PHP_LIB})
endif()

if (NOT ZAPI_PHP_INCLUDE_PATH)
   message(FATAL_ERROR "php include path is not found")
endif()

if (NOT ZAPI_PHP_EXECUTABLE)
   message(FATAL_ERROR "php executable is not found")
endif()

if (NOT ZAPI_OPT_DISABLE_TESTS AND NOT ZAPI_PHP_LIB)
   message(FATAL_ERROR "php library is not found")
endif()

# we use php-config to detect php extra include paths
execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --includes
                RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
                OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
                ERROR_QUIET)
if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
   message(FATAL_ERROR "run php-config error")
endif()

string(REPLACE " " ";" ZAPI_TEMP_RUN_PHPCFG_OUTPUT ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT})

foreach(zapi_temp_include_path ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT})
   string(SUBSTRING ${zapi_temp_include_path} 2 -1 zapi_temp_include_path)
   list(APPEND ZAPI_PHP_INCLUDE_PATH ${zapi_temp_include_path})
endforeach()
message("${ZAPI_PHP_INCLUDE_PATH}")
include_directories(ZAPI_PHP_INCLUDE_PATH)
