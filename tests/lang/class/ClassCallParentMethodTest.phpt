<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    $obj = new C();
    ob_start();
    $obj->printInfo();
    $ret = trim(ob_get_clean());
} else {
    goto error;
}
$expect = <<<EOF
C::printInfo been called
B::printInfo been called
B::showSomething been called
EOF;

if ($ret != $expect){
    goto error;
}

success:
exit(0);
error:
exit(1);

