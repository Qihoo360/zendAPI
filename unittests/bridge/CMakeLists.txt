set(BRIDGE_TEST_SRCS
   ExtensionTest.cpp
   IniEntryTest.cpp
   )
zapi_add_unittest(UnitTests BridgeTest ${BRIDGE_TEST_SRCS})

add_library(dummyext MODULE 
   DummyExtension.cpp
   ExtFuncs.h
   ExtFuncs.cpp)
target_link_libraries(dummyext ${PTHREAD_LIB} ${CMAKE_PROJECT_NAME})
set(BUILD_ETC_DIR ${ZAPI_BINARY_DIR}/etc)
set_target_properties(dummyext PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   LIBRARY_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   PREFIX ""
   )
set_target_properties(BridgeTest PROPERTIES
   COMPILE_DEFINITIONS
   PHP_INI_FILENAME="${ZAPI_BINARY_DIR}/etc/php.ini"
   )