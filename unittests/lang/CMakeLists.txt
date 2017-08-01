set(LANG_TEST_SRCS
    VariantTest.cpp
    ArgumentTest.cpp
    FunctionTest.cpp
    ConstantTest.cpp
    TypeTest.cpp)
zapi_add_unittest(UnitTests LangTest ${LANG_TEST_SRCS})