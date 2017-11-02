set(LANG_TEST_SRCS
    ArgumentTest.cpp
    FunctionTest.cpp
    ConstantTest.cpp
    TypeTest.cpp
    ExtensionTest.cpp
    NamespaceTest.cpp)
zapi_add_unittest(UnitTests LangTest ${LANG_TEST_SRCS})
