<?php

if(!defined("MY_CONST"))
{
   goto error;
}

if(!defined("PI"))
{
   goto error;
}

if(!defined("ZAPI_NAME"))
{
   goto error;
}

if(!defined("ZAPI_VERSION"))
{
   goto error;
}

if(!defined("QIHOO"))
{
   goto error;
}

success:
exit(0);
error:
exit(1);
