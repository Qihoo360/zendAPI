--TEST--
Class magic get test
--FILE--
<?php

if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    $nonMagicMethodObject = new NonMagicMethodClass();
    // $nonMagicMethodObject->prop1; Notice: Undefined property: NonMagicMethodClass::$prop1
    if (!property_exists($nonMagicMethodObject, "prop1")) {
        echo "\$nonMagicMethodObject->prop1 is not exist\n";
    }
    $magicMethodObject = new MagicMethodClass();
    if (property_exists($magicMethodObject, "prop1")) {
        echo "\$magicMethodObject->prop1 is exist\n";
        echo "the value of \$magicMethodObject->prop1 is ".$magicMethodObject->prop1."\n";
    }
}

?>
--EXPECT--
$nonMagicMethodObject->prop1 is not exist
$magicMethodObject->prop1 is exist
the value of $magicMethodObject->prop1 is zapi
