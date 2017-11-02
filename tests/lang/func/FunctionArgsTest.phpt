<?php

// here we test argument passed
if (function_exists("print_name")) {
   $ret = "";
   ob_start();
   print_name("unicorn team");
   $ret = ob_get_clean();
   if ("unicorn team" != $ret) {
      goto error;
   }
   ob_start();
   print_name(3.14);
   $ret = ob_get_clean();
   if ("3.14" != $ret) {
      goto error;
   }
   ob_start();
   print_name(true);
   $ret = ob_get_clean();
   if ("1" != $ret) {
      goto error;
   }
   
} else {
    goto error;
}

if (function_exists("\zapi\io\print_name")) {
   ob_start();
   print_name("hello, zapi");
   $ret = ob_get_clean();
   
   if ("hello, zapi" != $ret) {
      goto error;
   }
   ob_start();
   print_name(4.16);
   $ret = ob_get_clean();
   
   if ("4.16" != $ret) {
      goto error;
   }
   ob_start();
   print_name(false);
   $ret = ob_get_clean();
   if ("" != $ret) {
      goto error;
   }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
