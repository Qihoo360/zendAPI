--TEST--
Class inherit test
--FILE--
<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    if (new B instanceof \A) {
       echo "object of class B is instance of A\n"; 
    }
    $reflClsB = new ReflectionClass("B");
    $parentOfClsB = $reflClsB->getParentClass();
    if ($parentOfClsB->getName() == "A") {
        echo "The parent class of class B is class A\n";
    }
    $reflClsC = new ReflectionClass("C");
    $parentOfClsC = $reflClsC->getParentClass();
    if ($parentOfClsC->getName() == "B") {
        echo "The parent class of class C is class B\n";
    }
    if (new C instanceof \B) {
       echo "object of class C is instance of B\n"; 
    }
    if (new C instanceof \A) {
       echo "object of class C is instance of A\n"; 
    }
}

?>
--EXPECT--
class A and class B and class C exist
object of class B is instance of A
The parent class of class B is class A
The parent class of class C is class B
object of class C is instance of B
object of class C is instance of A
