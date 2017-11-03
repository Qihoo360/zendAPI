<?php
ob_start();
if (class_exists("\MagicMethodClass")) {
    $object = new \MagicMethodClass();
    $str = serialize($object);
    echo $str."\n";
    unserialize($str);
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
MagicMethodClass::serialize is called
C:16:"MagicMethodClass":14:{serialize data}
MagicMethodClass::unserialize is called
serialize data : serialize data}
EOF;

if ($ret != $expect) {
    exit(1);
}
