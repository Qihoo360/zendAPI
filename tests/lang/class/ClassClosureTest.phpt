<?php
ob_start();
if (class_exists("\ZapiClosure")) {
    echo "internal class ZapiClosure exists\n";
    $refl = new ReflectionClass("ZapiClosure");
    if ($refl->isFinal()) {
        echo "class ZapiClosure is final\n";
    }
}
if (class_exists("\ClosureTestClass")) {
    echo "internal class ClosureTestClass exists\n";
    $obj = new \ClosureTestClass();
    $obj->testClosureCallable();
    $callable = $obj->getNoArgAndReturnCallable();
    if (is_callable($callable)) {
        echo "\$callable is callable\n";
        $ret = $callable();
        echo "the return of callable is " . $ret."\n";
    }
    $hasParamCallable = $obj->getArgAndReturnCallable();
    if (is_callable($hasParamCallable)) {
        echo "\$hasParamCallable is callable\n";
        $ret = $hasParamCallable();
        echo "the return of hasParamCallable is " . $ret."\n";
        $ret = $hasParamCallable(123);
        echo "the return of hasParamCallable is " . $ret."\n";
        $ret = $hasParamCallable(3.14);
        echo "the return of hasParamCallable is " . $ret."\n";
        $ret = $hasParamCallable(true);
        echo "the return of hasParamCallable is " . $ret."\n";
    }
}

$ret = trim(ob_get_clean());
$expect = <<<'EOF'
internal class ZapiClosure exists
class ZapiClosure is final
internal class ClosureTestClass exists
$callable is callable
print_something called
the return of callable is print_some
$hasParamCallable is callable
have_ret_and_have_arg called
the return of hasParamCallable is have_ret_and_have_arg
have_ret_and_have_arg called
the return of hasParamCallable is 123
have_ret_and_have_arg called
the return of hasParamCallable is 3.14
have_ret_and_have_arg called
the return of hasParamCallable is 1
EOF;

if ($ret != $expect) {
    exit(1);
}
