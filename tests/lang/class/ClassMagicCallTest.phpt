--TEST--
Class magic call test
--FILE--
<?php

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

?>
--EXPECT--
the sum is 7
magicMethodClass notProcessCase('zapi') return null
