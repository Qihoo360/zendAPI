<?php
ob_start();
if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->testDerivedFrom();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
A is not derived from A
B is not derived from B
C is not derived from C
B is derived from A
C is derived from B
C is derived from A
A is not derived from B
C is not derived from B
C is not derived from A
EOF;

if ($ret != $expect) {
    exit(1);
}
