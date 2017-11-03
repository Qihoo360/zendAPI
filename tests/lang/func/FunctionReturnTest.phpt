<?php
ob_start();
// here we test simple return string value
if (function_exists("get_name")) {
   $name = get_name();
   if (is_string($name)) {
      echo $name;
   }
}
echo "\n";
if (function_exists("\zapi\get_name")) {
   $name = \zapi\get_name();
   if (is_string($name)) {
      echo $name;
   }
}
echo "\n";
if (function_exists("add_two_number")) {
   $sum = add_two_number(1, 2);
   if (is_int($sum)) {
      echo "the sum is " . $sum;
   }
}
echo "\n";
if (function_exists("return_arg")) {
   $data = return_arg(1);
   if (is_int($data) && $data == 1) {
      echo "success\n";
   }
   $data = return_arg(true);
   if (is_bool($data) && $data === true) {
      echo "success\n";
   }
   $data = return_arg(3.14);
   if (is_float($data) && $data === 3.14) {
      echo "success\n";
   }
   $data = return_arg(array(1));
   if (is_array($data) && $data === array(1)) {
      echo "success\n";
   }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
zzu_softboy
zzu_softboy
the sum is 3
success
success
success
success
EOF;

if ($ret != $expect) {
    exit(1);
}
