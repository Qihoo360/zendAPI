--TEST--
test function default argument pass
--FILE--
<?php

// here we test argument passed
if (function_exists("say_hello")) {
    say_hello();
    say_hello("unicornteam");
    say_hello("zzu_softboy");
    say_hello(3.14); // here will convert into string
}
?>
--EXPECT--
hello, zapi
hello, unicornteam
hello, zzu_softboy
hello, 3.14