<?php
ob_start();
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    echo "the public property name is : " . $obj->name ."\n";
    if (property_exists($obj, "protectedName")) {
        echo "property protectedName exists\n";
    }
    if (!property_exists($obj, "privateName")) {
        echo "property privateName not exists\n";
    }
    if (property_exists($obj, "propsFromB")) {
        echo "property propsFromB exists\n";
    }
    
    // $obj->protectedName; fata error
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
class A and class B and class C exist
the public property name is : zapi
property protectedName exists
property privateName not exists
property propsFromB exists
EOF;
if ($ret != $expect) {
    exit(1);
}
