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
// Created by zzu_softboy on 27/07/2017.

#include "zapi/lang/Function.h"
#include "gtest/gtest.h"

using zapi::lang::Function;
using zapi::lang::Arguments;
using zapi::lang::Type;
using zapi::lang::ValueArgument;
using zapi::lang::RefArgument;

void dummy_func(struct _zend_execute_data *executeData, struct _zval_struct *returnValue)
{}

TEST(FunctionTest, testConstructor)
{
   {
//      Function func("zapi_version", dummy_func);
//      zend_function_entry funcEntry;
//      func.initialize(&funcEntry);
//      ASSERT_STREQ(funcEntry.fname, "zapi_version");
//      ASSERT_EQ(funcEntry.handler, &dummy_func);
//      ASSERT_EQ(funcEntry.num_args, 0);
//      ASSERT_EQ(funcEntry.flags, 0);
   }
   // add argument
   {
//      Arguments args{
//         ValueArgument("name", Type::String),
//         RefArgument("age", Type::Numeric, true)      
//      };
//      Function func("zapi_version", dummy_func, args);
//      zend_function_entry funcEntry;
//      func.initialize(&funcEntry);
//      ASSERT_STREQ(funcEntry.fname, "zapi_version");
//      ASSERT_EQ(funcEntry.handler, &dummy_func);
//      ASSERT_EQ(funcEntry.num_args, 2);
//      ASSERT_EQ(funcEntry.flags, 0);
//      const struct _zend_internal_arg_info arg1 = funcEntry.arg_info[1];
//      const struct _zend_internal_arg_info arg2 = funcEntry.arg_info[2];
//      ASSERT_STREQ(arg1.name, "name");
//      ASSERT_EQ(arg1.type_hint, static_cast<zend_uchar>(Type::String));
//      ASSERT_FALSE(arg1.pass_by_reference);
//      ASSERT_STREQ(arg2.name, "age");
//      ASSERT_EQ(arg2.type_hint, static_cast<zend_uchar>(Type::Numeric));
   }
}
