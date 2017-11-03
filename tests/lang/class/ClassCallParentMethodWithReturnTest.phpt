<?php
$ret = "";
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    $obj = new C();
    ob_start();
    $obj->testCallParentWithReturn();
    $ret = trim(ob_get_clean());
} else {
    goto error;
}

$expect = <<<EOF
C::testCallParentWithReturn been called
B::addTwoNumber been called
after call addTwoNumber get : 24
EOF;

if ($ret != $expect) {
    goto error;
}

success:
exit(0);
error:
exit(1);
