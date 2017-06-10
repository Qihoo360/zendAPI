//
// Created by softboy on 18/05/2017.
//
#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/vm/Zval.h"

#include "php/Zend/zend_types.h"

#include "gtest/gtest.h"

#include <iostream>
#include <limits>

using zapi::lang::Variant;
using zapi::vm::Zval;

TEST(VariantTest, testConstructor)
{
   {
      Variant var(nullptr);
      const Zval &val = var.getZval();
      ASSERT_TRUE(Z_TYPE_P(val) == IS_NULL);
   }
   {
      int16_t intValue = 12;
      Variant var(intValue);
      const Zval &val = var.getZval();
      ASSERT_EQ(Z_LVAL_P(val), intValue);
      Variant var1(std::numeric_limits<int16_t>::min());
      const Zval &val1 = var1.getZval();
      ASSERT_EQ(Z_LVAL_P(val1), std::numeric_limits<int16_t>::min());
   }
}
