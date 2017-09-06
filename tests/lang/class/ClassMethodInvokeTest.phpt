--TEST--
Class method invoke test
--FILE--
<?php

if (class_exists("\Person")) {
    Person::makeNewPerson();
    $object = new \Person();
    if (method_exists($object, "showName")) {
        $object->showName();
    }
    if (method_exists($object, "print_sum")) {
        $object->print_sum(1, 2, 3, 5);
    }
    if (method_exists($object, "getAge")) {
        echo "the original age is " . $object->getAge()."\n";
    }
    if (method_exists($object, "setAge")) {
        $object->setAge(27);
    }
    if (method_exists($object, "getAge")) {
        echo "the new age is " . $object->getAge()."\n";
    }
    if (method_exists($object, "getName")) {
        echo "the name is " . $object->getName()."\n";
    }
    if (method_exists($object, "addTwoNum")) {
        echo "the sum of 1 and 99 is " . $object->addTwoNum(1, 99)."\n";
    }
    if (method_exists($object, "addSum")) {
        echo "the sum of 1, 2, 3, 4, 5, 6, 7 is " . $object->addSum(1, 2, 3, 4, 5, 6, 7)."\n";
        echo "the sum of 1, 2, 3 is " . $object->addSum(1, 2, 3)."\n";
    }
    // static method invoke
    if (method_exists("Person", "staticShowName")) {
        Person::staticShowName();
    }

    if (method_exists("Person", "concatStr")) {
        echo "the concat of abc and def is " . Person::concatStr("abc", "def");
    }

}

?>
--EXPECT--
my name is zapi
the sum is 11
the original age is 0
the new age is 27
the name is zzu_softboy
the sum of 1 and 99 is 100
the sum of 1, 2, 3, 4, 5, 6, 7 is 28
the sum of 1, 2, 3 is 6
static my name is zapi
the concat of abc and def is abcdef
