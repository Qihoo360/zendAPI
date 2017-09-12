--TEST--
Class magic debug info test
--FILE--
<?php

if (class_exists("\MagicMethodClass")) {
    echo "class MagicMethodClass exists\n";
    $object = new \MagicMethodClass();
    // var_dump($object);
    // have no __debuginfo()
    $anotherObj = new \FinalTestClass();
    // var_dump($anotherObj);
    // class MagicMethodClass exists
    // object(MagicMethodClass)#1 (2) {
    //  ["name"]=>
    //  string(4) "zapi"
    //  ["address"]=>
    //  string(7) "beijing"
    //}
    //object(FinalTestClass)#2 (0) {
    //}    
}

?>
--EXPECT--
class MagicMethodClass exists
