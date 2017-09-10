--TEST--
extension info test
--FILE--
<?php

if (function_exists("get_ext_handler_msgs")) {
    $msgs = get_ext_handler_msgs();
    var_dump($msgs);
    // here we just get two info
}

?>
--EXPECT--
array(2) {
  [0]=>
  string(29) "module startup handler called"
  [1]=>
  string(30) "request startup handler called"
}
