--TEST--
Class register test
--FILE--
<?php
if (class_exists("Person")) {
   echo "class Person exist";
}
?>
--EXPECT--
class Person exist