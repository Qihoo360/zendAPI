--TEST--
Class register test
--FILE--
<?php
var_dump(class_exists("XXXXXXXX"));
if (class_exists("Person")) {
   echo "class Person exist";
}
var_dump(new Person);
$person = new Person;
$person = new Person;
$person1 = new Person;
var_dump($person);
var_dump($person::$staticProp);
var_dump(Person::QIHOO);
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
var_dump(defined("zapi\\SYS_VERSION"));
var_dump(class_exists("zapi\\Address"));
\zapi\io\print_name("wangyang");
class  P{};
$arr = [];
$arr[1] = new P;
$arr[1][1][1][3] = 2;
//$arr[1][2] = "beijing";
var_dump($arr);
?>
--EXPECT--
zapi v0.0.1 hello world, zapi


