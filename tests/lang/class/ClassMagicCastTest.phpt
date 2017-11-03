<?php
ob_start();
if (class_exists("\MagicMethodClass")) {
    $object = new \MagicMethodClass();
    echo "cast to string : " . $object;
    echo "\n";
    echo "cast to integer : " . intval($object);
    echo "\n";
    echo "cast to integer : " . (int)$object;
    echo "\n";
    echo "cast to double : " . (double)$object;
    echo "\n";
    echo "cast to double : " . doubleval($object);
    echo "\n";
    echo "cast to boolean : " . boolval($object);
    echo "\n";
    echo "cast to boolean : " . (bool)$object;
    echo "\n";
}
$ret = trim(ob_get_clean());
$expect = <<<EOF
MagicMethodClass::__toString is called
cast to string : hello, zapi
MagicMethodClass::__toInteger is called
cast to integer : 2017
MagicMethodClass::__toInteger is called
cast to integer : 2017
MagicMethodClass::__toDouble is called
cast to double : 3.14
MagicMethodClass::__toDouble is called
cast to double : 3.14
MagicMethodClass::__toBool is called
cast to boolean : 1
MagicMethodClass::__toBool is called
cast to boolean : 1
EOF;

if ($ret != $expect) {
    exit(1);
}
