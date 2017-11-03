<?php
ob_start();
if (class_exists("A") && class_exists("B") && class_exists("C")) {
    echo "class A and class B and class C exist\n";
    $obj = new C();
    $obj->printInfo();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
class A and class B and class C exist
C::printInfo been called
B::printInfo been called
B::showSomething been called
EOF;

if ($ret != $expect) {
    exit(1);
}
