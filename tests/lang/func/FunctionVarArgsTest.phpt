<?php

if (function_exists("\zapi\io\print_sum")) {
    ob_start();
    \zapi\io\print_sum(1, 2);
    echo " ";
    \zapi\io\print_sum(1, 2, 3, 4, 5, 6, 7, 8, 10);
    echo " ";
    \zapi\io\print_sum(123, 321);
    $ret = ob_get_clean();
    if ("3 46 444" != trim($ret)) {
       goto error;
    }
} else {
    goto error;
}

if (function_exists("\zapi\io\calculate_sum")) {
    ob_start();
    echo \zapi\io\calculate_sum(1, 2);
    echo " ";
    echo \zapi\io\calculate_sum(1, 2, 3, 4, 5, 6, 7, 8, 10);
    echo " ";
    echo \zapi\io\calculate_sum(123, 321);
    $ret = ob_get_clean();
    if ("3 46 444" != trim($ret)) {
       goto error;
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
