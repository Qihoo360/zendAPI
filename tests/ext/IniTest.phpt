<?php

if ("shanghai" != ini_get("zapi_team_address")) {
    goto error;
}
if ("libpdk" != ini_get("zapi_product")) {
    goto error;
}
if (1 != ini_get("zapi_enable_gpu")) {
    goto error;
}
if ("zzu_softboy" != ini_get("zapi_name")) {
    goto error;
}
if ("27" != ini_get("zapi_age")) {
    goto error;
}
if (!empty(ini_get("zapi_not_registered"))) {
    goto error;
}
$strvalue = ini_get("zapi_string_value");
$intvalue = intval(ini_get("zapi_int_value"));
$boolvalue = boolval(ini_get("zapi_bool_value"));
$doublevalue = doubleval(ini_get("zapi_double_value"));

if (empty($strvalue) || !is_string($strvalue)) {
    goto error;
}

if (empty($intvalue) || !is_int($intvalue) || 2017 != $intvalue) {
    goto error;
}

if (empty($boolvalue) || !is_bool($boolvalue)) {
    goto error;
}

if (empty($doublevalue) || !is_double($doublevalue)) {
    goto error;
}

success:
exit(0);
error:
exit(1);
