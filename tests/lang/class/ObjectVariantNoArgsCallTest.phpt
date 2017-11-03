<?php
ob_start();
if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->testNoArgCall();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
ObjectVariantClass::printName been called
ObjectVariantClass::getName been called
the result of ObjectVariantClass::getName is hello, zapi
EOF;

if ($ret != $expect) {
    exit(1);
}
