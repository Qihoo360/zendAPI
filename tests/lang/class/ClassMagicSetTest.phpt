--TEST--
Class magic set test
--FILE--
<?php

if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    $magicMethodObject = new MagicMethodClass();
    if (!property_exists($magicMethodObject, "address")) {
        echo "\$magicMethodObject->address is not exist\n";
    }
    $magicMethodObject->address = "beijing";
    if (property_exists($magicMethodObject, "address")) {
        echo "\$magicMethodObject->address is exist\n";
        echo "\$magicMethodObject->address value is {$magicMethodObject->address}\n";
    }
}

?>
--EXPECT--
MagicMethodClass::__isset is called
$magicMethodObject->address is not exist
MagicMethodClass::__set is called
MagicMethodClass::__isset is called
$magicMethodObject->address is exist
MagicMethodClass::__get is called
$magicMethodObject->address value is beijing
