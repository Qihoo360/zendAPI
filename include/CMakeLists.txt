set(PUBLIC_HEADERS
      zapi/lang/Type.h
      zapi/lang/Variant.h)

#set(PRIVATE_HEADER
#    PARENT_SCOPE)
set(PUBLIC_HEADERS ${PUBLIC_HEADERS} PARENT_SCOPE)
set(PRIVATE_HEADER ${PRIVATE_HEADER} PARENT_SCOPE)

# install header files
install(DIRECTORY zapi
        DESTINATION
        "${ZAPI_INSTALL_INCLUDE_DIR}"
        COMPONENT dev)