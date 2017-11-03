<?php
ob_start();
if (class_exists("\MagicMethodClass")) {
    $lhs = new MagicMethodClass();
    $rhs = new MagicMethodClass();
    if ($lhs == $rhs) {
        echo "\$lhs == \$rhs\n";
    }
    if ($lhs <= $rhs) {
        echo "\$lhs <= \$rhs\n";
    }
    if ($lhs >= $rhs) {
        echo "\$lhs >= \$rhs\n";
    }
    if ($lhs > $rhs) {
        echo "\$lhs > \$rhs\n";
    }
    if ($lhs < $rhs) {
        echo "\$lhs < \$rhs\n";
    }
    if ($lhs != $rhs) {
        echo "\$lhs != \$rhs\n";
    }
    // assign value
    $lhs->length = 1;
    $rhs->length = 2;
    echo "-------\n";
    if ($lhs == $rhs) {
        echo "\$lhs == \$rhs\n";
    }
    if ($lhs <= $rhs) {
        echo "\$lhs <= \$rhs\n";
    }
    if ($lhs >= $rhs) {
        echo "\$lhs >= \$rhs\n";
    }
    if ($lhs > $rhs) {
        echo "\$lhs > \$rhs\n";
    }
    if ($lhs < $rhs) {
        echo "\$lhs < \$rhs\n";
    }
    if ($lhs != $rhs) {
        echo "\$lhs != \$rhs\n";
    }
    
    $lhs->length = 2;
    $rhs->length = 1;
    echo "-------\n";
    if ($lhs == $rhs) {
        echo "\$lhs == \$rhs\n";
    }
    if ($lhs <= $rhs) {
        echo "\$lhs <= \$rhs\n";
    }
    if ($lhs >= $rhs) {
        echo "\$lhs >= \$rhs\n";
    }
    if ($lhs > $rhs) {
        echo "\$lhs > \$rhs\n";
    }
    if ($lhs < $rhs) {
        echo "\$lhs < \$rhs\n";
    }
    if ($lhs != $rhs) {
        echo "\$lhs != \$rhs\n";
    }
    
}

$ret = trim(ob_get_clean());
$expect = <<<'EOF'
MagicMethodClass::__compare is called
$lhs == $rhs
MagicMethodClass::__compare is called
$lhs <= $rhs
MagicMethodClass::__compare is called
$lhs >= $rhs
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
MagicMethodClass::__set is called
MagicMethodClass::__set is called
-------
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
$lhs <= $rhs
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
$lhs < $rhs
MagicMethodClass::__compare is called
$lhs != $rhs
MagicMethodClass::__set is called
MagicMethodClass::__set is called
-------
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
$lhs >= $rhs
MagicMethodClass::__compare is called
$lhs > $rhs
MagicMethodClass::__compare is called
MagicMethodClass::__compare is called
$lhs != $rhs
EOF;

if ($ret != $expect) {
    exit(1);
}

