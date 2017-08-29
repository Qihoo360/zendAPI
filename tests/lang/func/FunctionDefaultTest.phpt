--TEST--
test simple function invoke
--FILE--
<?php

// here we just invoke a function without params and return value
if (function_exists("show_something")) {
   show_something();
}
echo "\n";
if (function_exists("\zapi\show_something")) {
   \zapi\show_something();
}
echo "\n";
if (function_exists("\zapi\io\show_something")) {
   \zapi\io\show_something();
}

?>
--EXPECT--
hello world, zapi
hello world, zapi
hello world, zapi