--TEST--
Class register test
--FILE--
<?php
if (class_exists("Person")) {
   echo "class Person exist";
}
var_dump(new Person);
$person = new Person;
$person = new Person;
$person1 = new Person;
var_dump($person);

var_dump(defined("Person::MY_CONST"));
var_dump(Person::MY_CONST);
var_dump(defined("Person::PI"));
var_dump(Person::PI);
var_dump($person->name);
var_dump(interface_exists("InfoProvider"));
var_dump(method_exists("InfoProvider", "getName"));
var_dump($person instanceof InfoProvider);
var_dump(function_exists("zapi\\get_name"));
var_dump(function_exists("zapi\\io\\print_name"));
var_dump(function_exists("zapi\\io\\show_something"));
?>
--EXPECT--
zapi v0.0.1 hello world, zapi


