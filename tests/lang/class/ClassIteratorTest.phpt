<?php
$targetStr = "";
if (class_exists("\IterateTestClass")) {
    $data = new IterateTestClass();
    ob_start();
    foreach ($data as $key => $value) {
        $targetStr .= "key: $key value: $value\n";
    }
    $targetStr .= ob_get_clean();
} else {
    goto error;
}

$expected = <<<EOD
key: key1 value: value1
key: key2 value: value2
key: key3 value: value3
key: key4 value: value4
IterateTestClass::rewind called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
IterateTestClass::next called
IterateTestClass::valid called
IterateTestClass::current called
IterateTestClass::key called
IterateTestClass::next called
IterateTestClass::valid called

EOD;

if ($targetStr != $expected) {
    goto error;
}

success:
exit(0);
error:
exit(1);
