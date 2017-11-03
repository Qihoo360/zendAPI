<?php
ob_start();
if (class_exists("PropsTestClass")) {

    if (property_exists("PropsTestClass", "staticNullProp")) {
        echo "PropsTestClass::staticNullProp is exist\n";
        echo "PropsTestClass::staticNullProp value : ". (is_null(PropsTestClass::$staticNullProp) ? "null" : "notnull")."\n";
    }
    if (property_exists("PropsTestClass", "staticTrueProp")) {
        echo "PropsTestClass::staticTrueProp is exist\n";
        echo "PropsTestClass::staticTrueProp value : ". (PropsTestClass::$staticTrueProp ? "true" : "false")."\n";
    }
    if (property_exists("PropsTestClass", "staticFalseProp")) {
        echo "PropsTestClass::staticFalseProp is exist\n";
        echo "PropsTestClass::staticFalseProp value : ". (PropsTestClass::$staticFalseProp ? "true" : "false")."\n";
    }
    if (property_exists("PropsTestClass", "staticNumProp")) {
        echo "PropsTestClass::staticNumProp is exist\n";
        echo "PropsTestClass::staticNumProp value : ". PropsTestClass::$staticNumProp."\n";
    }
    if (property_exists("PropsTestClass", "staticDoubleProp")) {
        echo "PropsTestClass::staticDoubleProp is exist\n";
        echo "PropsTestClass::staticDoubleProp value : ". PropsTestClass::$staticDoubleProp."\n";
    }
    if (property_exists("PropsTestClass", "staticStrProp")) {
        echo "PropsTestClass::staticStrProp is exist\n";
        echo "PropsTestClass::staticStrProp value : ". PropsTestClass::$staticStrProp."\n";
    }
    if (property_exists("PropsTestClass", "staticStrProp1")) {
        echo "PropsTestClass::staticStrProp1 is exist\n";
        echo "PropsTestClass::staticStrProp1 value : ". PropsTestClass::$staticStrProp1."\n";
    }
    if (defined("PropsTestClass::MATH_PI")) {
        echo "PropsTestClass::MATH_PI is exist\n";
        echo "PropsTestClass::MATH_PI value : ". PropsTestClass::MATH_PI ."\n";
    }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
PropsTestClass::staticNullProp is exist
PropsTestClass::staticNullProp value : null
PropsTestClass::staticTrueProp is exist
PropsTestClass::staticTrueProp value : true
PropsTestClass::staticFalseProp is exist
PropsTestClass::staticFalseProp value : false
PropsTestClass::staticNumProp is exist
PropsTestClass::staticNumProp value : 2012
PropsTestClass::staticDoubleProp is exist
PropsTestClass::staticDoubleProp value : 3.1415
PropsTestClass::staticStrProp is exist
PropsTestClass::staticStrProp value : static zapi
PropsTestClass::MATH_PI is exist
PropsTestClass::MATH_PI value : 3.14
EOF;

if ($ret != $expect) {
    exit(1);
}
