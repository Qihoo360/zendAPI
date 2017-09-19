--TEST--
ObjectVariant No Args call test
--FILE--
<?php

if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->testNoArgCall();
}

?>
--EXPECT--
ObjectVariantClass::printName been called
ObjectVariantClass::getName been called
the result of ObjectVariantClass::getName is hello, zapi
