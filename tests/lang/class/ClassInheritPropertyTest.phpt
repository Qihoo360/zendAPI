<?php
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    $obj = new C();
    if (!property_exists($obj, "protectedName")) {
        goto error;
    }
    if (property_exists($obj, "privateName")) {
        goto error;
    }
    if (!property_exists($obj, "propsFromB")) {
        goto error;
    }
    
    // $obj->protectedName; fata error
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
