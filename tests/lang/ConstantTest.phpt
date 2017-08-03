--TEST--
Contant register test
--FILE--
<?php

if(defined("ZAPI_NAME"))
{
   echo ZAPI_NAME;
}
echo " ";
if(defined("ZAPI_VERSION"))
{
   echo ZAPI_VERSION;
}
echo " ";
//show_something();
?>
--EXPECT--
zapi v0.0.1 hello world, zapi
