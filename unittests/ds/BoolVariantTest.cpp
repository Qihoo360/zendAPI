// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by softboy on 2017/08/09.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/BoolVariant.h"

using zapi::ds::BoolVariant;
using zapi::ds::Variant;
using zapi::lang::Type;

TEST(BoolVariantTest, testBoolVaraint)
{
   BoolVariant defaultVar;
   BoolVariant trueVar(true);
   BoolVariant falseVar(false);
   ASSERT_FALSE(defaultVar);
   ASSERT_TRUE(trueVar);
   ASSERT_FALSE(falseVar);
   ASSERT_EQ(falseVar, defaultVar);
   BoolVariant copyed(trueVar);
   ASSERT_TRUE(copyed);
   copyed = falseVar;
   ASSERT_FALSE(copyed);
   copyed = true;
   ASSERT_TRUE(copyed);
   copyed = false;
   ASSERT_FALSE(copyed);
   copyed = 1;
   ASSERT_TRUE(copyed);
   copyed = 0;
   ASSERT_FALSE(copyed);
   copyed = 0.0;
   ASSERT_FALSE(copyed);
   copyed = 1;
   ASSERT_TRUE(copyed);
   Variant baseVar(true);
   BoolVariant copyFromBaseVar(baseVar);
}

TEST(BoolVariantTest, testMoveConstuctorAndAssign)
{
   BoolVariant bool1(true);
   BoolVariant bool2(false);
   Variant bool3(true);
   Variant str("zapi"); // true
   BoolVariant bool4(std::move(bool1));
   ASSERT_TRUE(bool4);
   bool4 = bool2;
   ASSERT_FALSE(bool4);
   ASSERT_FALSE(bool2);
   bool4 = std::move(bool2);
   ASSERT_FALSE(bool4);
   bool4 = bool3;
   ASSERT_TRUE(bool4);
   ASSERT_TRUE(bool3);
   bool4 = std::move(bool3);
   ASSERT_TRUE(bool4);
   bool4 = str;
   ASSERT_TRUE(bool4);
   ASSERT_TRUE(nullptr != str.getZvalPtr());
   bool4 = std::move(str);
   ASSERT_TRUE(bool4);
   {
      // test ref
      Variant variant1(true);
      Variant variant2(variant1, true);
      BoolVariant boolVariant(variant2);
      ASSERT_EQ(boolVariant.getUnDerefType(), Type::True);
      BoolVariant boolVariant1(std::move(variant2));
      ASSERT_EQ(boolVariant1.getUnDerefType(), Type::True);
   }
   {
      // test ref
      Variant variant1(123);
      Variant variant2(variant1, true);
      BoolVariant boolVariant(variant2);
      ASSERT_EQ(boolVariant.getUnDerefType(), Type::True);
      BoolVariant boolVariant1(std::move(variant2));
      ASSERT_EQ(boolVariant1.getUnDerefType(), Type::True);
   }
}

TEST(BoolVariantTest, testMoveConstruct)
{
   // test move construct
   BoolVariant bool1(true);
   BoolVariant bool2(bool1, true);
   ASSERT_EQ(bool1.getUnDerefType(), Type::Reference);
   ASSERT_EQ(bool2.getUnDerefType(), Type::Reference);
   BoolVariant bool3(std::move(bool1));
   ASSERT_EQ(bool3.getUnDerefType(), Type::Reference);
   BoolVariant bool4(bool3);
   ASSERT_EQ(bool4.getUnDerefType(), Type::True);
}

TEST(BoolVariantTest, testRefConstruct)
{
   {
      BoolVariant bool1(true);
      ASSERT_EQ(bool1.getUnDerefType(), Type::True);
      BoolVariant bool2(bool1, false);
      ASSERT_EQ(bool2.getUnDerefType(), Type::True);
   }
   {
      BoolVariant bool1(true);
      ASSERT_EQ(bool1.getUnDerefType(), Type::True);
      BoolVariant bool2(bool1, true);
      ASSERT_EQ(bool2.getUnDerefType(), Type::Reference);
      ASSERT_EQ(bool2.getType(), Type::True);
      BoolVariant bool3 = bool2;
      ASSERT_EQ(bool3.getUnDerefType(), Type::True);
      ASSERT_EQ(bool3.getType(), Type::True);
      ASSERT_TRUE(bool1.toBool());
      ASSERT_TRUE(bool2.toBool());
      bool2 = false;
      ASSERT_FALSE(bool1.toBool());
      ASSERT_FALSE(bool2.toBool());
      ASSERT_TRUE(bool3.toBool());
      bool3 = bool2;
      ASSERT_FALSE(bool1.toBool());
      ASSERT_FALSE(bool2.toBool());
      ASSERT_FALSE(bool3.toBool());
      ASSERT_EQ(bool3.getType(), Type::False);
   }
   {
      zval var1;
      ZVAL_BOOL(&var1, true);
      BoolVariant bool1(var1, false);
      ASSERT_EQ(bool1.getUnDerefType(), Type::True);
      ASSERT_TRUE(Z_TYPE_P(&var1) == IS_TRUE);
      BoolVariant bool2(var1, true);
      ASSERT_EQ(bool2.getUnDerefType(), Type::Reference);
      ASSERT_TRUE(Z_TYPE_P(&var1) == IS_REFERENCE);
      zval_dtor(&var1);
      bool2 = false;
      ASSERT_FALSE(bool2.toBool());
      zval *rval = &var1;
      ZVAL_DEREF(rval);
      ASSERT_TRUE(Z_TYPE_P(rval) == IS_FALSE);
   }
}
