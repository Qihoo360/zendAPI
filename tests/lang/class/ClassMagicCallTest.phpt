<?php
ob_start();
if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    $nonMagicMethodObject = new NonMagicMethodClass();
    // $nonMagicMethodObject->calculateSum(1, 2, 3); // fata error
    $magicMethodClass = new MagicMethodClass();
    $sum = $magicMethodClass->calculateSum(1, 2, 4);
    echo "the sum is " . $sum ."\n";
    $ret = $magicMethodClass->notProcessCase("zapi");
    if (is_null($ret)) {
        echo "magicMethodClass notProcessCase('zapi') return null\n";
    }
}
$ret = trim(ob_get_clean());
$expect = <<<EOF
MagicMethodClass::__call is called
the sum is 7
MagicMethodClass::__call is called
magicMethodClass notProcessCase('zapi') return null
EOF;
if ($ret != $expect) {
    exit(1);
}
