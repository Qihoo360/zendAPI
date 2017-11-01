add_subdirectory(lang)
add_subdirectory(ext)
add_subdirectory(dummyext)

# just for show php test files
add_custom_target(ZapiRuntimeTests SOURCES ${ZAPI_PHP_TESTS})

# add the runtime tests
add_test(NAME ExtIniTest COMMAND ${ZAPI_PHP_EXECUTABLE} -c ${ZAPI_PHP_INI_FILENAME} ext/IniTest.phpt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME ExtCycleHandlerTest COMMAND ${ZAPI_PHP_EXECUTABLE} -c ${ZAPI_PHP_INI_FILENAME} ext/ExtensionHandlerTest.phpt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
