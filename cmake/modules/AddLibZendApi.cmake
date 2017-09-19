# zapi_add_library(name sources...
#   SHARED;STATIC
#     STATIC by default w/o BUILD_SHARED_LIBS.
#     SHARED by default w/  BUILD_SHARED_LIBS.
#   OBJECT
#     Also create an OBJECT library target. Default if STATIC && SHARED.
#   MODULE
#     Target ${name} might not be created on unsupported platforms.
#     Check with "if(TARGET ${name})".
#   OUTPUT_NAME name
#     Corresponds to OUTPUT_NAME in target properties.
#   DEPENDS targets...
#     Same semantics as add_dependencies().
#   LINK_COMPONENTS components...
#     Same as the variable LLVM_LINK_COMPONENTS.
#   LINK_LIBS lib_targets...
#     Same semantics as target_link_libraries().
#   ADDITIONAL_HEADERS
#     May specify header files for IDE generators.
#   )
function(zapi_add_library name)
   cmake_parse_arguments(ZAPI_ARG
      "MODULE;SHARED;STATIC;OBJECT"
      "OUTPUT_NAME"
      "ADDITIONAL_HEADERS;DEPENDS;LINK_COMPONENTS;LINK_LIBS;OBJLIBS"
      ${ARGN})
   list(APPEND ZAPI_COMMON_DEPENDS ${ZAPI_ARG_DEPENDS})
   if(ZAPI_ARG_ADDITIONAL_HEADERS)
      # Pass through ADDITIONAL_HEADERS.
      set(ZAPI_ARG_ADDITIONAL_HEADERS ADDITIONAL_HEADERS ${ZAPI_ARG_ADDITIONAL_HEADERS})
   endif()
   if(ZAPI_ARG_OBJLIBS)
      set(ALL_FILES ${ZAPI_ARG_OBJLIBS})
   else()
      list(APPEND ALL_FILES ${ZAPI_ARG_UNPARSED_ARGUMENTS} ${ZAPI_ARG_ADDITIONAL_HEADERS})
   endif()
   
   if(ZAPI_ARG_MODULE)
      if(ZAPI_ARG_SHARED OR ZAPI_ARG_STATIC)
         message(WARNING "MODULE with SHARED|STATIC doesn't make sense.")
      endif()
   else()
      if(BUILD_SHARED_LIBS AND NOT ZAPI_ARG_STATIC)
         set(ZAPI_ARG_SHARED TRUE)
      endif()
      if(NOT ZAPI_ARG_SHARED)
         set(ZAPI_ARG_STATIC TRUE)
      endif()
   endif()
   # Generate objlib
   if((ZAPI_ARG_SHARED AND ZAPI_ARG_STATIC) OR ZAPI_OBJECT)
      # Generate an obj library for both targets.
      set(obj_name "obj.${name}")
      add_library(${obj_name} OBJECT EXCLUDE_FROM_ALL
         ${ALL_FILES})
      set(ALL_FILES "$<TARGET_OBJECTS:${obj_name}>")
      # Do add_dependencies(obj) later due to CMake issue 14747.
      set_target_properties(${obj_name} PROPERTIES FOLDER "Object Libraries")
   endif()
   
   if(ZAPI_ARG_SHARED AND ZAPI_ARG_STATIC)
      # static
      set(name_static "${name}_static")
      if(ZAPI_ARG_OUTPUT_NAME)
         set(output_name OUTPUT_NAME "${ZAPI_ARG_OUTPUT_NAME}")
      endif()
      # DEPENDS has been appended to ZAPI_COMMON_LIBS.
      zapi_add_library(${name_static} STATIC
         ${output_name}
         OBJLIBS ${ALL_FILES} # objlib
         LINK_LIBS ${ZAPI_ARG_LINK_LIBS}
         LINK_COMPONENTS ${ZAPI_ARG_LINK_COMPONENTS})
      # FIXME: Add name_static to anywhere in TARGET ${name}'s PROPERTY.
      set(ZAPI_ARG_STATIC)
   endif()
   
   if(ZAPI_ARG_MODULE)
      add_library(${name} MODULE ${ALL_FILES})
      zapi_setup_rpath(${name})
   elseif(ZAPI_ARG_SHARED)
      add_library(${name} SHARED ${ALL_FILES})
      zapi_setup_rpath(${name})
   else()
      add_library(${name} STATIC ${ALL_FILES})
   endif()
   zapi_set_output_directory(${name}
      BINARY_DIR ${ZAPI_RUNTIME_OUTPUT_INTDIR}
      LIBRARY_DIR ${ZAPI_LIBRARY_OUTPUT_INTDIR})
   # $<TARGET_OBJECTS> doesn't require compile flags.
   if(ZAPI_ARG_OUTPUT_NAME)
      set_target_properties(${name}
         PROPERTIES
         OUTPUT_NAME ${ZAPI_ARG_OUTPUT_NAME})
   endif()
   
   if(ZAPI_ARG_MODULE)
      set_target_properties(${name}
         PROPERTIES
         PREFIX ""
         SUFFIX ${ZAPI_PLUGIN_EXT})
   endif()
   
   if(ZAPI_ARG_SHARED)
      if(WIN32)
         set_target_properties(${name} PROPERTIES
            PREFIX "")
      endif()
      # Set SOVERSION on shared libraries that lack explicit SONAME
      # specifier, on *nix systems that are not Darwin.
      if(UNIX)
         set_target_properties(${name}
            PROPERTIES
            # Since 1.0.0, the ABI version is indicated by the major version
            SOVERSION ${ZAPI_VERSION_MAJOR}
            VERSION ${ZAPI_VERSION_MAJOR}.${ZAPI_VERSION_MINOR}.${ZAPI_VERSION_PATCH})
      endif()
   endif()
   if (APPLE)
      # here when we compile zapi on osx, we do extra with linker flags -undefined dynamic_lookup
      set_property(TARGET ${name} APPEND_STRING PROPERTY
         LINK_FLAGS " -undefined dynamic_lookup ")
      target_link_libraries(${name} INTERFACE "-undefined dynamic_lookup")
   endif()
endfunction()

macro(zapi_add_executable name)
   cmake_parse_arguments(ZAPI_ARG "NO_INSTALL_RPATH" ""
      "DEPENDS" ${ARGN})
   set(ALL_FILES ${ZAPI_ARG_UNPARSED_ARGUMENTS})
   if(EXCLUDE_FROM_ALL)
      add_executable(${name} EXCLUDE_FROM_ALL ${ALL_FILES})
   else()
      add_executable(${name} ${ALL_FILES})
   endif()
   if(NOT ZAPI_ARG_NO_INSTALL_RPATH)
      zapi_setup_rpath(${name})
   endif()
   # Do not add -Dname_EXPORTS to the command-line when building files in this
   # target. Doing so is actively harmful for the modules build because it
   # creates extra module variants, and not useful because we don't use these
   # macros.
   set_target_properties( ${name} PROPERTIES DEFINE_SYMBOL "" )
   set(EXCLUDE_FROM_ALL OFF)
   zapi_set_output_directory(${name}
      BINARY_DIR ${ZAPI_RUNTIME_OUTPUT_INTDIR}
      LIBRARY_DIR ${ZAPI_LIBRARY_OUTPUT_INTDIR})
   if(ZAPI_COMMON_DEPENDS)
      add_dependencies(${name} ${ZAPI_COMMON_DEPENDS})
   endif()
endmacro()

function(zapi_add_unittest test_suite test_name)
   if(ZAPI_OPT_DISABLE_TESTS)
      set(EXCLUDE_FROM_ALL ON)
   endif()
   include_directories(${ZAPI_UTILS_DIR}/unittest/googletest/include)
   include_directories(${ZAPI_UTILS_DIR}/unittest/googlemock/include)
   include_directories(${ZAPI_PHP_INCLUDE_PATH})
   if (NOT ZAPI_OPT_ENABLE_THREADS)
      list(APPEND ZAPI_COMPILE_DEFINITIONS GTEST_HAS_PTHREAD=0)
   endif()
   zapi_add_executable(${test_name} NO_INSTALL_RPATH ${ARGN})
   set(outdir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
   zapi_set_output_directory(${test_name} BINARY_DIR ${outdir} LIBRARY_DIR ${outdir})
   # libpthreads overrides some standard library symbols, so main
   # executable must be linked with it in order to provide consistent
   # API for all shared libaries loaded by this executable.
   target_link_libraries(${test_name} gtest_main gtest ${PTHREAD_LIB} ${CMAKE_PROJECT_NAME}
      zapi_php_lib)
   add_dependencies(${test_suite} ${test_name})
   get_target_property(test_suite_folder ${test_suite} FOLDER)
   if (NOT ${test_suite_folder} STREQUAL "NOTFOUND")
      set_property(TARGET ${test_name} PROPERTY FOLDER "${test_suite_folder}")
   endif ()
endfunction()

function(zapi_setup_rpath name)
   if(CMAKE_INSTALL_RPATH)
      return()
   endif()
   if(ZAPI_OPT_INSTALL_PREFIX AND NOT (ZAPI_OPT_INSTALL_PREFIX STREQUAL CMAKE_INSTALL_PREFIX))
      set(extra_libdir ${ZAPI_OPT_INSTALL_PREFIX})
   elseif(ZAPI_OPT_INSTALL_PREFIX_DIR)
      set(extra_libdir ${ZAPI_OPT_INSTALL_PREFIX_DIR})
   endif()
   if(APPLE)
      set(_install_name_dir INSTALL_NAME_DIR "@rpath")
      set(_install_rpath "@load_path/../lib" ${extra_libdir})
   elseif(UNIX)
      set(_install_rpath "\$ORIGIN/../lib${ZAPI_OPT_LIBDIR_SUFFIX}" ${extra_libdir})
      if(${CMAKE_SYSTEM_NAME} MATCHES "(FreeBSD|DragonFly)")
         set_property(TARGET ${name} APPEND_STRING PROPERTY
            LINK_FLAGS " -Wl,-z,origin ")
      elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
         set_property(TARGET ${name} APPEND_STRING PROPERTY
            LINK_FLAGS " -Wl,-rpath-link,${ZAPI_LIBRARY_OUTPUT_INTDIR} ")
      endif()
   else()
      return()
   endif()
   set_target_properties(${name} PROPERTIES
      BUILD_WITH_INSTALL_RPATH ON
      INSTALL_RPATH "${_install_rpath}"
      ${_install_name_dir})
endfunction()

# Set each output directory according to ${CMAKE_CONFIGURATION_TYPES}.
# Note: Don't set variables CMAKE_*_OUTPUT_DIRECTORY any more,
# or a certain builder, for eaxample, msbuild.exe, would be confused.
function(zapi_set_output_directory target)
   cmake_parse_arguments(ZAPI_ARG "" "BINARY_DIR;LIBRARY_DIR" "" ${ARGN})
   
   # module_dir -- corresponding to LIBRARY_OUTPUT_DIRECTORY.
   # It affects output of add_library(MODULE).
   if(WIN32 OR CYGWIN)
      # DLL platform
      set(module_dir ${ZAPI_ARG_BINARY_DIR})
   else()
      set(module_dir ${ZAPI_ARG_LIBRARY_DIR})
   endif()
   if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      foreach(build_mode ${CMAKE_CONFIGURATION_TYPES})
         string(TOUPPER "${build_mode}" CONFIG_SUFFIX)
         if(ZAPI_ARG_BINARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} bi ${ZAPI_ARG_BINARY_DIR})
            set_target_properties(${target} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${bi})
         endif()
         if(ZAPI_ARG_LIBRARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} li ${ZAPI_ARG_LIBRARY_DIR})
            set_target_properties(${target} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${li})
         endif()
         if(module_dir)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} mi ${module_dir})
            set_target_properties(${target} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${mi})
         endif()
      endforeach()
   else()
      if(ZAPI_ARG_BINARY_DIR)
         set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ZAPI_ARG_BINARY_DIR})
      endif()
      if(ZAPI_ARG_LIBRARY_DIR)
         set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${ZAPI_ARG_LIBRARY_DIR})
      endif()
      if(module_dir)
         set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${module_dir})
      endif()
   endif()
endfunction()
