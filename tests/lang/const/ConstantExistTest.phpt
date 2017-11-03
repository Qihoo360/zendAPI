<?php
ob_start();
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
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
MY_CONST defined
PI defined
ZAPI_NAME defined
ZAPI_NAME defined
QIHOO defined
EOF;

if ($ret != $expect) {
    exit(1);
}
