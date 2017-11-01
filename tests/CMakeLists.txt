add_subdirectory(lang)
add_subdirectory(ext)
add_subdirectory(dummyext)

# just for show php test files
add_custom_target(ZapiRuntimeTests SOURCES ${ZAPI_PHP_TESTS})

set(ZAPI_RT_TESTCASES 
    ext/IniTest.phpt
    ext/ExtensionHandlerTest.phpt
    ext/ExtensionInfoTest.phpt)

file(GLOB_RECURSE ZAPI_RT_TESTCASES
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.phpt)

# add the runtime tests
foreach(testcase ${ZAPI_RT_TESTCASES})
    add_test(NAME ${testcase} COMMAND ${ZAPI_PHP_EXECUTABLE} -c ${ZAPI_PHP_INI_FILENAME} ${testcase}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
