--TEST--
Class get variant ptr test
--FILE--
<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->testGetObjectVaraintPtr();
}

?>
--EXPECT--
class A and class B and class C exist
C::testGetObjectVaraintPtr been called
property C::address exists
property value : beijing
property C::privateName not exists
property C::protectedName exists
property value : protected zapi
method C::showSomething exists
B::showSomething been called
method C::privateCMethod exists
C::privateCMethod been called
method C::privateCMethod exists
method C::protectedAMethod exists
A::protectedAMethod been called
method C::privateBMethod exists
method C::protectedBMethod exists
B::protectedBMethod been called
A::protectedAMethod been called
