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
set(ZAPI_API_VERSION 20170927)

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

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(ZAPI_BUILD_EMBED_MOED ON)
endif()

set(ZAPI_WORKSPACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ZAPI_UTILS_DIR "${ZAPI_WORKSPACE_DIR}/utils")
set(ZAPI_SOURCE_DIR "${ZAPI_WORKSPACE_DIR}/src")
set(ZAPI_ASSETS_DIR "${ZAPI_WORKSPACE_DIR}/assets")
set(ZAPI_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(ZAPI_CMAKE_DIR "${ZAPI_WORKSPACE_DIR}/cmake/modules")
set(ZAPI_INCLUDE_DIR "${ZAPI_WORKSPACE_DIR}/include")
set(ZAPI_SCRIPTS_DIR "${ZAPI_WORKSPACE_DIR}/scripts")
set(ZAPI_PHP_EXTENSION_DIR "")

set(ZAPI_EXTRA_INCLUDE_DIR "${ZAPI_BINARY_DIR}/include")
set(ZAPI_RUNTIME_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin")
set(ZAPI_LIBRARY_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")
set(ZAPI_ARCHIVE_OUTPUT_INTDIR "${ZAPI_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ZAPI_OPT_ENABLE_UNIT_TESTS
    "If tests should be compiled or not" ON)

option(ZAPI_OPT_ENABLE_RUNTIME_TESTS
    "If runtime tests should be compiled or not" ON)

option(ZAPI_OPT_ENABLE_VERBOSE_DEBUG
    "Enable verbose debugging" OFF)
set(ZAPI_OPT_PHPCFG_PATH "" CACHE STRING "Specify the php-config path of host platform.")
set(ZAPI_OPT_LIBDIR_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
string(TOUPPER "${CMAKE_BUILD_TYPE}" ZAPI_BUILD_TYPE)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_CXX_FLAGS STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    endif ()
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wno-macro-redefined ${CMAKE_CXX_FLAGS}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

string(REPLACE " " ";" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "-Xlinker --allow-shlib-undefined ${CMAKE_EXE_LINKER_FLAGS}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# prepare for installation
set(ZAPI_INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(ZAPI_INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(ZAPI_INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
    set(ZAPI_INSTALL_CMAKE_DIR_TMP cmake)
else()
    set(ZAPI_INSTALL_CMAKE_DIR_TMP lib/cmake/zapi)
endif()

set(ZAPI_INSTALL_CMAKE_DIR ${ZAPI_INSTALL_CMAKE_DIR_TMP}
    CACHE PATH "Installation directory for CMake files")

foreach(dirname LIB BIN INCLUDE CMAKE)
    set(var ZAPI_INSTALL_${dirname}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# setup for debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug") #  OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
    set(ZAPI_DEBUG TRUE)
    if(ZAPI_OPT_ENABLE_VERBOSE_DEBUG)
        set(ZAPI_VERBOSE_DEBUG TRUE)
    endif()
endif()

# if we are in embed mode, setup force setup some variables and options
if()
endif()

include(AddLibZendApi)
include(DetectPhpInfo)
include(FindGTest)

set(ZAPI_FOUND_NATIVE_GTEST OFF)
find_package(GTest)
if(GTEST_FOUND)
    set(ZAPI_FOUND_NATIVE_GTEST ON)
endif()

# check min compiler version
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.5.2")
    message(FATAL_ERROR "zendAPI only support clang 3.5.2 or above version.")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_HOST_APPLE)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.4.0")
            message(FATAL_ERROR "zendAPI only support gcc 5.4.0 or above version on MacOS platform.")
        endif()
    else()
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.5")
            message(FATAL_ERROR "zendAPI only support gcc 4.8.5 or above version.")
        endif()
    endif()
endif()

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

## for runtime test
if ((CMAKE_BUILD_TYPE STREQUAL "Debug" AND ZAPI_OPT_ENABLE_UNIT_TESTS) OR ZAPI_OPT_ENABLE_RUNTIME_TESTS)
    set(ZAPI_PHP_INI_TPL_FILENAME ${ZAPI_ASSETS_DIR}/php.ini.in)
    set(ZAPI_ETC_DIR ${ZAPI_BINARY_DIR}/etc)
    set(ZAPI_EXTENSION_DIR ${ZAPI_BINARY_DIR}/extensions)
    set(ZAPI_EXTENSIONS "extension=dummyext.so")
    set(ZAPI_PHP_INI_FILENAME ${ZAPI_ETC_DIR}/php.ini)
    configure_file(${ZAPI_PHP_INI_TPL_FILENAME} ${ZAPI_PHP_INI_FILENAME} @ONLY)    
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND ZAPI_OPT_ENABLE_UNIT_TESTS)
    if(NOT ZAPI_FOUND_NATIVE_GTEST)
        add_subdirectory(utils/unittest)
    endif()
    add_subdirectory(unittests)
endif()

if(ZAPI_OPT_ENABLE_RUNTIME_TESTS)
    enable_testing()
    ## normal test
    add_subdirectory(tests)
endif()

set(ZAPI_INCLUDE_DIRS_TEMP
    ${ZAPI_INCLUDE_DIR}
    ${ZAPI_EXTRA_INCLUDE_DIR})

set(ZAPI_INCLUDE_DIRS
    ${ZAPI_INCLUDE_DIRS_TEMP} CACHE PATH "ZendApi include directories")

configure_file(${ZAPI_CMAKE_DIR}/zendAPIConfigBuildTree.cmake.in
    ${ZAPI_BINARY_DIR}/zendAPIConfig.cmake
    @ONLY)

configure_file(${ZAPI_CMAKE_DIR}/zendAPIConfig.cmake.in
    ${ZAPI_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/zendAPIConfig.cmake
    @ONLY)

# Generate version info for both build-tree and install-tree.
configure_file(${ZAPI_CMAKE_DIR}/zendAPIConfigVersion.cmake.in
    ${ZAPI_BINARY_DIR}/zendAPIConfigVersion.cmake
    @ONLY)

install(FILES
    ${ZAPI_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/zendAPIConfig.cmake
    ${ZAPI_BINARY_DIR}/zendAPIConfigVersion.cmake
    DESTINATION
    "${ZAPI_INSTALL_CMAKE_DIR}"
    COMPONENT dev)

if(BUILD_SHARED_LIBS)
    set(ZAPI_BUILD_LIB_TYPE Shared)
else()
    set(ZAPI_BUILD_LIB_TYPE Static)
endif()

message(STATUS "--------------------------------------------------------------------------------------")
message(STATUS "Thank for using zendAPI project, have a lot of fun!")
message(STATUS "--------------------------------------------------------------------------------------")
message(STATUS "ZAPI_VERSION: ${ZAPI_VERSION}")
message(STATUS "ZAPI_BUILD_LIB_TYPE: ${ZAPI_BUILD_LIB_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "PHP_INCLUDE_PATH: ${ZAPI_PHP_INCLUDE_PATH}")
message(STATUS "PHP_LIB_PATH: ${ZAPI_PHP_LIB_PATH}")
message(STATUS "PHP_EXECUTABLE: ${ZAPI_PHP_EXECUTABLE}")
message(STATUS "PHP_CONFIG_EXECUABLE: ${ZAPI_PHP_CONFIG_EXECUABLE}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_ROOT: ${CMAKE_ROOT}")
message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SKIP_RPATH: ${CMAKE_SKIP_RPATH}")
message(STATUS "CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "--------------------------------------------------------------------------------------")
