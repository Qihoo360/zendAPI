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
// Created by softboy on 2017/11/01.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/utils/InternalFuncs.h"
#include <string>
#include <list>

using zapi::internal::parse_namespaces;

TEST(UtilsInternalTest, testParseNamespace)
{
   std::list<std::string> parts;
   std::string ns("::::");
   bool parseStatus = true;
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_FALSE(parseStatus);
   ns = "zapi::";
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_FALSE(parseStatus);
   ns = "zapi:kernel";
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_FALSE(parseStatus);
   ns = "zapi::kernel:io";
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_FALSE(parseStatus);
   ns = "zapi";
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_TRUE(parseStatus);
   ASSERT_EQ(parts, std::list<std::string>{"zapi"});
   parts.clear();
   ns = "zapi::kernel::io";
   parseStatus = parse_namespaces(ns, parts);
   ASSERT_TRUE(parseStatus);
   ASSERT_EQ(parts, std::list<std::string>({"zapi", "kernel", "io"}));
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
