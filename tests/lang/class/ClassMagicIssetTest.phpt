<?php
ob_start();
if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    $nonMagicMethodObject = new NonMagicMethodClass();
    // $nonMagicMethodObject->prop1; Notice: Undefined property: NonMagicMethodClass::$prop1
    if (!property_exists($nonMagicMethodObject, "prop1")) {
        echo "\$nonMagicMethodObject->prop1 is not exist\n";
    }
    $magicMethodObject = new MagicMethodClass();
    if (property_exists($magicMethodObject, "prop1")) {
        echo "\$magicMethodObject->prop1 is exist\n";
    }
    if (!property_exists($magicMethodObject, "notExistProp")) {
        echo "\$magicMethodObject->notExistProp is not exist\n";
    }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
$nonMagicMethodObject->prop1 is not exist
MagicMethodClass::__isset is called
$magicMethodObject->prop1 is exist
MagicMethodClass::__isset is called
$magicMethodObject->notExistProp is not exist
EOF;

if ($ret != $expect) {
    exit(1);
}
