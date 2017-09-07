--TEST--
ObjectVariant derived from method test
--FILE--
<?php

if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->testDerivedFrom();
}

?>
--EXPECT--
A is not derived from A
B is not derived from B
C is not derived from C
B is derived from A
C is derived from B
C is derived from A
A is not derived from B
C is not derived from B
C is not derived from A
