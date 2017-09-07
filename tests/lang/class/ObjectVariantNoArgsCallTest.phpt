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
the result of ObjectVariantClass::getName is ObjectVariantClass::getName been called
hello, zapi
