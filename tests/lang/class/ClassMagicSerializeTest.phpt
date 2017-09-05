--TEST--
Class magic Serialize test
--FILE--
<?php

if (class_exists("\MagicMethodClass")) {
    $object = new \MagicMethodClass();
    $str = serialize($object);
    echo $str."\n";
    unserialize($str);
}

?>
--EXPECT--
MagicMethodClass::serialize is called
C:16:"MagicMethodClass":14:{serialize data}
MagicMethodClass::unserialize is called
serialize data : serialize data}
