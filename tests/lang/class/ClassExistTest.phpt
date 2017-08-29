--TEST--
Class exist test
--FILE--
<?php
if (class_exists("Person")) {
   //echo "class Person exist\n";
}
//$person = new Person;
//$person = new Person;
//$person = new Person;
?>
--EXPECT--