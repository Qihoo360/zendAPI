--TEST--
Function exists test
--FILE--
<?php
if (function_exists("show_something")) {
   echo "show_something exist\n";
}

if (function_exists("get_name")) {
   echo "get_name exist\n";
}

if (function_exists("print_name")) {
   echo "print_name exist\n";
}

if (function_exists("print_name_and_age")) {
   echo "print_name_and_age exist\n";
}

if (function_exists("add_two_number")) {
   echo "add_two_number exist\n";
}

// test function in namespace
if (function_exists("\zapi\get_name")) {
   echo "\zapi\get_name exist\n";
}

if (function_exists("\zapi\io\print_name")) {
   echo "\zapi\io\print_name exist\n";
}

if (function_exists("\zapi\io\show_something")) {
   echo "\zapi\io\show_something exist\n";
}

?>
--EXPECT--
show_something exist
get_name exist
print_name exist
print_name_and_age exist
add_two_number exist
\zapi\get_name exist
\zapi\io\print_name exist
\zapi\io\show_something exist