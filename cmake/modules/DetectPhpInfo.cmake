# we detect the php execute binary path and the php version, Zend Engine API version

# we first search the newest php version
if (ZAPI_OPT_PHPCFG_PATH)
    set(ZAPI_PHP_CONFIG_EXECUABLE ${ZAPI_OPT_PHPCFG_PATH})
else()
    set(ZAPI_PHP_POSSIBLE_BIN_PATHS
        /usr/local/php/bin
        /usr/local/bin
        /usr/php/bin
        /usr/bin)
    find_program(ZAPI_PHP_CONFIG_EXECUABLE php-config
        HINTS ${ZAPI_PHP_POSSIBLE_BIN_PATHS} NO_DEFAULT_PATH)
endif()

if (NOT (ZAPI_PHP_CONFIG_EXECUABLE AND EXISTS ${ZAPI_PHP_CONFIG_EXECUABLE}))
    message(FATAL_ERROR "php-config script: ${ZAPI_PHP_CONFIG_EXECUABLE} is not found")
endif()

# if you build the unittest we will detect libphp library
if(ZAPI_OPT_ENABLE_UNIT_TESTS)
    execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --prefix
        RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
        OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
        ERROR_QUIET)
    if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
        message(FATAL_ERROR "run php-config error, unable to get php extension directory")
    endif()
    string(STRIP ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT} ZAPI_TEMP_RUN_PHPCFG_OUTPUT)
    set(ZAPI_PHP_LIB_PATH ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT}/lib)
    find_library(ZAPI_LIBPHP_PATH php7 NAEMS php php5
        PATHS ${ZAPI_PHP_LIB_PATH} NO_DEFAULT_PATH)
    if (NOT ZAPI_LIBPHP_PATH)
        message(FATAL_ERROR "php library is not found automatic, maybe your php is not in system standard path "
            "you can specify ZAPI_OPT_PHPCFG_PATH and try again, good luck!")
    endif()
    add_library(zapi_libphp SHARED IMPORTED GLOBAL)
    set_target_properties(zapi_libphp
        PROPERTIES
        IMPORTED_LOCATION ${ZAPI_LIBPHP_PATH})
endif()

# we use php-config to detect php extra include paths
execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --includes
    RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
    OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
    ERROR_QUIET)
if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
    message(FATAL_ERROR "run php-config error, unable to get php include directories")
endif()

string(REPLACE " " ";" ZAPI_TEMP_RUN_PHPCFG_OUTPUT ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT})

set(ZAPI_PHP_INCLUDE_PATHS "")

# here we just add subdirectory of php include path to compiler include paths
foreach(zapi_temp_include_path ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT})
    string(SUBSTRING ${zapi_temp_include_path} 2 -1 zapi_temp_include_path)
    string(STRIP ${zapi_temp_include_path} zapi_temp_include_path)
    list(APPEND ZAPI_PHP_INCLUDE_PATHS ${zapi_temp_include_path})
endforeach()

list(GET ZAPI_PHP_INCLUDE_PATHS 0 ZAPI_PHP_INCLUDE_PATH)
string(FIND ${ZAPI_PHP_INCLUDE_PATH} "/" ZAPI_TEMP_LAST_POS REVERSE)
string(SUBSTRING ${ZAPI_PHP_INCLUDE_PATH} 0 ${ZAPI_TEMP_LAST_POS} ZAPI_PHP_INCLUDE_PATH)

list(INSERT ZAPI_PHP_INCLUDE_PATHS 0 ${ZAPI_PHP_INCLUDE_PATH})

execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --php-binary
    RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
    OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
    ERROR_QUIET)
if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
    message(FATAL_ERROR "run php-config error, unable to get php include directories")
endif()
string(STRIP ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT} ZAPI_PHP_EXECUTABLE)

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

include_directories(BEFORE "${ZAPI_PHP_INCLUDE_PATHS}")

# we use php-config to detect php extension path
execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --extension-dir
    RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
    OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
    ERROR_QUIET)
if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
    message(FATAL_ERROR "run php-config error, unable to get php extension directory")
endif()
string(STRIP ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT} ZAPI_PHP_EXTENSION_DIR)

execute_process(COMMAND ${ZAPI_PHP_CONFIG_EXECUABLE} --php-sapis
    RESULT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_RET
    OUTPUT_VARIABLE ZAPI_TEMP_RUN_PHPCFG_OUTPUT
    ERROR_QUIET)
if (ZAPI_TEMP_RUN_PHPCFG_RET EQUAL 1)
    message(FATAL_ERROR "run php-config error, unable to get php include directories")
endif()
string(STRIP ${ZAPI_TEMP_RUN_PHPCFG_OUTPUT} ZAPI_PHP_SUPPORT_SAPIS)
