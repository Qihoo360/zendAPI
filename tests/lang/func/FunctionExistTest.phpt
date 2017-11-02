<?php
if (!function_exists("show_something")) {
   goto error;
}

if (!function_exists("get_name")) {
   goto error;
}

if (!function_exists("print_name")) {
   goto error;
}

if (!function_exists("print_name_and_age")) {
   goto error;
}

if (!function_exists("add_two_number")) {
   goto error;
}

// test function in namespace
if (!function_exists("\zapi\get_name")) {
   goto error;
}

if (!function_exists("\zapi\io\print_name")) {
   goto error;
}

if (!function_exists("\zapi\io\show_something")) {
   goto error;
}

success:
exit(0);
error:
exit(1);
