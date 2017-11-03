<?php
ob_start();
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
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
hello world, zapi
hello world, zapi
hello world, zapi
EOF;

if ($ret != $expect) {
    exit(1);
}
