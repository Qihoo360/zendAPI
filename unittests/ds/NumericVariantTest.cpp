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
#include "zapi/ds/NumericVariant.h"
#include <iostream>
using zapi::ds::NumericVariant;

TEST(NumericVarintTest, testBaseFuncs)
{
   NumericVariant num1(12);
   NumericVariant num2(21);
   ASSERT_FALSE(num1 == num2);
   ASSERT_TRUE(num1 != num2);
   ASSERT_TRUE(num1 < num2);
   ASSERT_TRUE(num1 <= num2);
   ASSERT_FALSE(num1 > num2);
   ASSERT_FALSE(num1 >= num2);
   ASSERT_FALSE(num1 == 13);
   ASSERT_TRUE(num1 != 13);
   ASSERT_TRUE(num1 < 13);
   ASSERT_TRUE(num1 <= 13);
   ASSERT_FALSE(num1 > 13);
   ASSERT_FALSE(num1 >= 13);
   ASSERT_TRUE(12 != num2);
   ASSERT_TRUE(12 < num2);
   ASSERT_TRUE(12 <= num2);
   ASSERT_FALSE(12 > num2);
   ASSERT_FALSE(12 >= num2);
   
}

TEST(NumericVariantTest, testAddOps)
{
   NumericVariant num1(1);
   num1++;
   ASSERT_EQ(num1, 2);
   num1 += 3;
   ASSERT_EQ(num1, 5);
   NumericVariant num2(3);
   ASSERT_EQ(num1 + num2, 8);
   num1 += 2.2; // trucate
   ASSERT_EQ(num1, 7);
   num1 += 2.7;
   ASSERT_EQ(num1, 10);
   ASSERT_EQ(num1 + 1, 11);
   ASSERT_EQ(1 + num1, 11);
   ASSERT_EQ(1.0 + num1, 11.0);
   ASSERT_EQ(num1 + 1.0, 11.0);
}
