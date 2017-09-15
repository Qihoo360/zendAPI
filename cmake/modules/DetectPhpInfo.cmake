# we detect the php execute binary path and the php version, Zend Engine API version

# we first search the newest php version

set(ZAPI_PHP_POSSIBLE_INCLUDE_PATHS
    /usr/local/php7/include
    /usr/local/php/include
    /usr/local/include
    /usr/include)

if (ZAPI_OPT_PHP_INCLUDE_PATH)
    list(INSERT ZAPI_PHP_POSSIBLE_INCLUDE_PATHS 0 ${ZAPI_OPT_PHP_INCLUDE_PATH}) 
endif()

set(ZAPI_PHP_POSSIBLE_LIB_PATHS
    /usr/local/php7/lib
    /usr/local/php/lib
    /usr/lib)

if (ZAPI_OPT_PHP_LIB_PATH)
    list(INSERT ZAPI_PHP_POSSIBLE_LIB_PATHS 0 ${ZAPI_OPT_PHP_LIB_PATH})
endif()

set(ZAPI_PHP_POSSIBLE_BIN_PATHS
    /usr/local/php7/bin
    /usr/local/php/bin
    /usr/local/bin
    /usr/bin)

if (ZAPI_OPT_PHP_BIN_PATH)
    list(INSERT ZAPI_PHP_POSSIBLE_BIN_PATHS 0 ${ZAPI_OPT_PHP_BIN_PATH})
endif()

# find php include path
find_path(ZAPI_PHP_INCLUDE_PATH php/main/php.h
    HINTS ${ZAPI_PHP_POSSIBLE_INCLUDE_PATHS}
    )

find_program(ZAPI_PHP_EXECUTABLE NAEMS php
    HINTS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

find_program(ZAPI_PHP_CONFIG_EXECUABLE php-config
    HINTS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)

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

# here we need ensure detected paths is compatible
execute_process(COMMAND ${ZAPI_PHP_EXECUTABLE} ${ZAPI_SCRIPTS_DIR}/build/retrieve_php_info.php ${ZAPI_PHP_INCLUDE_PATH} --PHP_VERSION_ID
    RESULT_VARIABLE ZAPI_TEMP_RUN_PHP_INFO_RET
    OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHP_INFO_OUTPUT
    ERROR_QUIET)

if (ZAPI_TEMP_RUN_PHP_INFO_RET)
    message(FATAL_ERROR "execute retrieve_php_info.php script error: " ${ZAPI_TEMP_RUN_PHP_INFO_OUTPUT})
endif()

if (ZAPI_TEMP_RUN_PHP_INFO_OUTPUT LESS 70000)
    message(FATAL_ERROR "sorry, zendAPI just support PHP version 7.0.0 or above, please upgrade PHP.")
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

set(ZAPI_PHP_INCLUDE_PATHS ${ZAPI_PHP_INCLUDE_PATH})

# here we just add subdirectory of php include path to compiler include paths
foreach(zapi_temp_include_path ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT})
    string(SUBSTRING ${zapi_temp_include_path} 2 -1 zapi_temp_include_path)
    list(APPEND ZAPI_PHP_INCLUDE_PATHS ${zapi_temp_include_path})
endforeach()
include_directories(BEFORE ${ZAPI_PHP_INCLUDE_PATHS})
