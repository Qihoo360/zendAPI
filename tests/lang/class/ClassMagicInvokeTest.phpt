<?php
ob_start();
if (class_exists("\NonMagicMethodClass") && class_exists("\MagicMethodClass")) {
    //$nonMagicMethodObject = new NonMagicMethodClass();
    // $nonMagicMethodObject(1, 2, 3); // fata error
    $magicMethodClass = new MagicMethodClass();
    $sum = $magicMethodClass();
    echo "the sum of \$magicMethodClass() is " . $sum ."\n";
    $sum = $magicMethodClass(1, 2, 3, 4);
    echo "the sum of \$magicMethodClass() is " . $sum ."\n";
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
MagicMethodClass::__invoke is called
the sum of $magicMethodClass() is 0
MagicMethodClass::__invoke is called
the sum of $magicMethodClass() is 10
EOF;

if ($ret != $expect) {
    exit(1);
}

