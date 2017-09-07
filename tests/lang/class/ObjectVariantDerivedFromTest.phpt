--TEST--
ObjectVariant derivedFrom method test
--FILE--
<?php

if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->forwardInvoke();
}

?>
--EXPECT--
