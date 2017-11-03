<?php

$ret = "";
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    $obj = new C();
    ob_start();
    $obj->testCallParentPassRefArg();
    $ret = trim(ob_get_clean());
} else {
    goto error;
}

$expect = <<<EOF
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
    goto error;
}

success:
exit(0);
error:
exit(1);
