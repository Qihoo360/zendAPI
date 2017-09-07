--TEST--
ObjectVariant invoke test
--FILE--
<?php

if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->forwardInvoke();
}

?>
--EXPECT--
begin invoke ObjectVariant::classInvoke : the text is xxx
ObjectVariantClass::__invoke invoked
