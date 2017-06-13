//
// Created by softboy on 18/05/2017.
//
#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/vm/Zval.h"
#include "zapi/lang/FatalError.h"
#include "zapi/lang/Type.h"

#include "php/sapi/embed/php_embed.h"
#include "php/Zend/zend_types.h"

#include "gtest/gtest.h"

#include <iostream>
#include <limits>
#include <string>
#include <cstring>

using zapi::lang::Variant;
using zapi::lang::FatalError;
using zapi::lang::Type;
using zapi::vm::Zval;

namespace
{

class VariantTest : public ::testing::Test
{
   
};

}

TEST_F(VariantTest, testConstructor)
{
   Variant nullvar(nullptr);
   const Zval &nullval = nullvar.getZval();
   ASSERT_TRUE(Z_TYPE_P(nullval) == IS_NULL);
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
   // test constructor(const std::string &value)
   std::string zapiStr("zapi");
   Variant zapiStrVar(zapiStr);
   char *zapiStrPtr = Z_STRVAL_P(zapiStrVar.getZval());
   ASSERT_EQ(Z_STRLEN_P(zapiStrVar.getZval()), 4);
   ASSERT_EQ(std::strncmp(zapiStrPtr, "zapi", 4), 0);
   ASSERT_EQ(Z_TYPE_P(zapiStrVar.getZval()), IS_STRING);
   // test constructor(const char *value, int length = -1)
   Variant sizeStr1("hello zapi!", -1);
   ASSERT_EQ(Z_STRLEN_P(sizeStr1.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr1.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr1.getZval()), IS_STRING);
   Variant sizeStr2("hello zapi!", 5);
   ASSERT_EQ(Z_STRLEN_P(sizeStr2.getZval()), 5);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr2.getZval()), "hello", 5), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr2.getZval()), IS_STRING);
   Variant sizeStr3("hello zapi!", 20);
   ASSERT_EQ(Z_STRLEN_P(sizeStr3.getZval()), 20);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr3.getZval()), "hello zapi!\0\0\0\0\0\0\0\0\0", 20), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr3.getZval()), IS_STRING);
   Variant sizeStr4("hello zapi!");
   ASSERT_EQ(Z_STRLEN_P(sizeStr4.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr4.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr4.getZval()), IS_STRING);
   // test constructor(double value)
   double dvalue = 0.12;
   Variant doubleVar(dvalue);
   ASSERT_EQ(Z_TYPE_P(doubleVar.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL_P(doubleVar.getZval()), 0.12);
   double dvalue1 = -0.12;
   Variant doubleVar1(dvalue1);
   ASSERT_EQ(Z_TYPE_P(doubleVar1.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL_P(doubleVar1.getZval()), -0.12);
   // test type constructor
   ASSERT_THROW({Variant undefinedVar(Type::Undefined);}, FatalError);
   ASSERT_THROW({Variant resourceVar(Type::Resource);}, FatalError);
   ASSERT_THROW({Variant constVar(Type::Constant);}, FatalError);
   ASSERT_THROW({Variant constAstVar(Type::ConstantAST);}, FatalError);
   ASSERT_THROW({Variant callableVar(Type::Callable);}, FatalError);
   ASSERT_THROW({Variant refVar(Type::Reference);}, FatalError);
   Variant nullTypeVar(Type::Null);
   ASSERT_EQ(Z_TYPE_P(nullTypeVar.getZval()), IS_NULL);
   Variant falseTypeVar(Type::False);
   ASSERT_EQ(Z_TYPE_P(falseTypeVar.getZval()), IS_FALSE);
   Variant trueTypeVar(Type::True);
   ASSERT_EQ(Z_TYPE_P(trueTypeVar.getZval()), IS_TRUE);
   Variant longTypeVar(Type::Long);
   ASSERT_EQ(Z_TYPE_P(longTypeVar.getZval()), IS_LONG);
   Variant doubleTypeVar(Type::Double);
   ASSERT_EQ(Z_TYPE_P(doubleTypeVar.getZval()), IS_DOUBLE);
   Variant stringTypeVar(Type::String);
   ASSERT_EQ(Z_TYPE_P(stringTypeVar.getZval()), IS_STRING);
   Variant arrayTypeVar(Type::Array);
   ASSERT_EQ(Z_TYPE_P(arrayTypeVar.getZval()), IS_ARRAY);
   Variant objectTypeVar(Type::Object);
   ASSERT_EQ(Z_TYPE_P(objectTypeVar.getZval()), IS_OBJECT);
   
   // test construct from other _zval_struct *
   zval rawVar;
   ZVAL_LONG(&rawVar, 2017);
   Variant constructFromRaw(&rawVar);
   ASSERT_EQ(Z_LVAL_P(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_TYPE_P(constructFromRaw.getZval()), IS_LONG);
   ZVAL_LONG(&rawVar, 2018);
   ASSERT_EQ(Z_LVAL_P(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_LVAL_P(&rawVar), 2018);
   zval rawVar1;
   ZVAL_LONG(&rawVar1, 2017);
   Variant constructFromRawRef(&rawVar1, true);
   ASSERT_EQ(Z_TYPE_P(constructFromRawRef.getZval()), IS_LONG);
   ASSERT_EQ(Z_LVAL_P(constructFromRawRef.getZval()), 2017);
   // test copy constructor
   {
      Variant orig(1234);
      Variant me(orig);
      ASSERT_EQ(Z_TYPE_P(me.getZval()), Z_TYPE_P(orig.getZval()));
      ASSERT_EQ(Z_LVAL_P(me.getZval()), Z_LVAL_P(orig.getZval()));
      Variant origStr("zapi", 4);
      Variant meStr(origStr);
      ASSERT_EQ(Z_TYPE_P(meStr.getZval()), Z_TYPE_P(origStr.getZval()));
      ASSERT_EQ(Z_LVAL_P(meStr.getZval()), Z_LVAL_P(origStr.getZval()));
   }
   {
      // test move constructor 
      Variant orig("zapi", 4);
      Variant me(std::move(orig));
      ASSERT_EQ(Z_TYPE_P(orig.getZval()), IS_UNDEF);
      ASSERT_EQ(Z_TYPE_P(me.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(me.getZval()), "zapi", 4), 0);
   }
}

TEST_F(VariantTest, testAssignOperator)
{
   {
      Variant orig(2018);
      Variant me("hello", 5);
      me = orig;
      ASSERT_EQ(Z_TYPE_P(orig.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(me.getZval()), 2018);
   }
   {
      // test number assign operators
      Variant var(1);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 1);
      var = static_cast<int16_t>(123);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 123);
      var = std::numeric_limits<int16_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int16_t>::min());
      var = std::numeric_limits<int16_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int16_t>::max());
      var = static_cast<int32_t>(1232);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 1232);
      var = std::numeric_limits<int32_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int32_t>::min());
      var = std::numeric_limits<int32_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int32_t>::max());
      var = static_cast<int64_t>(2018);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 2018);
      var = std::numeric_limits<int64_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int64_t>::min());
      var = std::numeric_limits<int64_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int64_t>::max());
      // test double assign operators
      var = 0.12;
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_DOUBLE);
      ASSERT_EQ(Z_DVAL_P(var.getZval()), 0.12);
   }
   {
      // test boolean assign operators
      Variant booleanVar;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_NULL);
      booleanVar = true;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_TRUE);
      booleanVar = false;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_FALSE);
   }
   {
      // string assign operators
      Variant strVar;
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_NULL);
      strVar = std::string("zapi");
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "zapi", 4), 0);
      strVar = 'a';
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "a", 1), 0);
      strVar = "hello zapi";
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "hello zapi", 10), 0);
   }
}

TEST_F(VariantTest, testGetType)
{
   Variant var;
   ASSERT_EQ(var.getType(), Type::Null);
   ASSERT_TRUE(var.isNull());
   ASSERT_TRUE(var.isScalar());
   var = 1;
   ASSERT_EQ(var.getType(), Type::Long);
   ASSERT_TRUE(var.isNumeric());
   ASSERT_TRUE(var.isScalar());
   var = "xiuxiux";
   ASSERT_EQ(var.getType(), Type::String);
   ASSERT_TRUE(var.isString());
   ASSERT_TRUE(var.isScalar());
   var = 0.3;
   ASSERT_EQ(var.getType(), Type::Double);
   ASSERT_TRUE(var.isScalar());
   ASSERT_TRUE(var.isDouble());
   var = true;
   ASSERT_EQ(var.getType(), Type::True);
   ASSERT_TRUE(var.isScalar());
   ASSERT_TRUE(var.isBoolean());
   var = false;
   ASSERT_EQ(var.getType(), Type::False);
   ASSERT_TRUE(var.isScalar());
   ASSERT_TRUE(var.isBoolean());
}

TEST_F(VariantTest, testSetType)
{
   Variant var;
   ASSERT_EQ(var.getType(), Type::Null);
   var.setType(Type::True);
   ASSERT_EQ(var.getType(), Type::True);
   var.setType(Type::False);
   ASSERT_EQ(var.getType(), Type::False);
   var.setType(Type::String);
   ASSERT_EQ(var.getType(), Type::String);
   var.setType(Type::Long);
   ASSERT_EQ(var.getType(), Type::Long);
   var.setType(Type::Double);
   ASSERT_EQ(var.getType(), Type::Double);
   var.setType(Type::Array);
   ASSERT_EQ(var.getType(), Type::Array);
   var.setType(Type::Object);
   ASSERT_EQ(var.getType(), Type::Object);
   // unsupported types
   ASSERT_THROW({var.setType(Type::Undefined);}, FatalError);
   ASSERT_THROW({var.setType(Type::Resource);}, FatalError);
   ASSERT_THROW({var.setType(Type::Constant);}, FatalError);
   ASSERT_THROW({var.setType(Type::ConstantAST);}, FatalError);
   ASSERT_THROW({var.setType(Type::Callable);}, FatalError);
}

TEST_F(VariantTest, testClone)
{
   {
      Variant var(2018);
      Variant cloned = var.clone();
      ASSERT_EQ(cloned.getType(), var.getType());
      ASSERT_EQ(cloned.getNumericValue(), var.getNumericValue());
   }
   {
      Variant var(true);
      Variant cloned = var.clone();
      ASSERT_EQ(cloned.getType(), var.getType());
      ASSERT_EQ(cloned.getBooleanValue(), var.getBooleanValue());
   }
   {
      Variant var("zapi");
      Variant cloned = var.clone();
      ASSERT_EQ(cloned.getType(), var.getType());
      ASSERT_EQ(cloned.getStringValue(), var.getStringValue());
   }
   {
      Variant var(static_cast<double>(0.32));
      Variant cloned = var.clone();
      ASSERT_EQ(cloned.getType(), var.getType());
      ASSERT_EQ(cloned.getDoubleValue(), var.getDoubleValue());
   }
   // set type when clone
   {
      Variant var(static_cast<double>(0.32));
      Variant cloned = var.clone(Type::Long);
      ASSERT_EQ(cloned.getType(), Type::Long);
      ASSERT_EQ(var.getDoubleValue(), 0.32);
      ASSERT_EQ(cloned.getNumericValue(), 0);
   }
}

TEST_F(VariantTest, testPlusEqOperator)
{
   {
      Variant numberVar(0);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += 1;
      ASSERT_EQ(numberVar.getNumericValue(), 1);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += 5;
      ASSERT_EQ(numberVar.getNumericValue(), 6);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += true;
      ASSERT_EQ(numberVar.getNumericValue(), 7);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += false;
      ASSERT_EQ(numberVar.getNumericValue(), 7);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += "12";
      ASSERT_EQ(numberVar.getNumericValue(), 19);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += "1.2";
      ASSERT_EQ(numberVar.getNumericValue(), 20);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += static_cast<int16_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 21);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += static_cast<int32_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 22);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += static_cast<int64_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 23);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += std::string("2");
      ASSERT_EQ(numberVar.getNumericValue(), 25);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += 'a'; // convert to 0
      ASSERT_EQ(numberVar.getNumericValue(), 25);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar += '3';
      ASSERT_EQ(numberVar.getNumericValue(), 28);
      ASSERT_EQ(numberVar.getType(), Type::Long);
   }
   {
      Variant numberVar(static_cast<double>(0));
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += 1;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 1);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += 5.2;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += true;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 7.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += false;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 7.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += "12";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 19.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += "1.2";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 20.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += static_cast<int16_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 21.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += static_cast<int32_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 22.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += static_cast<int64_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 23.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += std::string("2");
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 25.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += 'a'; // convert to 0
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 25.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar += '3';
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 28.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
   }
}

TEST_F(VariantTest, testMinusEqOperators)
{
   {
      Variant numberVar(100);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= 1;
      ASSERT_EQ(numberVar.getNumericValue(), 99);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= 5;
      ASSERT_EQ(numberVar.getNumericValue(), 94);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= true;
      ASSERT_EQ(numberVar.getNumericValue(), 93);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= false;
      ASSERT_EQ(numberVar.getNumericValue(), 93);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= "12";
      ASSERT_EQ(numberVar.getNumericValue(), 81);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= "1.2";
      ASSERT_EQ(numberVar.getNumericValue(), 80);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= static_cast<int16_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 79);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= static_cast<int32_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 78);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= static_cast<int64_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 77);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= std::string("2");
      ASSERT_EQ(numberVar.getNumericValue(), 75);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= 'a'; // convert to 0
      ASSERT_EQ(numberVar.getNumericValue(), 75);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar -= '3';
      ASSERT_EQ(numberVar.getNumericValue(), 72);
      ASSERT_EQ(numberVar.getType(), Type::Long);
   }
   {
      Variant numberVar(static_cast<double>(100.0));
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= 1;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 99.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= 5.2;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 93.8);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= true;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 92.8);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= false;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 92.8);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= "12";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 80.8);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= "1.2";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 79.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= static_cast<int16_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 78.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= static_cast<int32_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 77.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= static_cast<int64_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 76.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= std::string("2");
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 74.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= 'a'; // convert to 0
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 74.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar -= '3';
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 71.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
   }
}

TEST_F(VariantTest, testMultiEqOperators)
{
   {
      Variant numberVar(1);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= 2;
      ASSERT_EQ(numberVar.getNumericValue(), 2);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= 5;
      ASSERT_EQ(numberVar.getNumericValue(), 10);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= true;
      ASSERT_EQ(numberVar.getNumericValue(), 10);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= false;
      ASSERT_EQ(numberVar.getNumericValue(), 0);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar = 1;
      numberVar *= "12";
      ASSERT_EQ(numberVar.getNumericValue(), 12);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= "1.2";
      ASSERT_EQ(numberVar.getNumericValue(), 12);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= static_cast<int16_t>(2);
      ASSERT_EQ(numberVar.getNumericValue(), 24);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= static_cast<int32_t>(2);
      ASSERT_EQ(numberVar.getNumericValue(), 48);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= static_cast<int64_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 48);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= std::string("2");
      ASSERT_EQ(numberVar.getNumericValue(), 96);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar *= 'a'; // convert to 0
      ASSERT_EQ(numberVar.getNumericValue(), 0);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar = 1;
      numberVar *= '2';
      ASSERT_EQ(numberVar.getNumericValue(), 2);
      ASSERT_EQ(numberVar.getType(), Type::Long);
   }
   {
      Variant numberVar(static_cast<double>(1.2));
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= 1;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 1.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= 2;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 2.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= true;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 2.4);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= false;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar = 2.4;
      numberVar *= "2.3";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 5.52);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= "1.2";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.624);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar = 6.6;
      numberVar *= static_cast<int16_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= static_cast<int32_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= static_cast<int64_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.6);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= std::string("2");
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 13.2);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar *= 'a'; // convert to 0
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar = 2.3;
      numberVar *= '3';
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 6.9);
      ASSERT_EQ(numberVar.getType(), Type::Double);
   }
}

TEST_F(VariantTest, DivideOperators)
{
   {
      Variant numberVar(10000);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= 2;
      ASSERT_EQ(numberVar.getNumericValue(), 5000);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= 3;
      ASSERT_EQ(numberVar.getNumericValue(), 1666);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= true;
      ASSERT_EQ(numberVar.getNumericValue(), 1666);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= "12";
      ASSERT_EQ(numberVar.getNumericValue(), 138);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= "1.2";
      ASSERT_EQ(numberVar.getNumericValue(), 138);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= static_cast<int16_t>(2);
      ASSERT_EQ(numberVar.getNumericValue(), 69);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= static_cast<int32_t>(2);
      ASSERT_EQ(numberVar.getNumericValue(), 34);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= static_cast<int64_t>(1);
      ASSERT_EQ(numberVar.getNumericValue(), 34);
      ASSERT_EQ(numberVar.getType(), Type::Long);
      numberVar /= std::string("2");
      ASSERT_EQ(numberVar.getNumericValue(), 17);
      numberVar /= '2';
      ASSERT_EQ(numberVar.getNumericValue(), 8);
      ASSERT_EQ(numberVar.getType(), Type::Long);
   }
   {
      Variant numberVar(static_cast<double>(1000));
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= 1.0;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 1000.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= 2.0;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 500.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= true;
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 500.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= "2.3";
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 217.39130434782609);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar = 217.0;
      numberVar /= static_cast<int16_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(),  217.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= static_cast<int32_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 217.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= static_cast<int64_t>(1);
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 217.0);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      numberVar /= std::string("2");
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 108.5);
      ASSERT_EQ(numberVar.getType(), Type::Double);
      //numberVar /= 'a'; // convert to 0
      numberVar /= '3';
      EXPECT_DOUBLE_EQ(numberVar.getDoubleValue(), 36.166666666666664);
      ASSERT_EQ(numberVar.getType(), Type::Double);
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

