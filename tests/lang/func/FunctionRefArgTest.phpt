<?php

if (function_exists("get_value_ref"))
{
    $num = 123;
    get_value_ref($num);
    if (321 != $num) {
        goto error;
    }
} else {
    goto error;
}
if (function_exists("passby_value"))
{
    $num = 123;
    passby_value($num);
    if (123 != $num) {
        goto error;
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
