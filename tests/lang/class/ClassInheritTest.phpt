<?php

if (class_exists("A") && class_exists("B") && class_exists("C")) {
    if (!(new B instanceof \A)) {
       goto error;
    }
    $reflClsB = new ReflectionClass("B");
    $parentOfClsB = $reflClsB->getParentClass();
    if ($parentOfClsB->getName() != "A") {
        goto error;
    }
    $reflClsC = new ReflectionClass("C");
    $parentOfClsC = $reflClsC->getParentClass();
    if ($parentOfClsC->getName() != "B") {
        goto error;
    }
    if (!(new C instanceof \B)) {
       goto error;
    }
    if (!(new C instanceof \A)) {
       goto error; 
    }
} else {
    goto error;
}

success:
exit(0);
error:
exit(1);
