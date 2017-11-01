set(DUMMYEXT_SRCS
    Entry.cpp
    CycleHandlerTestcases.h
    CycleHandlerTestcases.cpp
    ClassTestcases.h
    ClassTestcases.cpp
    FunctionTestcases.h
    FunctionTestcases.cpp
    IniTestcases.h
    IniTestcases.cpp
    NamespaceTestcases.h
    NamespaceTestcases.cpp)

add_library(dummyext MODULE 
    ${DUMMYEXT_SRCS})

target_link_libraries(dummyext ${PTHREAD_LIB} ${CMAKE_PROJECT_NAME})
set(BUILD_ETC_DIR ${ZAPI_BINARY_DIR}/etc)
set_target_properties(dummyext PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   LIBRARY_OUTPUT_DIRECTORY ${ZAPI_BINARY_DIR}/extensions
   PREFIX ""
   )
