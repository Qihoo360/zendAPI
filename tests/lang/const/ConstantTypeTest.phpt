<?php
ob_start();
if(defined("MY_CONST"))
{
   if (is_int(MY_CONST)) {
      echo "MY_CONST is int\n";
   }
}
if(defined("PI"))
{
   if (is_double(PI)) {
      echo "PI is double\n";
   }
}
if(defined("ZAPI_NAME"))
{
   if (is_string(ZAPI_NAME)) {
      echo "ZAPI_NAME is string\n";
   }
}
if(defined("ZAPI_VERSION"))
{
   if (is_string(ZAPI_VERSION)) {
      echo "ZAPI_VERSION is string\n";
   }
}
if(defined("QIHOO"))
{
   if (is_string(QIHOO)) {
      echo "QIHOO is string\n";
   }
}
if(defined("\zapi\SYS_VERSION"))
{
   if (is_string(\zapi\SYS_VERSION)) {
      echo "\zapi\SYS_VERSION is string\n";
   }
}
if(defined("\zapi\io\IO_TYPE"))
{
   if (is_string(\zapi\io\IO_TYPE)) {
      echo "\zapi\io\IO_TYPE is string\n";
   }
}
if(defined("\zapi\io\NATIVE_STREAM"))
{
   if (is_bool(\zapi\io\NATIVE_STREAM)) {
      echo "\zapi\io\NATIVE_STREAM is bool\n";
   }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
MY_CONST is int
PI is double
ZAPI_NAME is string
ZAPI_VERSION is string
QIHOO is string
\zapi\SYS_VERSION is string
\zapi\io\IO_TYPE is string
\zapi\io\NATIVE_STREAM is bool
EOF;

if ($ret != $expect) {
    exit(1);
}

