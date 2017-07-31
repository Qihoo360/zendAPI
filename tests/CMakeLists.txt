set(ZAPI_PHP_TESTS
   entry.php)
add_subdirectory(lang)
add_custom_target(PhpTests ALL
   COMMAND ${ZAPI_PHP_EXECUTABLE} -c ${ZAPI_PHP_INI_FILENAME} -f entry.php
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   SOURCES ${ZAPI_PHP_TESTS})