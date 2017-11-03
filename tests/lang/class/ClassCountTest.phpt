<?php
ob_start();
if (class_exists("\IterateTestClass")) {
    $data = new IterateTestClass();
    echo "the count of \$data is ".count($data)."\n";
}
$ret = trim(ob_get_clean());

$expect = <<<'EOF'
IterateTestClass::count called
the count of $data is 4
EOF;

if ($ret != $expect) {
    exit(1);
}
