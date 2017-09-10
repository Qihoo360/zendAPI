--TEST--
Class method visibility test
--FILE--
<?php

if (class_exists("\VisibilityClass")) {
    echo "class VisibilityClass exists\n";
    $refl = new ReflectionClass("VisibilityClass");
    $publicMethod = $refl->getMethod("publicMethod");
    $protectedMethod = $refl->getMethod("protectedMethod");
    $privateMethod = $refl->getMethod("privateMethod");
    if ($publicMethod->isPublic()) {
        echo  "VisibilityClass::publicMethod is public\n";
    }
    if ($protectedMethod->isProtected()) {
        echo  "VisibilityClass::protectedMethod is protected\n";
    }
    if ($privateMethod->isPrivate()) {
        echo  "VisibilityClass::privateMethod is private\n";
    }
}

?>
--EXPECT--
class VisibilityClass exists
VisibilityClass::publicMethod is public
VisibilityClass::protectedMethod is protected
VisibilityClass::privateMethod is private

