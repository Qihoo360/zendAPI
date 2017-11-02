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
// Created by zzu_softboy on 2017/11/02.

#include "php/sapi/embed/php_embed.h"
#include "zapi/lang/Namespace.h"
#include "zapi/lang/Class.h"
#include "zapi/lang/StdClass.h"
#include "zapi/vm/AbstractClass.h"
#include "gtest/gtest.h"

using zapi::lang::Namespace;
using zapi::lang::Class;
using zapi::lang::StdClass;
using zapi::vm::AbstractClass;

class ClassA : public StdClass
{};

class ClassB : public StdClass
{};

TEST(NamespaceTest, testFindNamespace)
{
   Namespace zapi("zapi");
   Namespace kernel("kernel");
   Namespace net("net");
   zapi.registerNamespace(kernel);
   zapi.registerNamespace(net);
   Namespace *result = nullptr;
   result = zapi.findNamespace("NotExistNamespace");
   ASSERT_EQ(result, nullptr);
   result = zapi.findNamespace("kernel");
   ASSERT_NE(result, nullptr);
   ASSERT_EQ(result->getName(), "kernel");
   result = nullptr;
   result = zapi.findNamespace("net");
   ASSERT_NE(result, nullptr);
   ASSERT_EQ(result->getName(), "net");
}

TEST(NamespaceTest, testFindClass)
{
   Namespace zapi("zapi");
   Class<ClassA> classA("ClassA");
   Class<ClassB> classB("ClassB");
   zapi.registerClass(classA);
   zapi.registerClass(classB);
   AbstractClass *resultCls = nullptr;
   resultCls = zapi.findClass("NotExistClass");
   ASSERT_EQ(resultCls, nullptr);
   resultCls = zapi.findClass("ClassA");
   ASSERT_NE(resultCls, nullptr);
   ASSERT_EQ(resultCls->getClassName(), "ClassA");
   resultCls = nullptr;
   resultCls = zapi.findClass("ClassB");
   ASSERT_NE(resultCls, nullptr);
   ASSERT_EQ(resultCls->getClassName(), "ClassB");
}
