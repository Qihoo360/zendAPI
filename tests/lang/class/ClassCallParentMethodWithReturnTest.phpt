<?php
ob_start();
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->testCallParentWithReturn();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
class A and class B and class C exist
C::testCallParentWithReturn been called
B::addTwoNumber been called
after call addTwoNumber get : 24
EOF;

if ($ret != $expect) {
    exit(1);
}
