<?php

// here we test argument passed
if (function_exists("say_hello")) {
    ob_start();
    say_hello();
    $ret = ob_get_clean();
    if ("hello, zapi" != trim($ret)) {
       goto error;
    }
    ob_start();
    say_hello("unicornteam");
    $ret = ob_get_clean();
    if ("hello, unicornteam" != trim($ret)) {
       goto error;
    }
    ob_start();
    say_hello("zzu_softboy");
    $ret = ob_get_clean();
    if ("hello, zzu_softboy" != trim($ret)) {
       goto error;
    }
    ob_start();
    say_hello(3.14); // here will convert into string
    $ret = ob_get_clean();
    if ("hello, 3.14" != trim($ret)) {
       goto error;
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
