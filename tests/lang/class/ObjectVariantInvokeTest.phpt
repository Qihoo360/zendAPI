<?php
ob_start();
if (class_exists("ObjectVariantClass")) {
    $object = new ObjectVariantClass();
    $object->forwardInvoke();
}
$ret = trim(ob_get_clean());
$expect = <<<'EOF'
begin invoke ObjectVariant::classInvoke : the text is xxx
ObjectVariantClass::__invoke invoked
after invoke ObjectVariant::classInvoke : this text is zapi
EOF;

if ($ret != $expect) {
    exit(1);
}
