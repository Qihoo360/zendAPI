--TEST--
Class call parent method with return test
--FILE--
<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->testCallParentWithReturn();
}

?>
--EXPECT--
class A and class B and class C exist
C::testCallParentWithReturn been called
B::addTwoNumber been called
after call addTwoNumber get : 24
