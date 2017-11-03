<?php
ob_start();
if (class_exists("\ConstructAndDestruct")) {
    {
        $object = new \ConstructAndDestruct();
        $object = null;
    }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
constructor been invoked
destructor been invoked
EOF;

if ($ret != $expect) {
    exit(1);
}


