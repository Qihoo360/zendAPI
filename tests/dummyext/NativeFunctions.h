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

#ifndef ZAPI_TEST_DUMMYEXT_NATIVE_FUNCTIONS_H
#define ZAPI_TEST_DUMMYEXT_NATIVE_FUNCTIONS_H

#include "zapi/ZendApi.h"

namespace dummyext 
{

using zapi::ds::StringVariant;
using zapi::ds::NumericVariant;

ZAPI_DECL_EXPORT Variant return_arg(Variant &value);
ZAPI_DECL_EXPORT void show_something();
ZAPI_DECL_EXPORT Variant get_name();
ZAPI_DECL_EXPORT void get_value_ref(NumericVariant &number);
ZAPI_DECL_EXPORT void passby_value(NumericVariant &number);
ZAPI_DECL_EXPORT void print_sum(NumericVariant argQuantity, ...);
ZAPI_DECL_EXPORT void print_name(const StringVariant &name);
ZAPI_DECL_EXPORT void print_name_and_age(const StringVariant &name, const NumericVariant &age);
ZAPI_DECL_EXPORT Variant calculate_sum(NumericVariant argQuantity, ...);
ZAPI_DECL_EXPORT Variant add_two_number(const NumericVariant &num1, const NumericVariant &num2);
ZAPI_DECL_EXPORT void say_hello(StringVariant &name);

}

#endif // ZAPI_TEST_DUMMYEXT_NATIVE_FUNCTIONS_H
