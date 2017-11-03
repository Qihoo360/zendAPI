<?php
ob_start();
if (class_exists("Person")) {
   if (defined("Person::UNICORNTEAM")) {
      echo "Person::UNICORNTEAM is ".Person::UNICORNTEAM."\n";
   }
   if (defined("Person::MY_CONST")) {
      echo "Person::MY_CONST is ".Person::MY_CONST."\n";
   }
   if (defined("Person::PI")) {
      echo "Person::PI is ".Person::PI."\n";
   }
   if (defined("Person::HEADER_SIZE")) {
      echo "Person::HEADER_SIZE is ".Person::HEADER_SIZE."\n";
   }
   if (defined("Person::ALLOW_ACL")) {
      echo "Person::ALLOW_ACL is ".(Person::ALLOW_ACL ? "true": "false")."\n";
   }
}
$ret = trim(ob_get_clean());
$expect = <<<EOF
Person::UNICORNTEAM is beijing unicornteam
Person::MY_CONST is MY_CONST_VALUE
Person::PI is 3.1415926
Person::HEADER_SIZE is 123
Person::ALLOW_ACL is true
EOF;

if ($ret != $expect) {
    exit(1);
}
