set(ZAPI_SRC
   Global.cpp
   bridge/Extension.cpp
   bridge/IniEntry.cpp
   kernel/Meta.cpp
   kernel/StreamBuffer.cpp
   kernel/FatalError.cpp
   lang/Variant.cpp
   lang/Class.cpp
   lang/Interface.cpp
   lang/Method.cpp
   lang/Parameters.cpp
   lang/Function.cpp
   lang/Argument.cpp
   lang/Constant.cpp
   lang/Namespace.cpp
   lang/Method.cpp
   lang/Property.cpp
   lang/StringMember.cpp
   lang/Type.cpp
   ds/String.cpp
   ds/HashTable.cpp
   ds/AbstractIterator.cpp
   vm/AbstractClass.cpp
   vm/AbstractMember.cpp
   vm/InvokeBridge.cpp
   vm/Callable.cpp
   vm/Engine.cpp
   utils/PhpFuncs.cpp)

zapi_add_library(${ZAPI_PROJECT_NAME} SHARED ${ZAPI_SRC} ${ZAPI_PUBLIC_HEADERS})

export(TARGETS ${ZAPI_PROJECT_NAME}
   FILE "${ZAPI_BINARY_DIR}/ZendApiTargets.cmake")
export(PACKAGE ${ZAPI_PROJECT_NAME})

file(RELATIVE_PATH
   REL_INCLUDE_DIR
   "${ZAPI_INSTALL_CMAKE_DIR}"
   "${ZAPI_INSTALL_INCLUDE_DIR}")

#
# Install targets
#
install(TARGETS ${ZAPI_PROJECT_NAME}
   EXPORT ZendApiTargets
   RUNTIME DESTINATION "${ZAPI_INSTALL_BIN_DIR}" COMPONENT bin
   LIBRARY DESTINATION "${ZAPI_INSTALL_LIB_DIR}" COMPONENT lib
   ARCHIVE DESTINATION "${ZAPI_INSTALL_LIB_DIR}" COMPONENT lib)

# Install exports for the install-tree.
install(EXPORT ZendApiTargets
   DESTINATION
   "${ZAPI_INSTALL_CMAKE_DIR}"
   COMPONENT dev)
