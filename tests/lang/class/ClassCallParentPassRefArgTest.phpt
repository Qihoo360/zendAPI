--TEST--
Class call parent method pass ref argument test
--FILE--
<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->testCallParentPassRefArg();
}

?>
--EXPECT--

