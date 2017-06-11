//
// Created by softboy on 18/05/2017.
//
#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/vm/Zval.h"

#include "php/sapi/embed/php_embed.h"
#include "php/Zend/zend_types.h"

#include "gtest/gtest.h"

#include <iostream>
#include <limits>

using zapi::lang::Variant;
using zapi::vm::Zval;

namespace
{

class VariantTest : public ::testing::Test
{
   
};

}

TEST_F(VariantTest, testConstructor)
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
      ASSERT_EQ(Z_TYPE_P(var1.getZval()), IS_LONG);
      Variant var2(std::numeric_limits<int16_t>::max());
      ASSERT_EQ(Z_LVAL_P(var2.getZval()), std::numeric_limits<int16_t>::max());
      ASSERT_EQ(Z_TYPE_P(var2.getZval()), IS_LONG);
      Variant var3(std::numeric_limits<int32_t>::min());
      ASSERT_EQ(Z_LVAL_P(var3.getZval()), std::numeric_limits<int32_t>::min());
      Variant var4(std::numeric_limits<int32_t>::max());
      ASSERT_EQ(Z_LVAL_P(var4.getZval()), std::numeric_limits<int32_t>::max());
      ASSERT_EQ(Z_TYPE_P(var4.getZval()), IS_LONG);
      Variant var5(std::numeric_limits<int64_t>::min());
      ASSERT_EQ(Z_LVAL_P(var5.getZval()), std::numeric_limits<int64_t>::min());
      ASSERT_EQ(Z_TYPE_P(var5.getZval()), IS_LONG);
      Variant var6(std::numeric_limits<int64_t>::max());
      ASSERT_EQ(Z_LVAL_P(var6.getZval()), std::numeric_limits<int64_t>::max());
      ASSERT_EQ(Z_TYPE_P(var6.getZval()), IS_LONG);
      // test boolean
      Variant trueVariant(true);
      ASSERT_EQ(Z_TYPE_P(trueVariant.getZval()), IS_TRUE);
      Variant falseVariant(false);
      ASSERT_EQ(Z_TYPE_P(falseVariant.getZval()), IS_FALSE);
      // char constructor test
      Variant charVariant('a');
      char *retCharArr = Z_STRVAL_P(charVariant.getZval());
      ASSERT_EQ(retCharArr[0], 'a');
      ASSERT_EQ(Z_STRLEN_P(charVariant.getZval()), 1);
   }
}

int main(int argc, char **argv)
{
   int retCode = 0;
   PHP_EMBED_START_BLOCK(argc,argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}

