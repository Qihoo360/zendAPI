<?php
if (!class_exists("Person")) {
   goto error;
}
if (!class_exists("\zapi\EmptyClass")) {
   goto error;
}
if (!class_exists("ConstructAndDestruct")) {
   goto error;
}

success:
exit(0);
error:
exit(1);
