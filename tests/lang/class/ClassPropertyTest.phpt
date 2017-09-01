--TEST--
Class property test
--FILE--
<?php

if (class_exists("PropsTestClass")) {
    // instance property
    $object = new PropsTestClass();
    if (property_exists($object, "nullProp")) {
        echo "PropsTestClass::nullProp is exist\n";
        echo "PropsTestClass::nullProp value : ". (is_null($object->nullProp) ? "null\n" : "notnull\n");
    }
    if (property_exists($object, "trueProp")) {
        echo "PropsTestClass::trueProp is exist\n";
        echo "PropsTestClass::trueProp value : " . ($object->trueProp ? "true" : "false");
        echo "\n";
    }
    if (property_exists($object, "falseProp")) {
        echo "PropsTestClass::falseProp is exist\n";
        echo "PropsTestClass::falseProp value : " . ($object->falseProp ? "true" : "false");
        echo "\n";
    }
    if (property_exists($object, "numProp")) {
        echo "PropsTestClass::numProp is exist\n";
        echo "PropsTestClass::numProp value : {$object->numProp}\n";
    }
    if (property_exists($object, "doubleProp")) {
        echo "PropsTestClass::doubleProp is exist\n";
        echo "PropsTestClass::doubleProp value : {$object->doubleProp}\n";
    }
    if (property_exists($object, "strProp")) {
        echo "PropsTestClass::strProp is exist\n";
        echo "PropsTestClass::strProp value : {$object->strProp}\n";
    }
    
    if (property_exists($object, "strProp1")) {
        echo "PropsTestClass::strProp1 is exist\n";
        echo "PropsTestClass::strProp1 value : {$object->strProp1}\n";
    }
    
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
}

?>
--EXPECT--
PropsTestClass::nullProp is exist
PropsTestClass::nullProp value : null
PropsTestClass::trueProp is exist
PropsTestClass::trueProp value : true
PropsTestClass::falseProp is exist
PropsTestClass::falseProp value : false
PropsTestClass::numProp is exist
PropsTestClass::numProp value : 2017
PropsTestClass::doubleProp is exist
PropsTestClass::doubleProp value : 3.1415
PropsTestClass::strProp is exist
PropsTestClass::strProp value : zapi
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