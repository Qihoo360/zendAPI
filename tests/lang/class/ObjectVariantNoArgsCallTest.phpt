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
