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
}
