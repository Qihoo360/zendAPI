--TEST--
Class final test
--FILE--
<?php

if (class_exists("\FinalTestClass")) {
    echo "FinalTestClass exists\n";
    $refl = new ReflectionClass("FinalTestClass");
    if ($refl->isFinal()) {
        echo "class FinalTestClass is final\n";
    }
}
if (class_exists("\VisibilityClass")) {
    echo "VisibilityClass exists\n";
    $refl = new ReflectionClass("VisibilityClass");
    $mrefl = $refl->getMethod("finalMethod");
    if ($mrefl->isFinal()) {
        echo "method VisibilityClass::finalMethod is final\n";
    }
}
// class xx extends FinalTestClass
// {}
// PHP Fatal error:  Class xx may not inherit from final class (FinalTestClass)
?>
--EXPECT--
FinalTestClass exists
FinalTestClass exists
class FinalTestClass is final
VisibilityClass exists
method VisibilityClass::finalMethod is final
