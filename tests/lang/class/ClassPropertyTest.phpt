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