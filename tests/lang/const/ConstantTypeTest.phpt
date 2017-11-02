<?php

if(!defined("MY_CONST") || !is_int(MY_CONST))
{
   goto error;
}

if(!defined("PI") || !is_double(PI))
{
   goto error;
}

if(!defined("ZAPI_NAME") || !is_string(ZAPI_NAME))
{
   goto error;
}

if(!defined("ZAPI_VERSION") || !is_string(ZAPI_VERSION))
{
   goto error;
}

if(!defined("QIHOO") || !is_string(QIHOO))
{
   goto error;
}

if(!defined("\zapi\SYS_VERSION") || !is_string(\zapi\SYS_VERSION))
{
   goto error;
}

if(!defined("\zapi\io\IO_TYPE") || !is_string(\zapi\io\IO_TYPE))
{
   goto error;
}

if(!defined("\zapi\io\NATIVE_STREAM") || !is_bool(\zapi\io\NATIVE_STREAM))
{
   goto error;
}

success:
exit(0);
error:
exit(1);
