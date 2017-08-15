# data structure tests
set(DS_TEST_SRCS
    BoolVariantTest.cpp
    NumericVariantTest.cpp
    StringVariantTest.cpp
    ArrayVariantTest.cpp)
zapi_add_unittest(UnitTests DsTest ${DS_TEST_SRCS})