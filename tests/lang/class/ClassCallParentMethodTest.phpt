--TEST--
Class call parent method test
--FILE--
<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->printInfo();
}

?>
--EXPECT--
class A and class B and class C exist
C::printInfo been called
B::printInfo been called
B::showSomething been called
