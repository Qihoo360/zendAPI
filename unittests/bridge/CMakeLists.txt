set(BRIDGE_TEST_SRCS
   ExtensionTest.cpp
   )
zapi_add_unittest(UnitTests BridgeTest ${BRIDGE_TEST_SRCS})

add_library(dummyext MODULE DummyExtension.cpp)
target_link_libraries(dummyext ${PTHREAD_LIB} ${CMAKE_PROJECT_NAME}
   zapi_php_lib)
set(BUILD_ETC_DIR ${ZAPI_BINARY_DIR}/etc)
set_target_properties(dummyext PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   LIBRARY_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   PREFIX ""
   )
set_target_properties(BridgeTest PROPERTIES
   COMPILE_DEFINITIONS
   PHP_INI_DIR="${ZAPI_BINARY_DIR}/etc/php.ini"
   )
file(WRITE ${BUILD_ETC_DIR}/php.ini extension_dir="${ZAPI_BINARY_DIR}/extensions"\n
   extension=dummyext.so)