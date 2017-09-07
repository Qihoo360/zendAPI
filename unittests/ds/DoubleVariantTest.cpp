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
// Created by softboy on 2017/08/30.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/DoubleVariant.h"

using zapi::ds::NumericVariant;
using zapi::ds::DoubleVariant;
using zapi::ds::Variant;
using zapi::lang::Type;

TEST(DoubleVariantTest, testMoveConstruct)
{
   // test move construct
   DoubleVariant num1(123);
   DoubleVariant num2(num1, true);
   ASSERT_EQ(num1.getUnDerefType(), Type::Reference);
   ASSERT_EQ(num2.getUnDerefType(), Type::Reference);
   DoubleVariant num3(std::move(num1));
   ASSERT_EQ(num3.getUnDerefType(), Type::Reference);
   DoubleVariant num4(num3);
   ASSERT_EQ(num4.getUnDerefType(), Type::Double);
}

TEST(DoubleVariantTest, testRefConstruct)
{
   {
      DoubleVariant num1(123);
      DoubleVariant num2(num1, false);
      ASSERT_EQ(num1.getUnDerefType(), Type::Double);
      ASSERT_EQ(num2.getUnDerefType(), Type::Double);
      ASSERT_EQ(num1.toDouble(), 123);
      ASSERT_EQ(num2.toDouble(), 123);
   }
   {
      DoubleVariant num1(123);
      DoubleVariant num2(num1, true);
      ASSERT_EQ(num1.getUnDerefType(), Type::Reference);
      ASSERT_EQ(num2.getUnDerefType(), Type::Reference);
      ASSERT_EQ(num1.toDouble(), 123);
      ASSERT_EQ(num2.toDouble(), 123);
      DoubleVariant num3(num2, false);
      DoubleVariant num4(num1);
      ASSERT_EQ(num3.toDouble(), 123);
      ASSERT_EQ(num4.toDouble(), 123);
      num1 = 321;
      ASSERT_EQ(num1.toDouble(), 321);
      ASSERT_EQ(num2.toDouble(), 321);
      ASSERT_EQ(num3.toDouble(), 123);
      ASSERT_EQ(num4.toDouble(), 123);
      num3 = num1;
      ASSERT_EQ(num3.toDouble(), 321);
      ASSERT_EQ(num3.getUnDerefType(), Type::Double);
   }
   {
      zval numVar;
      ZVAL_DOUBLE(&numVar, 123);
      DoubleVariant num2(numVar, false);
      ASSERT_EQ(num2.getUnDerefType(), Type::Double);
      ASSERT_EQ(num2.toDouble(), 123);
      ASSERT_EQ(Z_TYPE(numVar), IS_DOUBLE);
   }
   {
      zval numVar;
      ZVAL_DOUBLE(&numVar, 123);
      DoubleVariant num1(numVar, true);
      ASSERT_EQ(num1.getUnDerefType(), Type::Reference);
      ASSERT_EQ(num1.getType(), Type::Double);
      ASSERT_EQ(num1.toDouble(), 123);
      zval *rval = &numVar;
      ZVAL_DEREF(rval);
      ASSERT_TRUE(Z_TYPE_P(rval) == IS_DOUBLE);
      num1 = 321;
      ASSERT_EQ(num1.toDouble(), 321);
      ASSERT_EQ(Z_DVAL_P(rval), 321);
      zval_dtor(&numVar);
   }
}

