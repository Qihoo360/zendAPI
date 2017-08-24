set(VM_TEST_SRCS
    EngineTest.cpp
    InvokeBridgeTest.cpp)
zapi_add_unittest(UnitTests VmTest ${VM_TEST_SRCS})