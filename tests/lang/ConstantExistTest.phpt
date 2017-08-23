--TEST--
Contant register test
--FILE--
<?php

if(defined("MY_CONST"))
{
   echo "MY_CONST defined\n";
}

if(defined("PI"))
{
   echo "PI defined\n";
}

if(defined("ZAPI_NAME"))
{
   echo "ZAPI_NAME defined\n";
}

if(defined("ZAPI_VERSION"))
{
   echo "ZAPI_NAME defined\n";
}

if(defined("QIHOO"))
{
   echo "QIHOO defined\n";
}

if(defined("\zapi\SYS_VERSION"))
{
   echo "\zapi\SYS_VERSION defined\n";
}

if(defined("\zapi\io\IO_TYPE"))
{
   echo "\zapi\io\IO_TYPE defined\n";
}

if(defined("\zapi\io\NATIVE_STREAM"))
{
   echo "\zapi\io\NATIVE_STREAM defined\n";
}

?>
--EXPECT--
MY_CONST defined
PI defined
ZAPI_NAME defined
ZAPI_NAME defined
QIHOO defined
\zapi\SYS_VERSION defined
\zapi\io\IO_TYPE defined
\zapi\io\NATIVE_STREAM defined