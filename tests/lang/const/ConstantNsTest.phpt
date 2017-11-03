<?php
ob_start();
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
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
\zapi\SYS_VERSION defined
\zapi\io\IO_TYPE defined
\zapi\io\NATIVE_STREAM defined
EOF;

if ($ret != $expect) {
    exit(1);
}

