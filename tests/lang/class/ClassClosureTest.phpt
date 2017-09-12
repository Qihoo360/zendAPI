--TEST--
Class call parent method pass ref argument test
--FILE--
<?php

if (class_exists("\ZapiClosure")) {
    echo "internal class ZapiClosure exists\n";
    $refl = new ReflectionClass("ZapiClosure");
    if ($refl->isFinal()) {
        echo "class ZapiClosure is final\n";
    }
}

?>
--EXPECT--
internal class ZapiClosure exists
class ZapiClosure is final
