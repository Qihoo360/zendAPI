cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(zapi CXX)

set(ZAPI_PROJECT_NAME zapi)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(ZAPI_VERSION 0.0.1)
set(ZAPI_VERSION_MAJOR 0)
set(ZAPI_VERSION_MINOR 0)
set(ZAPI_VERSION_PATCH 1)
set(ZAPI_VERSION_STAGE "devel")
# the ZAPI api version, is release date number
set(ZAPI_API_VERSION 20170605)

set(ZAPI_ABI_MAJOR ${ZAPI_VERSION_MAJOR})
set(ZAPI_ABI_MINOR ${ZAPI_VERSION_MINOR})
set(ZAPI_ABI_PATCH ${ZAPI_VERSION_PATCH})

set(ZAPI_ABI_VERSION
    "{ZAPI_ABI_MAJOR}.${ZAPI_ABI_MINOR}.${ZAPI_ABI_PATCH}")
set(ZAPI_PACKAGE_VERSION
    "${ZAPI_VERSION_MAJOR}.${ZAPI_VERSION_MINOR}.${ZAPI_VERSION_PATCH}")
set(ZAPI_NUMERIC_VERSION 0x000001)
set(ZAPI_STAGE_NAME ${ZAPI_VERSION_STAGE})

set(ZAPI_ALLOWED_STAGE_NAMES
    rc
    beta
    alpha
    devel
    release
    stable)

list(FIND ZAPI_ALLOWED_STAGE_NAMES
    "${ZAPI_STAGE_NAME}" ZAPI_TEMP_STAGE_RET)

if(ZAPI_TEMP_STAGE_RET EQUAL -1)
    message(WARNING "stage ${NOTIFY_STAGE_NAME}  is not allowed, reset to be beta")
    set(ZAPI_STAGE_NAME beta)
endif()

set(ZAPI_VERSION
    "${ZAPI_VERSION_MAJOR}.${ZAPI_VERSION_MINOR}.${ZAPI_VERSION_PATCH}-${ZAPI_VERSION_STAGE}")

set(ZAPI_WORKSPACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ZAPI_UTILS_DIR "${ZAPI_WORKSPACE_DIR}/utils")
set(ZAPI_SOURCE_DIR "${ZAPI_WORKSPACE_DIR}/src")
set(ZAPI_ASSETS_DIR "${ZAPI_WORKSPACE_DIR}/assets")
set(ZAPI_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(ZAPI_CMAKE_DIR "${ZAPI_WORKSPACE_DIR}/cmake/modules")
set(ZAPI_INCLUDE_DIR "${ZAPI_WORKSPACE_DIR}/include")
set(ZAPI_SCRIPTS_DIR "${ZAPI_WORKSPACE_DIR}/scripts")

set(ZAPI_EXTRA_INCLUDE_DIR "${ZAPI_BINARY_DIR}/include")
set(ZAPI_RUNTIME_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin")
set(ZAPI_LIBRARY_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")
set(ZAPI_ARCHIVE_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ZAPI_OPT_DISABLE_TESTS
    "If tests should be compiled or not" OFF)
option(ZAPI_OPT_ENABLE_VERBOSE_DEBUG
    "Enable verbose debugging" OFF)
option(ZAPI_OPT_DISABLE_TESTS
    "If tests should be compiled or not" OFF)

set(ZAPI_OPT_PHP_ROOT_PATH "" CACHE STRING "Specify the root php path of host platform")
set(ZAPI_OPT_PHP_INCLUDE_PATH "" CACHE STRING "Specify the php include path of host platform")
set(ZAPI_OPT_PHP_LIB_PATH "" CACHE STRING "Specify the php lib path of host platform")
set(ZAPI_OPT_PHP_BIN_PATH "" CACHE STRING "Specify the php bin path of host platform")

if (ZAPI_OPT_PHP_ROOT_PATH)
    if (NOT ZAPI_OPT_PHP_INCLUDE_PATH)
        set(ZAPI_OPT_PHP_INCLUDE_PATH ${ZAPI_OPT_PHP_ROOT_PATH}/include)
    endif()
    if (NOT ZAPI_OPT_PHP_LIB_PATH)
        set(ZAPI_OPT_PHP_LIB_PATH ${ZAPI_OPT_PHP_ROOT_PATH}/lib)
    endif()
    if (NOT ZAPI_OPT_PHP_BIN_PATH)
        set(ZAPI_OPT_PHP_BIN_PATH ${ZAPI_OPT_PHP_ROOT_PATH}/bin)
    endif()
endif()

set(ZAPI_OPT_LIBDIR_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
string(TOUPPER "${CMAKE_BUILD_TYPE}" ZAPI_BUILD_TYPE)

if(ZAPI_OPT_ENABLE_VERBOSE_DEBUG)
    add_definitions(-DZAPI_DEBUG_MODE 1)
endif()

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-std=gnu++11 -Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_CXX_FLAGS STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    endif ()
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wa,-q -Wno-macro-redefined ${CMAKE_CXX_FLAGS}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "-Xlinker --allow-shlib-undefined ${CMAKE_EXE_LINKER_FLAGS}")
endif()

# prepare for installation
set(ZAPI_INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(ZAPI_INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(ZAPI_INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
    set(ZAPI_INSTALL_CMAKE_DIR_TMP cmake)
else()
    set(ZAPI_INSTALL_CMAKE_DIR_TMP lib/cmake/zendapi)
endif()

set(ZAPI_INSTALL_CMAKE_DIR ${ZAPI_INSTALL_CMAKE_DIR_TMP}
    CACHE PATH "Installation directory for CMake files")

foreach(dirname LIB BIN INCLUDE CMAKE)
    set(var ZAPI_INSTALL_${dirname}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

include(AddLibZendApi)
include(DetectPhpInfo)

configure_file(
    ${ZAPI_INCLUDE_DIR}/zapi/Config.h.in
    ${ZAPI_EXTRA_INCLUDE_DIR}/zapi/Config.h
    NEWLINE_STYLE UNIX)

configure_file(
    ${ZAPI_INCLUDE_DIR}/zapi/Version.h.in
    ${ZAPI_EXTRA_INCLUDE_DIR}/zapi/Version.h
    NEWLINE_STYLE UNIX)

include_directories(
    BEFORE
    ${ZAPI_INCLUDE_DIR}
    ${ZAPI_EXTRA_INCLUDE_DIR})

# variable about header files
set(ZAPI_PUBLIC_HEADERS "")
set(ZAPI_PRIVATE_HEADERS "")

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(scripts)

if(NOT ZAPI_OPT_DISABLE_TESTS)
    set(ZAPI_PHP_INI_TPL_FILENAME ${ZAPI_ASSETS_DIR}/php.ini.in)
    set(ZAPI_ETC_DIR ${ZAPI_BINARY_DIR}/etc)
    set(ZAPI_EXTENSION_DIR ${ZAPI_BINARY_DIR}/extensions)
    set(ZAPI_EXTENSIONS "extension=dummyext.so")
    set(ZAPI_PHP_INI_FILENAME ${ZAPI_ETC_DIR}/php.ini)
    configure_file(${ZAPI_PHP_INI_TPL_FILENAME} ${ZAPI_PHP_INI_FILENAME} @ONLY)
    add_subdirectory(utils/unittest)
    add_subdirectory(tests)
    add_subdirectory(unittests)
endif()

set(ZAPI_INCLUDE_DIRS_TEMP
    ${ZAPI_INCLUDE_DIR}
    ${ZAPI_EXTRA_INCLUDE_DIR})

set(ZAPI_INCLUDE_DIRS
    ${ZAPI_INCLUDE_DIRS_TEMP} CACHE PATH "ZendApi include directories")

configure_file(${ZAPI_CMAKE_DIR}/ZendApiConfigBuildTree.cmake.in
    ${ZAPI_BINARY_DIR}/ZendApiConfig.cmake
    @ONLY)

configure_file(${ZAPI_CMAKE_DIR}/ZendApiConfig.cmake.in
    ${ZAPI_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/ZendApiConfig.cmake
    @ONLY)

# Generate version info for both build-tree and install-tree.
configure_file(${ZAPI_CMAKE_DIR}/ZendApiConfigVersion.cmake.in
    ${ZAPI_BINARY_DIR}/ZendApiConfigVersion.cmake
    @ONLY)

install(FILES
    ${ZAPI_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/ZendApiConfig.cmake
    ${ZAPI_BINARY_DIR}/ZendApiConfigVersion.cmake
    DESTINATION
    "${ZAPI_INSTALL_CMAKE_DIR}"
    COMPONENT dev)

message(STATUS "--------------------------------------------------------------------------------------")
message(STATUS "Thank for using zendAPI project, have a lot of fun!")
message(STATUS "--------------------------------------------------------------------------------------")
message(STATUS "ZAPI_VERSION: ${ZAPI_VERSION}")
message(STATUS "PHP_INCLUDE_PATH: ${ZAPI_PHP_INCLUDE_PATH}")
message(STATUS "PHP_LIB_PATH: ${ZAPI_PHP_LIB}")
message(STATUS "PHP_EXECUTABLE: ${ZAPI_PHP_EXECUTABLE}")
message(STATUS "PHP_CONFIG_EXECUABLE: ${ZAPI_PHP_CONFIG_EXECUABLE}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_COMMAND: ${CMAKE_COMMAND}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_ROOT: ${CMAKE_ROOT}")
message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SKIP_RPATH: ${CMAKE_SKIP_RPATH}")
message(STATUS "CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_AR: ${CMAKE_AR}")
message(STATUS "CMAKE_RANLIB: ${CMAKE_RANLIB}")
message(STATUS "--------------------------------------------------------------------------------------")
