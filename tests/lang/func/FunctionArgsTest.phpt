<?php
ob_start();
// here we test argument passed
if (function_exists("print_name")) {
   print_name("unicorn team");
   echo "\n";
   print_name(3.14);
   echo "\n";
   print_name(true);
}
echo "\n";
if (function_exists("\zapi\io\print_name")) {
   print_name("hello, zapi");
   echo "\n";
   print_name(4.16);
   echo "\n";
   print_name(false);
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
unicorn team
3.14
1
hello, zapi
4.16
EOF;

if ($ret != $expect) {
    exit(1);
}
