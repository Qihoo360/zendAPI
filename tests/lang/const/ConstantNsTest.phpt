<?php

if(!defined("\zapi\SYS_VERSION"))
{
   goto error;
}

if(!defined("\zapi\io\IO_TYPE"))
{
   goto error;
}

if(!defined("\zapi\io\NATIVE_STREAM"))
{
   goto error;
}

success:
exit(0);
error:
exit(1);
