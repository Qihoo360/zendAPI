<?php
ob_start();
if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    // $NonMagicMethodClass::staticCalculateSum(12, 24, 48); // fata error
    $sum = MagicMethodClass::staticCalculateSum(12, 24, 48);
    echo "the sum is " . $sum ."\n";
    $ret = MagicMethodClass::notProcessCase("zapi");
    if (!is_null($ret)) {
       echo "MagicMethodClass::notProcessCase('zapi') return $ret\n";
    }
}
$ret = trim(ob_get_clean());
$expect = <<<EOF
MagicMethodClass::__callStatic is called
the sum is 84
MagicMethodClass::__callStatic is called
MagicMethodClass::notProcessCase('zapi') return hello, zapi
EOF;
if ($ret != $expect) {
    exit(1);
}
