<?php
ob_start();
if (class_exists("Person")) {
   echo "class \Person exist\n";
}
if (class_exists("\zapi\EmptyClass")) {
   echo "class \zapi\EmptyClass exist\n";
}
if (class_exists("ConstructAndDestruct")) {
   echo "class \ConstructAndDestruct exist\n";
}
$ret = trim(ob_get_clean());

$expect = <<<'EOF'
class \Person exist
class \zapi\EmptyClass exist
class \ConstructAndDestruct exist
EOF;

if ($ret != $expect) {
    exit(1);
}


