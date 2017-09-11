set(ZAPI_PHP_TESTS 
    ${ZAPI_PHP_TESTS}
    
    ext/ExtensionInfoTest.phpt
    ext/IniTest.phpt
    
    lang/const/ConstantTypeTest.phpt
    lang/const/ConstantExistTest.phpt
    lang/const/ConstantNsTest.phpt
    lang/func/FunctionExistTest.phpt
    lang/func/FunctionDefaultTest.phpt
    lang/func/FunctionArgsTest.phpt
    lang/func/FunctionDefaultArgTest.phpt
    lang/func/FunctionRefArgTest.phpt
    lang/func/FunctionReturnTest.phpt
    lang/func/FunctionVarArgsTest.phpt
    
    lang/interface/InterfaceImplementTest.phpt
    lang/interface/InterfaceInheritTest.phpt
    lang/interface/InterfaceExistTest.phpt
    
    lang/class/ClassExistTest.phpt
    lang/class/ClassArrayAccessTest.phpt
    lang/class/ClassCallParentMethodTest.phpt
    lang/class/ClassCountTest.phpt
    lang/class/ClassConstantTest.phpt
    lang/class/ClassConstantTypeTest.phpt
    lang/class/ClassInheritTest.phpt
    lang/class/ClassPolymorphicTest.phpt
    lang/class/ClassIteratorTest.phpt
    lang/class/ClassStaticMethodExistTest.phpt
    lang/class/ClassCallParentPassRefArgTest.phpt
    lang/class/ClassCallParentMethodWithReturnTest.phpt
    lang/class/ClassInheritPropertyTest.phpt
    lang/class/ClassGetVariantPtrTest.phpt
    lang/class/ClassPropertyVisibilityTest.phpt
    lang/class/ClassMethodVisibilityTest.phpt
    lang/class/ClassFinalTest.phpt
    lang/class/ClassMethodExistTest.phpt
    lang/class/ClassPropertyTest.phpt
    lang/class/ClassPropertyTypeTest.phpt
    lang/class/ClassStaticPropertyTest.phpt
    lang/class/ClassPropGetterAndSetterTest.phpt
    lang/class/ClassMethodAccessLevelTest.phpt
    lang/class/ClassMethodInvokeTest.phpt
    
    lang/class/ClassMagicCastTest.phpt
    lang/class/ClassMagicCallTest.phpt
    lang/class/ClassMagicCallStaticTest.phpt
    lang/class/ClassMagicCloneTest.phpt
    lang/class/ClassMagicInvokeTest.phpt
    lang/class/ClassMagicConstructAndDestructTest.phpt
    lang/class/ClassMagicGetTest.phpt
    lang/class/ClassMagicIssetTest.phpt
    lang/class/ClassMagicSetTest.phpt
    lang/class/ClassMagicUnsetTest.phpt
    lang/class/ClassMagicSerializeTest.phpt
    lang/class/ClassMagicCompareTest.phpt
    lang/class/ClassMagicDebugInfoTest.phpt
    
    lang/class/ObjectVariantInvokeTest.phpt
    lang/class/ObjectVariantInstanceOfTest.phpt
    lang/class/ObjectVariantDerivedFromTest.phpt
    lang/class/ObjectVariantNoArgsCallTest.phpt
    lang/class/ObjectVariantVarArgsCallTest.phpt
    
    PARENT_SCOPE)
