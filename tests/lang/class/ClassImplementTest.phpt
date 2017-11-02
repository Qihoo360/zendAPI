--TEST--
interface implement test
--FILE--
<?php

if (class_exists("C") && interface_exists("InterfaceA") && interface_exists("InterfaceB") && interface_exists("InterfaceC")) {
    $obj = new C();
    var_dump(class_implements("C"));
    if ($obj instanceof InterfaceA) {
        echo "\$obj instanceof InterfaceA\n";
    }
    if (method_exists($obj, "methodOfA")) {
       echo "method C::methodOfA exists\n";
    }
    if (method_exists($obj, "protectedMethodOfA")) {
       echo "method C::protectedMethodOfA exists\n";
    }
    if (method_exists($obj, "privateMethodOfA")) {
       echo "method C::privateMethodOfA exists\n";
    }
}

?>
--EXPECT--
array(1) {
  ["InterfaceA"]=>
  string(10) "InterfaceA"
}
$obj instanceof InterfaceA
method C::methodOfA exists
method C::protectedMethodOfA exists
method C::privateMethodOfA exists

