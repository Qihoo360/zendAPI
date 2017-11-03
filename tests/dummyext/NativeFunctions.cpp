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
// Created by softboy on 2017/11/02.

#include "NativeFunctions.h"

namespace dummyext 
{

using zapi::lang::Variant;

void show_something()
{
   zapi::out << "hello world, zapi" << std::flush;
}

void get_value_ref(NumericVariant &number)
{
   number = 321;
}

void passby_value(NumericVariant &number)
{
   // have no effect
   number = 321;
}

Variant get_name()
{
   return "zzu_softboy";
}

void print_name(const StringVariant &name)
{
   zapi::out << name << std::flush;
}

void print_sum(NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   zapi::out << result << std::flush;
}

Variant calculate_sum(NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   return result;
}

void print_name_and_age(const StringVariant &name, const NumericVariant &age)
{
   zapi::out << "name: " << name << " age: " << age << std::flush;
}

Variant add_two_number(const NumericVariant &num1, const NumericVariant &num2)
{
   return num1 + num2;
}

void say_hello(StringVariant &name)
{
   if (name.getCapacity() == 0) {
      name = "zapi";
   }
   zapi::out << "hello, " << name << std::endl;
}

Variant return_arg(Variant &value)
{
   return value;
}

// for test closure
Variant print_something()
{
   zapi::out << "print_something called" << std::endl;
   return "print_some";
}

Variant have_ret_and_have_arg(Parameters &params)
{
   zapi::out << "have_ret_and_have_arg called" << std::endl;
   if (params.empty()) {
      return "have_ret_and_have_arg";
   } else {
      return params.at(0);
   }
}

} // dummyext
