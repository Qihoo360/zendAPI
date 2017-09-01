--TEST--
Function reference argument test
--FILE--
<?php

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
?>
--EXPECT--
321
123