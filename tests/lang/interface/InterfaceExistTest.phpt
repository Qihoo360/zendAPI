<?php

if (!interface_exists("InterfaceA") || !interface_exists("InterfaceB") || !interface_exists("InterfaceC")) {
    goto error;   
}

success:
exit(0);
error:
exit(1);
