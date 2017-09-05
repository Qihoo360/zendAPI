--TEST--
Class const type test
--FILE--
<?php
if (class_exists("Person")) {
   if (defined("Person::UNICORNTEAM") && is_string(Person::UNICORNTEAM)) {
      echo "Person::UNICORNTEAM is string\n";
   }
   if (defined("Person::MY_CONST") && is_string(Person::MY_CONST)) {
      echo "Person::MY_CONST is string\n";
   }
   if (defined("Person::PI") && is_double(Person::PI)) {
      echo "Person::PI is double\n";
   }
   if (defined("Person::HEADER_SIZE") && is_int(Person::HEADER_SIZE)) {
      echo "Person::HEADER_SIZE is int\n";
   }
   if (defined("Person::ALLOW_ACL") && is_bool(Person::ALLOW_ACL)) {
      echo "Person::ALLOW_ACL is bool\n";
   }
}
?>
--EXPECT--
Person::UNICORNTEAM is string
Person::MY_CONST is string
Person::PI is double
Person::HEADER_SIZE is int
Person::ALLOW_ACL is bool
