<?php

if (class_exists("\IterateTestClass")) {;
    $data = new IterateTestClass();
    if (!isset($data["key1"]) || "value1" != $data["key1"]) {
        goto error;
    }
    if (!isset($data["key2"]) || "value2" != $data["key2"]) {
        goto error;
    }
    if (!isset($data["key3"]) || "value3" != $data["key3"]) {
        goto error;
    }
    // test unset
    if (4 != count($data)) {
        goto error;
    }
    unset($data["key1"]);
    if (isset($data["key1"])) {
        goto error;
    }
    if (3 != count($data)) {
        goto error;
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);

