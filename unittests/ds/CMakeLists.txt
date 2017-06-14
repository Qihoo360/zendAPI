# data structure tests
set(DS_TEST_SRCS
    StringTest.cpp
    HashTableTest.cpp)
zapi_add_unittest(UnitTests DsTest ${DS_TEST_SRCS})