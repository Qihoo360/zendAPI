<?php
ob_start();
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
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
class VisibilityClass exists
VisibilityClass::publicProp is public
VisibilityClass::protectedProp is protected
VisibilityClass::privateProp is private
EOF;

if ($ret != $expect) {
    exit(1);
}

