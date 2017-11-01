<?php

if (function_exists("get_ext_handler_msgs")) {
    $msgs = get_ext_handler_msgs();
    if ($msgs != array(
        "module startup handler called",
        "request startup handler called"
    )) {
        goto error;
    }
}

success:
exit(0);
error:
exit(1);
