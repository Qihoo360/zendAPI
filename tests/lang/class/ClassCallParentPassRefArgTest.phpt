<?php
ob_start();
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->testCallParentPassRefArg();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
class A and class B and class C exist
C::testCallParentPassRefArg been called
before call changeNameByRef : xxxx
A::changeNameByRef been called
get ref arg
after call changeNameByRef : hello, zapi
before call calculateSumByRef : 0
C::calculateSumByRef been called
got 4 args
retval is reference arg
after call calculateSumByRef : 47
EOF;

if ($ret != $expect) {
    exit(1);
}
