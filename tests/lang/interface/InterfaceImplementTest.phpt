--TEST--
interface implement test
--FILE--
<?php

if (interface_exists("InterfaceA") && interface_exists("InterfaceB") && interface_exists("InterfaceC")) {
   if (is_subclass_of("InterfaceC", "InterfaceB") {
      echo "xxxxx";
   }
}

?>
--EXPECT--

