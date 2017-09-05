--TEST--
Class magic clone test
--FILE--
<?php

if (class_exists("\MagicMethodClass")) {
    $object = new \MagicMethodClass();
    $object1 = clone $object;
    //var_dump($object);
    //var_dump($object1);
}
?>
--EXPECT--
MagicMethodClass::__clone is called
