<?php
ob_start();
if (function_exists("get_value_ref"))
{
    $num = 123;
    get_value_ref($num);
    echo $num;
}
echo "\n";
if (function_exists("passby_value"))
{
    $num = 123;
    passby_value($num);
    echo $num;
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
321
123
EOF;

if ($ret != $expect) {
    exit(1);
}
