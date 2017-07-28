set(LANG_TEST_SRCS
    VariantTest.cpp
    FunctionTest.cpp)
zapi_add_unittest(UnitTests LangTest ${LANG_TEST_SRCS})