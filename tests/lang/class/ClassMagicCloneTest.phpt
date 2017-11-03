<?php
ob_start();
if (class_exists("\MagicMethodClass")) {
    $object = new \MagicMethodClass();
    $object1 = clone $object;
    //var_dump($object);
    //var_dump($object1);
}

$ret = trim(ob_get_clean());
$expect = <<<EOF
MagicMethodClass::__clone is called
EOF;

if ($ret != $expect) {
    exit(1);
}


