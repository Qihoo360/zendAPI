set(ZAPI_PUBLIC_HEADERS
    ${ZAPI_INCLUDE_DIR}/zapi/kernel/ExtensionBase.h
    ${ZAPI_INCLUDE_DIR}/zapi/bridge/Extension.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Type.h
    ${ZAPI_INCLUDE_DIR}/zapi/lang/Variant.h
    ${ZAPI_INCLUDE_DIR}/zapi/vm/Extension.h
    ${ZAPI_INCLUDE_DIR}/zapi/utils/HashTable.h
    ${ZAPI_INCLUDE_DIR}/zapi/utils/List.h
    ${ZAPI_INCLUDE_DIR}/zapi/utils/String.h)

set(ZAPI_PUBLIC_HEADERS ${ZAPI_PUBLIC_HEADERS} PARENT_SCOPE)
set(ZAPI_PRIVATE_HEADERS ${ZAPI_PRIVATE_HEADERS} PARENT_SCOPE)

# install header files
install(DIRECTORY zapi
        DESTINATION
        "${ZAPI_INSTALL_INCLUDE_DIR}"
        COMPONENT dev)