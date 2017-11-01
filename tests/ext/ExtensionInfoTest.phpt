<?php
$refl = new ReflectionExtension("dummyext");

if ($refl->getName() != "dummyext") {
    goto error;
}
if ($refl->getVersion() != "1.0") {
    goto error;
}

ob_start();
phpinfo();
$text = ob_get_clean();
if (-1 == strpos($text, "dummyext support => enabled")) {
    goto error;
}
if (-1 == strpos($text, "dummyext team => qcoreteam")) {
    goto error;
}

success:
exit(0);
error:
exit(1);
