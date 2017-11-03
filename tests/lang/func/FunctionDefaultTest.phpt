<?php

// here we just invoke a function without params and return value
if (function_exists("show_something")) {
   ob_start();
   show_something();
   $ret = ob_get_clean();
   if ("hello world, zapi" != trim($ret)) {
      goto error;
   }
} else {
    goto error;
}
if (function_exists("\zapi\show_something")) {
   ob_start();
   \zapi\show_something();
   $ret = ob_get_clean();
   if ("hello world, zapi" != trim($ret)) {
      goto error;
   }
}
if (function_exists("\zapi\io\show_something")) {
   ob_start();
   \zapi\io\show_something();
   $ret = ob_get_clean();
   if ("hello world, zapi" != trim($ret)) {
      goto error;
   }
}


success:
exit(0);
error:
exit(1);
