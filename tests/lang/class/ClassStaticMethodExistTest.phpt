<?php
ob_start();
if (class_exists("Person")) {
   if (method_exists("Person", "staticShowName")) {
       echo "static method Person::staticShowName exists\n";
   }
   if (method_exists("Person", "staticProtectedMethod")) {
       echo "static method Person::staticProtectedMethod exists\n";
   }
   if (method_exists("Person", "staticPrivateMethod")) {
       echo "static method Person::staticPrivateMethod exists\n";
   }
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
static method Person::staticShowName exists
static method Person::staticProtectedMethod exists
static method Person::staticPrivateMethod exists
EOF;

if ($ret != $expect) {
    exit(1);
}

