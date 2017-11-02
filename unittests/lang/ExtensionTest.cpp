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
// Created by zzu_softboy on 2017/11/01.

#include "php/sapi/embed/php_embed.h"
#include "zapi/lang/Extension.h"
#include "zapi/lang/Namespace.h"
#include "gtest/gtest.h"

using zapi::lang::Extension;
using zapi::lang::Namespace;

TEST(ExtensionTest, testFindNamespace)
{
   Extension ext("dummyext", "1.0");
   ext.registerNamespace(Namespace("zapi"));
   ext.registerNamespace(Namespace("php"));
   ASSERT_EQ(ext.getNamespaceQuantity(), 2);
   Namespace *result = nullptr;
   result = ext.findNamespace("notexist");
   ASSERT_EQ(result, nullptr);
   result = ext.findNamespace("zapi");
   ASSERT_NE(result, nullptr);
   ASSERT_EQ(result->getName(), "zapi");
   result = ext.findNamespace("php");
   ASSERT_NE(result, nullptr);
   ASSERT_EQ(result->getName(), "php");
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
