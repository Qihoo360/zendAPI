--TEST--
Class closure call test
--FILE--
<?php

if (class_exists("\ZapiClosure")) {
    echo "internal class ZapiClosure exists\n";
    $refl = new ReflectionClass("ZapiClosure");
    if ($refl->isFinal()) {
        echo "class ZapiClosure is final\n";
    }
}

if (class_exists("\ClosureTestClass")) {
    echo "internal class ClosureTestClass exists\n";
    $obj = new \ClosureTestClass();
    $obj->testClosureCallable();
    $callable = $obj->getCallable();
    if (is_callable($callable)) {
        echo "\$callable is callable\n";
        $callable();
    }
}

?>
--EXPECT--
internal class ZapiClosure exists
class ZapiClosure is final
internal class ClosureTestClass exists
$callable is callable
print_something called
