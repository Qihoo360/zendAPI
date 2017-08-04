--TEST--
Class register test
--FILE--
<?php
if (class_exists("Person")) {
   echo "class Person exist";
}

$person = new Person;
var_dump($person);
//$person = new Person;

?>
--EXPECT--
zapi v0.0.1 hello world, zapi


