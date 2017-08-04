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
var_dump(defined("Person::MY_CONST"));
var_dump(Person::MY_CONST);
var_dump(defined("Person::PI"));
var_dump(Person::PI);
var_dump($person->name);
?>
--EXPECT--
zapi v0.0.1 hello world, zapi


