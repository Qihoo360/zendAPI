--TEST--
interface inherit test
--FILE--
<?php

if (interface_exists("InterfaceA") && interface_exists("InterfaceB") && interface_exists("InterfaceC")) {
   if (is_subclass_of("InterfaceC", "InterfaceB")) {
      echo "InterfaceC parent class is InterfaceB\n";
   }
   if (is_subclass_of("InterfaceC", "InterfaceA")) {
      echo "InterfaceC parent class is InterfaceA\n";
   }
   if (is_subclass_of("InterfaceB", "InterfaceA")) {
      echo "InterfaceB parent class is InterfaceA\n";
   }
   // method of interface
   if (method_exists("InterfaceA", "methodOfA")) {
      echo "method InteraceA::methodOfA exists\n";
   }
   if (method_exists("InterfaceA", "protectedMethodOfA")) {
      echo "method InteraceA::protectedMethodOfA exists\n";
   }
   if (method_exists("InterfaceA", "privateMethodOfA")) {
      echo "method InteraceA::privateMethodOfA exists\n";
   }
   
   if (method_exists("InterfaceB", "methodOfB")) {
      echo "method InterfaceB::methodOfA exists\n";
   }
   if (method_exists("InterfaceB", "protectedMethodOfB")) {
      echo "method InterfaceB::protectedMethodOfB exists\n";
   }
   if (method_exists("InterfaceB", "privateMethodOfB")) {
      echo "method InterfaceB::privateMethodOfB exists\n";
   }
   
   if (method_exists("InterfaceC", "methodOfC")) {
      echo "method InterfaceC::methodOfC exists\n";
   }
   if (method_exists("InterfaceC", "protectedMethodOfC")) {
      echo "method InterfaceC::protectedMethodOfC exists\n";
   }
   if (method_exists("InterfaceC", "privateMethodOfC")) {
      echo "method InterfaceC::privateMethodOfC exists\n";
   }
   
    if (method_exists("InterfaceC", "methodOfA")) {
       echo "method InterfaceC::methodOfA exists\n";
    }
    if (method_exists("InterfaceC", "protectedMethodOfA")) {
       echo "method InterfaceC::protectedMethodOfA exists\n";
    }
    if (method_exists("InterfaceC", "privateMethodOfA")) {
       echo "method InterfaceC::privateMethodOfA exists\n";
    }
    if (method_exists("InterfaceC", "methodOfB")) {
       echo "method InterfaceC::methodOfB exists\n";
    }
    if (method_exists("InterfaceC", "protectedMethodOfB")) {
       echo "method InterfaceC::protectedMethodOfB exists\n";
    }
    if (method_exists("InterfaceC", "privateMethodOfB")) {
       echo "method InterfaceC::privateMethodOfB exists\n";
    }
}

?>
--EXPECT--
InterfaceC parent class is InterfaceB
InterfaceC parent class is InterfaceA
InterfaceB parent class is InterfaceA
method InteraceA::methodOfA exists
method InteraceA::protectedMethodOfA exists
method InteraceA::privateMethodOfA exists
method InterfaceB::methodOfA exists
method InterfaceB::protectedMethodOfB exists
method InterfaceB::privateMethodOfB exists
method InterfaceC::methodOfC exists
method InterfaceC::protectedMethodOfC exists
method InterfaceC::privateMethodOfC exists
method InterfaceC::methodOfA exists
method InterfaceC::protectedMethodOfA exists
method InterfaceC::privateMethodOfA exists
method InterfaceC::methodOfB exists
method InterfaceC::protectedMethodOfB exists
method InterfaceC::privateMethodOfB exists

