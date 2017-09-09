--TEST--
Class property visibility test
--FILE--
<?php

if (class_exists("\VisibilityClass")) {
    echo "class VisibilityClass exists\n";
    $refl = new ReflectionClass("VisibilityClass");
    $publicProp = $refl->getProperty("publicProp");
    $protectedProp = $refl->getProperty("protectedProp");
    $privateProp = $refl->getProperty("privateProp");
    if ($publicProp->isPublic()) {
        echo  "VisibilityClass::publicProp is public\n";
    }
    if ($protectedProp->isProtected()) {
        echo  "VisibilityClass::protectedProp is protected\n";
    }
    if ($privateProp->isPrivate()) {
        echo  "VisibilityClass::privateProp is private\n";
    }
}

?>
--EXPECT--
class VisibilityClass exists
VisibilityClass::publicProp is public
VisibilityClass::protectedProp is protected
VisibilityClass::privateProp is private
