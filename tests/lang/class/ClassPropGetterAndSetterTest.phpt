--TEST--
Class property getter and setter test
--FILE--
<?php
    
if (class_exists("PropsTestClass")) {
    $object = new PropsTestClass();
    if (property_exists($object, "name")) {
        echo "PropsTestClass::name is exist\n";
        echo "PropsTestClass::name value : {$object->name}\n";
    }
    if (property_exists($object, "name")) {
        $object->name = "unicornteam";
        echo "PropsTestClass::name value : {$object->name}\n";
    }
    if (!property_exists($object, "notExistsProp")) {
        echo "PropsTestClass::notExistsProp is not exist\n";
    }
    $object->notExistsProp = 123;
    if (property_exists($object, "notExistsProp")) {
        echo "PropsTestClass::notExistsProp is exist\n";
        echo "PropsTestClass::notExistsProp value : {$object->notExistsProp}\n";
    }
}

?>
--EXPECT--
PropsTestClass::name is exist
PropsTestClass::name value : 
PropsTestClass::name value : zapi:unicornteam
PropsTestClass::name is not exist
