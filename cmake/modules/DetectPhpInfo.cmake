# we detect the php execute binary path and the php version, Zend Engine API version

# we first search the newest php version
set(ZAPI_PHP_POSSIBLE_INCLUDE_PATHS
    /usr/local/php7/include/php
    /usr/local/php/include/php
    /usr/local/include/php
    /usr/include/php)

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
find_path(ZAPI_PHP_INCLUDE_PATH main/php.h
          PATHS ${ZAPI_PHP_POSSIBLE_INCLUDE_PATHS})

find_program(ZAPI_PHP_EXECUTABLE NAEMS php
             PATHS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

find_program(ZAPI_PHP_CONFIG_EXECUABLE php-config
              PATHS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

# if you build the unittest we will detect libphp library
if(NOT ZAPI_OPT_DISABLE_TESTS)
   find_library(ZAPI_PHP_LIB php7 NAEMS php php5
                PATHS ${ZAPI_PHP_POSSIBLE_LIB_PATHS} NO_DEFAULT_PATH)
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

include_directories(ZAPI_PHP_INCLUDE_PATH)

