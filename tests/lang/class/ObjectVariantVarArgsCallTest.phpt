--TEST--
ObjectVariant Variant Args call test
--FILE--
<?php

if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->testVarArgsCall();
}

?>
--EXPECT--
ObjectVariantClass::printSum been called
got 3 args
the result is 36
ObjectVariantClass::calculateSum been called
got 3 args
the result is 7
the result of ObjectVariantClass::calculateSum is 7
