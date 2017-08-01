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
// Created by zzu_softboy on 2017/08/01.

#include "gtest/gtest.h"
#include "zapi/lang/Type.h"

using zapi::lang::Modifier;

TEST(TypeTest, testModifierOperator)
{
   {
      Modifier modifier = Modifier::Abstract;
      ASSERT_TRUE(modifier == Modifier::Abstract);
      modifier |= Modifier::Public;
      ASSERT_TRUE(modifier == 0x02 | 0x100);
   }
   ASSERT_EQ(Modifier::Abstract | Modifier::Public, (0x02 | 0x100));
   ASSERT_EQ(Modifier::MethodModifiers, Modifier::Final | Modifier::Public | Modifier::Protected | Modifier::Private | Modifier::Static);
   ASSERT_EQ(Modifier::PropertyModifiers, Modifier::Final | Modifier::Public | Modifier::Protected | Modifier::Private | Modifier::Const | Modifier::Static);
   {
      Modifier modifier = Modifier::Public;
      ASSERT_TRUE((modifier & Modifier::Public) == Modifier::Public);
      modifier |= Modifier::Const;
      ASSERT_TRUE((modifier & Modifier::Const) == Modifier::Const);
      ASSERT_FALSE((modifier & Modifier::Const) == Modifier::Protected);
      modifier &= Modifier::Const;
      ASSERT_TRUE(modifier == Modifier::Const);
      ASSERT_TRUE((Modifier::MethodModifiers & Modifier::Final) == Modifier::Final);
      ASSERT_TRUE((Modifier::MethodModifiers & Modifier::Public) == Modifier::Public);
      ASSERT_TRUE((Modifier::MethodModifiers & Modifier::Const) == Modifier::Const);
   }
}