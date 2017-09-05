--TEST--
Class magic invoke test
--FILE--
<?php

if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    $nonMagicMethodObject = new NonMagicMethodClass();
    // $nonMagicMethodObject(1, 2, 3); // fata error
    $magicMethodClass = new MagicMethodClass();
    $sum = $magicMethodClass();
    echo "the sum of \$magicMethodClass() is " . $sum ."\n";
    $sum = $magicMethodClass(1, 2, 3, 4);
    echo "the sum of \$magicMethodClass() is " . $sum ."\n";
}

?>
--EXPECT--
the sum of $magicMethodClass() is 0
the sum of $magicMethodClass() is 10
