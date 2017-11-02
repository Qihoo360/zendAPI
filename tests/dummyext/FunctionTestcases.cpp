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

#include "FunctionTestcases.h"
#include "NativeFunctions.h"

namespace dummyext 
{

using zapi::lang::Namespace;
using zapi::lang::ValueArgument;
using zapi::lang::RefArgument;
using zapi::lang::VariadicArgument;

void register_function_testcases(Extension &extension)
{
   extension.registerFunction<decltype(&dummyext::show_something), &dummyext::show_something>("show_something");
   extension.registerFunction<decltype(&dummyext::get_name), &dummyext::get_name>("get_name");
   extension.registerFunction<decltype(&dummyext::print_name), &dummyext::print_name>
         ("print_name", {
             ValueArgument("name", zapi::lang::Type::String)
          });
   extension.registerFunction<decltype(&dummyext::print_name_and_age), &dummyext::print_name_and_age>
         ("print_name_and_age", {
             ValueArgument("name", zapi::lang::Type::String),
             ValueArgument("age", zapi::lang::Type::Long)
          });
   extension.registerFunction<decltype(&dummyext::add_two_number), &dummyext::add_two_number>
         ("add_two_number", {
             ValueArgument("num1", zapi::lang::Type::Long),
             ValueArgument("num2", zapi::lang::Type::Long)
          });
   extension.registerFunction<decltype(&dummyext::return_arg), &dummyext::return_arg>
         ("return_arg", {
             ValueArgument("number1"),
          });
   // for passby value and reference test
   extension.registerFunction<decltype(&dummyext::get_value_ref), &dummyext::get_value_ref>
         ("get_value_ref", {
             RefArgument("number", zapi::lang::Type::Numeric),
          });
   extension.registerFunction<decltype(&dummyext::passby_value), &dummyext::passby_value>
         ("passby_value", {
             ValueArgument("number", zapi::lang::Type::Numeric),
          });
   
   // test for default arguments
   extension.registerFunction<decltype(&dummyext::say_hello), &dummyext::say_hello>
         ("say_hello", {
             ValueArgument("name", zapi::lang::Type::String, false)
          });
   
   // register for namespace
   Namespace *zapi = extension.findNamespace("zapi");
   Namespace *io = zapi->findNamespace("io");
   
   zapi->registerFunction<decltype(&dummyext::get_name), &dummyext::get_name>("get_name");
   zapi->registerFunction<decltype(&dummyext::show_something), &dummyext::show_something>("show_something");
   
   io->registerFunction<decltype(&dummyext::calculate_sum), &dummyext::calculate_sum>
         ("calculate_sum", {
             VariadicArgument("numbers")
          });
   io->registerFunction<decltype(&dummyext::print_name), &dummyext::print_name>
         ("print_name", {
             ValueArgument("name", zapi::lang::Type::String)
          });
   // for test varidic
   io->registerFunction<decltype(&dummyext::print_sum), &dummyext::print_sum>
         ("print_sum", {
             VariadicArgument("numbers")
          });
   io->registerFunction<decltype(&dummyext::show_something), &dummyext::show_something>("show_something");
}

} // dummyext
