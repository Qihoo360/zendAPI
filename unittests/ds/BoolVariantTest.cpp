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
// Created by softboy on 2017/06/13.

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
