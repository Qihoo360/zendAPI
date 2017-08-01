--TEST--
function register test
--FILE--
<?php
if (function_exists("show_something")) {
   echo "show_something exist";
}
// call show_something
echo "\n";
show_something();
echo "\n";
if (function_exists("get_name")) {
   echo "get_name exist";
}
echo "\n";
echo get_name();
echo "\n";
if (function_exists("print_name")) {
   echo "print_name exist";
}
echo "\n";
print_name("zapi");
if (function_exists("print_name_and_age")) {
   echo "print_name_and_age exist";
}
echo "\n";
print_name_and_age("zzu_softboy", 27);
echo "\n";
if (function_exists("add_two_number")) {
   echo "add_two_number exist";
}
echo "\n";
echo "1 + 2 = " . add_two_number(1, 2);
?>
--EXPECT--
show_something exist
hello world, zapi
get_name exist
zzu_softboy
print_name exist
zapiprint_name_and_age exist
name: zzu_softboy age: 27
add_two_number exist
1 + 2 = 3