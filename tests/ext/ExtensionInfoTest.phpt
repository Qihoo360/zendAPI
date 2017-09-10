--TEST--
extension info test
--FILE--
<?php

$refl = new ReflectionExtension("dummyext");
var_dump($refl->getName());
var_dump($refl->getVersion());
ob_start();
phpinfo();
$text = ob_get_clean();
if (-1 !== strpos($text, "dummyext support => enabled")) {
    echo "dummyext support => enabled\n";
}
if (-1 !== strpos($text, "dummyext team => qcoreteam")) {
    echo "dummyext team => qcoreteam \n";
}
?>
--EXPECT--
string(8) "dummyext"
string(3) "1.0"
dummyext support => enabled
dummyext team => qcoreteam
