--TEST--
Class magic constructor and destructor test
--FILE--
<?php

if (class_exists("\ConstructAndDestruct")) {
    {
        $object = new \ConstructAndDestruct();
        $object = null;
    }
}
?>
--EXPECT--
constructor been invoked
destructor been invoked
