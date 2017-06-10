//
// Created by softboy on 18/05/2017.
//
#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/vm/Zval.h"

#include "gtest/gtest.h"

#include <iostream>

using zapi::lang::Variant;
using zapi::vm::Zval;

TEST(VariantTest, testConstructor)
{
   Variant var(nullptr);
   const Zval &val = var.getZval();
   std::cout << "xiuxiux" << std::endl;
}
