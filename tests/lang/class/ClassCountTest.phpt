--TEST--
Class count test
--FILE--
<?php

if (class_exists("\IterateTestClass")) {
    $data = new IterateTestClass();
    echo "the count of \$data is ".count($data)."\n";
}

?>
--EXPECT--
IterateTestClass::count called
the count of $data is 4
