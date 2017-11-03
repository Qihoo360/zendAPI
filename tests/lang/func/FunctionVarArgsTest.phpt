<?php
ob_start();
if (function_exists("\zapi\io\print_sum")) {
    \zapi\io\print_sum(1, 2);
    echo "\n";
    \zapi\io\print_sum(1, 2, 3, 4, 5, 6, 7, 8, 10);
    echo "\n";
    \zapi\io\print_sum(123, 321);
    echo "\n";
}
if (function_exists("\zapi\io\calculate_sum")) {
    echo \zapi\io\calculate_sum(1, 2);
    echo "\n";
    echo \zapi\io\calculate_sum(1, 2, 3, 4, 5, 6, 7, 8, 10);
    echo "\n";
    echo \zapi\io\calculate_sum(123, 321);
}

$ret = trim(ob_get_clean());
$expect = <<<'EOF'
3
46
444
3
46
444
EOF;

if ($ret != $expect) {
    exit(1);
}

