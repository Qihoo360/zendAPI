// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by softboy on 2017/08/29.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/Variant.h"

using zapi::ds::Variant;
using zapi::lang::Type;

TEST(VariantTest, testRefConstruct)
{
   zval var1;
   ZVAL_LONG(&var1, 123);
   ASSERT_EQ(Z_LVAL(var1), 123);
   Variant var(var1, true);
   ASSERT_EQ(Z_LVAL(var.getZval()), 123);
   var = 213;
   ASSERT_EQ(Z_LVAL(var.getZval()), 213);
   ASSERT_EQ(Z_LVAL_P(Z_REFVAL(var1)), 213);
   var = "zapi";
   ASSERT_STREQ(Z_STRVAL(var.getZval()), "zapi");
   ASSERT_STREQ(Z_STRVAL_P(Z_REFVAL(var1)), "zapi");
   var = Variant(2017);
   ASSERT_EQ(Z_LVAL(var.getZval()), 2017);
   ASSERT_EQ(Z_LVAL_P(Z_REFVAL(var1)), 2017);
   zval_dtor(&var1);
}

TEST(VariantTest, testCompareOperators)
{
   zval var1;
   ZVAL_LONG(&var1, 123);
   Variant variant1(var1, true);
   zval var2;
   ZVAL_LONG(&var2, 123);
   Variant variant2(var2, true);
   Variant variant3(123);
   ASSERT_TRUE(variant1 == variant2);
   ASSERT_TRUE(variant1 == variant3);
   zval_dtor(&var1);
   zval_dtor(&var2);
}

TEST(VariantTest, testRefVariantConstruct) 
{
   Variant variant1(123);
   ASSERT_EQ(variant1.getRefCount(), 0);
   ASSERT_FALSE(variant1.isReference());
   Variant variant2(variant1, true);
   ASSERT_EQ(variant1.getRefCount(), 2);
   ASSERT_EQ(variant2.getRefCount(), 2);
   ASSERT_EQ(variant1.getType(), Type::Numeric);
   ASSERT_EQ(variant2.getType(), Type::Numeric);
   ASSERT_EQ(variant1.getUnDerefType(), Type::Reference);
   ASSERT_EQ(variant2.getUnDerefType(), Type::Reference);
   ASSERT_TRUE(variant1.isReference());
   ASSERT_TRUE(variant2.isReference());
}

TEST(VariantTest, testMoveAssignOperator)
{
   {
      Variant variant1(123);
      Variant variant2(123);
      Variant variant3(213);
      ASSERT_EQ(Z_LVAL(variant1.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant2.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant3.getZval()), 213);
      variant2 = std::move(variant3);
      ASSERT_EQ(Z_LVAL(variant2.getZval()), 213);
      ASSERT_EQ(Z_LVAL(variant1.getZval()), 123);
   }
   {
      Variant variant1(123);
      Variant variant2(variant1, true);
      Variant variant3(213);
      ASSERT_EQ(Z_LVAL(variant1.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant2.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant3.getZval()), 213);
      variant2 = std::move(variant3);
      ASSERT_EQ(Z_LVAL(variant1.getZval()), 213);
      ASSERT_EQ(Z_LVAL(variant2.getZval()), 213);
   }
   {
      Variant variant1(123);
      Variant variant2(variant1, true);
      Variant variant3(213);
      ASSERT_EQ(Z_LVAL(variant1.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant2.getZval()), 123);
      ASSERT_EQ(Z_LVAL(variant3.getZval()), 213);
      variant3 = variant2;
      ASSERT_EQ(Z_LVAL(variant3.getZval()), 123);
      ASSERT_EQ(variant3.getUnDerefType(), Type::Numeric);
      variant3 = std::move(variant2);
      ASSERT_EQ(Z_LVAL(variant3.getZval()), 123);
      ASSERT_EQ(variant3.getUnDerefType(), Type::Numeric);
   }
}

TEST(VariantTest, testVariantMoveConstruct)
{
   Variant variant1(123);
   Variant variant2(variant1, true);
   Variant variant3(std::move(variant1));
   ASSERT_EQ(variant3.getUnDerefType(), Type::Reference);
   Variant variant4(variant3);
   ASSERT_EQ(variant4.getUnDerefType(), Type::Numeric);
}

TEST(VariantTest, testConstructor)
{
   Variant nullvar(nullptr);
   zval nullval = nullvar.getZval();
   ASSERT_TRUE(Z_TYPE(nullval) == IS_NULL);
   int16_t intValue = 12;
   Variant var(intValue);
   zval val = var.getZval();
   ASSERT_EQ(Z_LVAL(val), intValue);
   Variant var1(std::numeric_limits<int16_t>::min());
   ASSERT_EQ(Z_LVAL(var1.getZval()), std::numeric_limits<int16_t>::min());
   ASSERT_EQ(Z_TYPE(var1.getZval()), IS_LONG);
   Variant var2(std::numeric_limits<int16_t>::max());
   ASSERT_EQ(Z_LVAL(var2.getZval()), std::numeric_limits<int16_t>::max());
   ASSERT_EQ(Z_TYPE(var2.getZval()), IS_LONG);
   Variant var3(std::numeric_limits<int32_t>::min());
   ASSERT_EQ(Z_LVAL(var3.getZval()), std::numeric_limits<int32_t>::min());
   Variant var4(std::numeric_limits<int32_t>::max());
   ASSERT_EQ(Z_LVAL(var4.getZval()), std::numeric_limits<int32_t>::max());
   ASSERT_EQ(Z_TYPE(var4.getZval()), IS_LONG);
   Variant var5(std::numeric_limits<int64_t>::min());
   ASSERT_EQ(Z_LVAL(var5.getZval()), std::numeric_limits<int64_t>::min());
   ASSERT_EQ(Z_TYPE(var5.getZval()), IS_LONG);
   Variant var6(std::numeric_limits<int64_t>::max());
   ASSERT_EQ(Z_LVAL(var6.getZval()), std::numeric_limits<int64_t>::max());
   ASSERT_EQ(Z_TYPE(var6.getZval()), IS_LONG);
   // test boolean
   Variant trueVariant(true);
   ASSERT_EQ(Z_TYPE(trueVariant.getZval()), IS_TRUE);
   Variant falseVariant(false);
   ASSERT_EQ(Z_TYPE(falseVariant.getZval()), IS_FALSE);
   // char constructor test
   Variant charVariant('a');
   char *retCharArr = Z_STRVAL(charVariant.getZval());
   ASSERT_EQ(retCharArr[0], 'a');
   ASSERT_EQ(Z_STRLEN(charVariant.getZval()), 1);
   // test constructor(const std::string &value)
   std::string zapiStr("zapi");
   Variant zapiStrVar(zapiStr);
   char *zapiStrPtr = Z_STRVAL(zapiStrVar.getZval());
   ASSERT_EQ(Z_STRLEN(zapiStrVar.getZval()), 4);
   ASSERT_EQ(std::strncmp(zapiStrPtr, "zapi", 4), 0);
   ASSERT_EQ(Z_TYPE(zapiStrVar.getZval()), IS_STRING);
   // test constructor(const char *value, int length = -1)
   Variant sizeStr1("hello zapi!");
   ASSERT_EQ(Z_STRLEN(sizeStr1.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL(sizeStr1.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE(sizeStr1.getZval()), IS_STRING);
   Variant sizeStr2("hello zapi!", 5);
   ASSERT_EQ(Z_STRLEN(sizeStr2.getZval()), 5);
   ASSERT_EQ(std::strncmp(Z_STRVAL(sizeStr2.getZval()), "hello", 5), 0);
   ASSERT_EQ(Z_TYPE(sizeStr2.getZval()), IS_STRING);
   Variant sizeStr3("hello zapi!", 20);
   ASSERT_EQ(Z_STRLEN(sizeStr3.getZval()), 20);
   ASSERT_EQ(std::strncmp(Z_STRVAL(sizeStr3.getZval()), "hello zapi!\0\0\0\0\0\0\0\0\0", 20), 0);
   ASSERT_EQ(Z_TYPE(sizeStr3.getZval()), IS_STRING);
   Variant sizeStr4("hello zapi!");
   ASSERT_EQ(Z_STRLEN(sizeStr4.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL(sizeStr4.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE(sizeStr4.getZval()), IS_STRING);
   //   test constructor(double value)
   double dvalue = 0.12;
   Variant doubleVar(dvalue);
   ASSERT_EQ(Z_TYPE(doubleVar.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL(doubleVar.getZval()), 0.12);
   double dvalue1 = -0.12;
   Variant doubleVar1(dvalue1);
   ASSERT_EQ(Z_TYPE(doubleVar1.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL(doubleVar1.getZval()), -0.12);
   // test construct from other _zval_struct *
   zval rawVar;
   ZVAL_LONG(&rawVar, 2017);
   Variant constructFromRaw(&rawVar);
   ASSERT_EQ(Z_LVAL(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_TYPE(constructFromRaw.getZval()), IS_LONG);
   ZVAL_LONG(&rawVar, 2018);
   ASSERT_EQ(Z_LVAL(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_LVAL_P(&rawVar), 2018);
   zval rawVar1;
   ZVAL_LONG(&rawVar1, 2017);
   Variant constructFromRawRef(&rawVar1, true);
   std::shared_ptr<zval> rawVal1Ptr(&rawVar1, [](void *rawPtr){
      zval *ptr = static_cast<zval *>(rawPtr);
      if (Z_ISREF_P(ptr)) {
         zval_ptr_dtor(ptr);
      }
   });
   ASSERT_EQ(Z_TYPE_P(Z_REFVAL(constructFromRawRef.getUnDerefZval())), IS_LONG);
   ASSERT_EQ(Z_LVAL_P(Z_REFVAL(constructFromRawRef.getUnDerefZval())), 2017);
   // test copy constructor
   {
      Variant orig(1234);
      Variant me(orig);
      ASSERT_EQ(Z_TYPE(me.getZval()), Z_TYPE(orig.getZval()));
      ASSERT_EQ(Z_LVAL(me.getZval()), Z_LVAL(orig.getZval()));
      Variant origStr("zapi", 4);
      Variant meStr(origStr);
      ASSERT_EQ(Z_TYPE(meStr.getZval()), Z_TYPE(origStr.getZval()));
      ASSERT_EQ(Z_LVAL(meStr.getZval()), Z_LVAL(origStr.getZval()));
   }
   {
      // test move constructor 
      Variant orig("zapi", 4);
      Variant me(std::move(orig));
      ASSERT_EQ(Z_TYPE(me.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL(me.getZval()), "zapi", 4), 0);
   }
}

TEST(VariantTest, testAssignOperator)
{
   {
      Variant orig(2018);
      Variant me("hello", 5);
      me = orig;
      ASSERT_EQ(Z_TYPE(orig.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(me.getZval()), 2018);
   }
   {
      // test number assign operators
      Variant var(1);
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), 1);
      var = static_cast<int16_t>(123);
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), 123);
      var = std::numeric_limits<int16_t>::min();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int16_t>::min());
      var = std::numeric_limits<int16_t>::max();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int16_t>::max());
      var = static_cast<int32_t>(1232);
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), 1232);
      var = std::numeric_limits<int32_t>::min();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int32_t>::min());
      var = std::numeric_limits<int32_t>::max();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int32_t>::max());
      var = static_cast<int64_t>(2018);
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), 2018);
      var = std::numeric_limits<int64_t>::min();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int64_t>::min());
      var = std::numeric_limits<int64_t>::max();
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL(var.getZval()), std::numeric_limits<int64_t>::max());
      // test double assign operators
      var = 0.12;
      ASSERT_EQ(Z_TYPE(var.getZval()), IS_DOUBLE);
      ASSERT_EQ(Z_DVAL(var.getZval()), 0.12);
   }
   {
      // test boolean assign operators
      Variant booleanVar;
      ASSERT_EQ(Z_TYPE(booleanVar.getZval()), IS_NULL);
      booleanVar = true;
      ASSERT_EQ(Z_TYPE(booleanVar.getZval()), IS_TRUE);
      booleanVar = false;
      ASSERT_EQ(Z_TYPE(booleanVar.getZval()), IS_FALSE);
   }
   {
      // string assign operators
      Variant strVar;
      ASSERT_EQ(Z_TYPE(strVar.getZval()), IS_NULL);
      strVar = std::string("zapi");
      ASSERT_EQ(Z_TYPE(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL(strVar.getZval()), "zapi", 4), 0);
      strVar = 'a';
      ASSERT_EQ(Z_TYPE(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL(strVar.getZval()), "a", 1), 0);
      strVar = "hello zapi";
      ASSERT_EQ(Z_TYPE(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL(strVar.getZval()), "hello zapi", 10), 0);
   }
}

TEST(VariantTest, testGetType)
{
   Variant var;
   ASSERT_EQ(var.getType(), Type::Null);
   ASSERT_TRUE(var.isNull());
   ASSERT_TRUE(var.isScalar());
   var = 1;
   ASSERT_EQ(var.getType(), Type::Long);
   ASSERT_TRUE(var.isLong());
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
   ASSERT_TRUE(var.isBool());
   var = false;
   ASSERT_EQ(var.getType(), Type::False);
   ASSERT_TRUE(var.isScalar());
   ASSERT_TRUE(var.isBool());
}
