--TEST--
Class exist test
--FILE--
<?php
if (class_exists("Person")) {
   echo "class \Person exist\n";
}
if (class_exists("\zapi\EmptyClass")) {
   echo "class \zapi\EmptyClass exist\n";
}
if (class_exists("ConstructAndDestruct")) {
   echo "class \ConstructAndDestruct exist\n";
}
?>
--EXPECT--
class \Person exist
class \zapi\EmptyClass exist
class \ConstructAndDestruct exist