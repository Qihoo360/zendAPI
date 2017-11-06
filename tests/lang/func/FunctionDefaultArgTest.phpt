<?php
ob_start();
// here we test argument passed
if (function_exists("say_hello")) {
    say_hello();
    say_hello("unicornteam");
    say_hello("zzu_softboy");
    say_hello(3.14); // here will convert into string
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
hello, zapi
hello, unicornteam
hello, zzu_softboy
hello, 3.14
EOF;

if ($ret != $expect) {
    echo "got: ".$ret;
    exit(1);
}
