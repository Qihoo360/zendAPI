--TEST--
Class polymorphic test
--FILE--
<?php

function print_info(A $obj)
{
    $obj->printInfo();
}

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    print_info(new A);
    print_info(new B);
    print_info(new C);
}

?>
--EXPECT--
class A and class B and class C exist
A::printInfo been called
B::printInfo been called
C::printInfo been called
B::printInfo been called
B::showSomething been called
