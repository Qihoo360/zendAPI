--TEST--
Class static method exist test
--FILE--
<?php
if (class_exists("Person")) {
   if (method_exists("Person", "staticShowName")) {
       echo "static method Person::staticShowName exists\n";
   }
   if (method_exists("Person", "staticProtectedMethod")) {
       echo "static method Person::staticProtectedMethod exists\n";
   }
   if (method_exists("Person", "staticPrivateMethod")) {
       echo "static method Person::staticPrivateMethod exists\n";
   }
}
?>
--EXPECT--
static method Person::staticShowName exists
static method Person::staticProtectedMethod exists
static method Person::staticPrivateMethod exists
