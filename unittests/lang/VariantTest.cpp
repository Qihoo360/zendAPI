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
      Variant var2(std::numeric_limits<int16_t>::max());
      ASSERT_EQ(Z_LVAL_P(var2.getZval()), std::numeric_limits<int16_t>::max());
      Variant var3(std::numeric_limits<int32_t>::min());
      ASSERT_EQ(Z_LVAL_P(var3.getZval()), std::numeric_limits<int32_t>::min());
      Variant var4(std::numeric_limits<int32_t>::max());
      ASSERT_EQ(Z_LVAL_P(var4.getZval()), std::numeric_limits<int32_t>::max());
      Variant var5(std::numeric_limits<int64_t>::min());
      ASSERT_EQ(Z_LVAL_P(var5.getZval()), std::numeric_limits<int64_t>::min());
      Variant var6(std::numeric_limits<int64_t>::max());
      ASSERT_EQ(Z_LVAL_P(var6.getZval()), std::numeric_limits<int64_t>::max());
   }
}
