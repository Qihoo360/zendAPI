--TEST--
extension info test
--FILE--
<?php

$refl = new ReflectionExtension("dummyext");
var_dump($refl->getName());
var_dump($refl->getVersion());
?>
--EXPECT--
string(8) "dummyext"
string(3) "1.0"
