<?php
// here we test simple return string value
if (function_exists("get_name")) {
   $name = get_name();
   if (!is_string($name)) {
      goto error;
   }
} else {
    goto error;
}

if (function_exists("\zapi\get_name")) {
   $name = \zapi\get_name();
   if (!is_string($name)) {
      goto error;
   }
} else {
    goto error;
}

if (function_exists("add_two_number")) {
   $sum = add_two_number(1, 2);
   if (!is_int($sum)) {
      goto error;
   }
} else {
    goto error;
}

if (function_exists("return_arg")) {
   $data = return_arg(1);
   if (!is_int($data) || $data !== 1) {
      goto error;
   }
   $data = return_arg(true);
   if (!is_bool($data) || $data !== true) {
      goto error;
   }
   $data = return_arg(3.14);
   if (!is_float($data) || $data !== 3.14) {
      goto error;
   }
   $data = return_arg(array(1));
   if (!is_array($data) || $data !== array(1)) {
      goto error;
   }
} else {
    goto error;
}
success:
exit(0);
error:
exit(1);
