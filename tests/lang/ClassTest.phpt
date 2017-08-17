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
//$arr = [];
//$arr[1] = new P;
//$arr[1][1][1][3] = 2;
//$arr[1][2] = "beijing";
//var_dump($arr);
$arr1 = array(
   1 => array(
      "name" => "xiux",
      "age" => 123
   )
);
$arr2 = $arr1;
var_dump($arr2);
var_dump($arr1);
$arr1[1]["age"] = 321;
//xdebug_debug_zval($arr1);
//xdebug_debug_zval($arr2);
var_dump($arr2);
var_dump($arr1);
?>
--EXPECT--
zapi v0.0.1 hello world, zapi


