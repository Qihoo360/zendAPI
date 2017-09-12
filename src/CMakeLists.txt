set(ZAPI_SRC
   Global.cpp
   kernel/Meta.cpp
   kernel/StreamBuffer.cpp
   kernel/FatalError.cpp
   kernel/Exception.cpp
   protocol/AbstractIterator.cpp
   protocol/ArrayAccess.cpp
   lang/Extension.cpp
   lang/Ini.cpp
   lang/Class.cpp
   lang/Interface.cpp
   lang/Method.cpp
   lang/Parameters.cpp
   lang/Function.cpp
   lang/Argument.cpp
   lang/Constant.cpp
   lang/Namespace.cpp
   lang/Method.cpp
   lang/Type.cpp
   lang/StdClass.cpp
   ds/Variant.cpp
   ds/StringVariant.cpp
   ds/BoolVariant.cpp
   ds/NumericVariant.cpp
   ds/DoubleVariant.cpp
   ds/ObjectVariant.cpp
   ds/CallableVariant.cpp
   ds/ArrayVariant.cpp
   ds/ArrayItemProxy.cpp
   vm/AbstractClass.cpp
   vm/AbstractMember.cpp
   vm/ZValMember.cpp
   vm/StringMember.cpp
   vm/BoolMember.cpp
   vm/FloatMember.cpp
   vm/NumericMember.cpp
   vm/NullMember.cpp
   vm/Property.cpp
   vm/InvokeBridge.cpp
   vm/Callable.cpp
   vm/Engine.cpp
   vm/IteratorBridge.cpp
   vm/ObjectBinder.cpp
   vm/Closure.cpp
   utils/PhpFuncs.cpp
   utils/CommonFuncs.cpp)

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
