--TEST--
Class magic static call test
--FILE--
<?php

if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    // $NonMagicMethodClass::staticCalculateSum(12, 24, 48); // fata error
    $sum = MagicMethodClass::staticCalculateSum(12, 24, 48);
    echo "the sum is " . $sum ."\n";
    $ret = MagicMethodClass::notProcessCase("zapi");
    if (!is_null($ret)) {
       echo "MagicMethodClass::notProcessCase('zapi') return $ret\n";
    }
}

?>
--EXPECT--
MagicMethodClass::__callStatic is called
the sum is 84
MagicMethodClass::__callStatic is called
MagicMethodClass::notProcessCase('zapi') return hello, zapi
