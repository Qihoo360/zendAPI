--TEST--
extension ini test
--FILE--
<?php

$zapi_team_address = ini_get("zapi_team_address")."\n";
echo "ini value zapi_team_address: ". ini_get("zapi_team_address")."\n";
echo "ini value zapi_product: ". ini_get("zapi_product")."\n";
echo "ini value zapi_enable_gpu: ". ini_get("zapi_enable_gpu")."\n";
echo "ini value zapi_name: ". ini_get("zapi_name")."\n";
echo "ini value zapi_age: ". ini_get("zapi_age")."\n";
if (empty(ini_get("zapi_not_registered"))) {
    echo "ini value zapi_not_registered is empty\n";
}
// test data type

$strvalue = ini_get("zapi_string_value");
$intvalue = intval(ini_get("zapi_int_value"));
$boolvalue = boolval(ini_get("zapi_bool_value"));
$doublevalue = doubleval(ini_get("zapi_double_value"));
if (!empty($strvalue) && is_string($strvalue)) {
    echo "ini value zapi_string_value is string value\n";
}
if (!empty($intvalue) && is_int($intvalue) && 2017 == $intvalue) {
    echo "ini value zapi_int_value is int value\n";
}
if (!empty($boolvalue) && is_bool($boolvalue)) {
    echo "ini value zapi_bool_value is bool value\n";
}
if (!empty($doublevalue) && is_double($doublevalue)) {
    echo "ini value zapi_double_value is double value\n";
}

?>
--EXPECT--
ini value zapi_team_address: shanghai
ini value zapi_product: libpdk
ini value zapi_enable_gpu: 1
ini value zapi_name: zzu_softboy
ini value zapi_age: 27
ini value zapi_not_registered is empty
ini value zapi_string_value is string value
ini value zapi_int_value is int value
ini value zapi_bool_value is bool value
ini value zapi_double_value is double value
