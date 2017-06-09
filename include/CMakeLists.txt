set(ZAPI_PUBLIC_HEADERS
    ${ZAPI_INCLUDE_DIR}/zapi/kernel/ExtensionBase.h
    ${ZAPI_INCLUDE_DIR}/zapi/bridge/Extension.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Type.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Variant.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Exception.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/FatalError.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/StdClass.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Parameters.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Errors.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/NotImplemented.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/OrigException.h
    ${ZAPI_INCLUDE_DIR}/zapi/vm/Extension.h
    ${ZAPI_INCLUDE_DIR}/zapi/vm/StdClassImpl.h
    ${ZAPI_INCLUDE_DIR}/zapi/ds/HashTable.h
    ${ZAPI_INCLUDE_DIR}/zapi/ds/List.h
    ${ZAPI_INCLUDE_DIR}/zapi/ds/String.h
    ${ZAPI_INCLUDE_DIR}/zapi/utils/Arithmetic.h
    ${ZAPI_INCLUDE_DIR}/zapi/ZendApi.h
    ${ZAPI_INCLUDE_DIR}/zapi/CompilerDetection.h
    ${ZAPI_INCLUDE_DIR}/zapi/Global.h
    ${ZAPI_INCLUDE_DIR}/zapi/ProcessorDetection.h
    ${ZAPI_INCLUDE_DIR}/zapi/SystemDetection.h)

set(ZAPI_PUBLIC_HEADERS ${ZAPI_PUBLIC_HEADERS} PARENT_SCOPE)
set(ZAPI_PRIVATE_HEADERS ${ZAPI_PRIVATE_HEADERS} PARENT_SCOPE)

# install header files
install(DIRECTORY zapi
        DESTINATION
        "${ZAPI_INSTALL_INCLUDE_DIR}"
        COMPONENT dev)