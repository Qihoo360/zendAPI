<?php

if (interface_exists("InterfaceA") && interface_exists("InterfaceB") && interface_exists("InterfaceC")) {
   if (!is_subclass_of("InterfaceC", "InterfaceB")) {
      goto error;
   }
   if (!is_subclass_of("InterfaceC", "InterfaceA")) {
      goto error;
   }
   if (!is_subclass_of("InterfaceB", "InterfaceA")) {
      goto error;
   }
   // method of interface
   if (!method_exists("InterfaceA", "methodOfA")) {
      goto error;
   }
   if (!method_exists("InterfaceA", "protectedMethodOfA")) {
      goto error;
   }
   if (!method_exists("InterfaceA", "privateMethodOfA")) {
      goto error;
   }
   
   if (!method_exists("InterfaceB", "methodOfB")) {
      goto error;
   }
   if (!method_exists("InterfaceB", "protectedMethodOfB")) {
      goto error;
   }
   if (!method_exists("InterfaceB", "privateMethodOfB")) {
      goto error;
   }
   
   if (!method_exists("InterfaceC", "methodOfC")) {
      goto error;
   }
   if (!method_exists("InterfaceC", "protectedMethodOfC")) {
      goto error;
   }
   if (!method_exists("InterfaceC", "privateMethodOfC")) {
     goto error;
   }
   
    if (!method_exists("InterfaceC", "methodOfA")) {
       goto error;
    }
    if (!method_exists("InterfaceC", "protectedMethodOfA")) {
       goto error;
    }
    if (!method_exists("InterfaceC", "privateMethodOfA")) {
       goto error;
    }
    if (!method_exists("InterfaceC", "methodOfB")) {
       goto error;
    }
    if (!method_exists("InterfaceC", "protectedMethodOfB")) {
       goto error;
    }
    if (!method_exists("InterfaceC", "privateMethodOfB")) {
       goto error;
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
