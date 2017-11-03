<?php
ob_start();
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
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
PropsTestClass::name is exist
PropsTestClass::name value : 
PropsTestClass::name value : zapi:unicornteam
PropsTestClass::notExistsProp is not exist
PropsTestClass::notExistsProp is exist
PropsTestClass::notExistsProp value : 123
EOF;

if ($ret != $expect) {
    exit(1);
}

