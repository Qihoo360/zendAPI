<?php
ob_start();
if (class_exists("\IterateTestClass")) {
    $data = new IterateTestClass();
    foreach ($data as $key => $value) {
        echo "key: $key value: $value\n";
    }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
IterateTestClass::rewind called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
key: key1 value: value1
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
key: key2 value: value2
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
key: key3 value: value3
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
key: key4 value: value4
IterateTestClass::next called
IterateTestClass::valid called
EOF;
if ($ret != $expect) {
    exit(1);
}


