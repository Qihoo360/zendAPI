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

#include "ClassTestcases.h"
#include "NativeClasses.h"

namespace dummyext
{

using zapi::lang::Modifier;
using zapi::lang::Constant;
using zapi::lang::ValueArgument;
using zapi::lang::VariadicArgument;
using zapi::lang::Namespace;

void register_class_testcases(Extension &extension)
{
   register_basic_classes(extension);
   register_construct_and_destruct_classes(extension);
   register_namespace_classes(extension);
}

void register_basic_classes(Extension &extension)
{
   zapi::lang::Class<Person> personClass("Person");
   personClass.registerConstant("UNICORNTEAM", "beijing unicornteam");
   personClass.registerConstant("MY_CONST", "MY_CONST_VALUE");
   personClass.registerConstant(Constant("PI", 3.1415926));
   personClass.registerConstant("HEADER_SIZE", 123);
   personClass.registerConstant("ALLOW_ACL", true);
   personClass.registerProperty("name", "zzu_softboy");
   personClass.registerProperty("staticProp", "beijing", Modifier::Public | Modifier::Static);
   personClass.registerMethod<decltype(&Person::showName), &Person::showName>("showName");
   personClass.registerMethod<decltype(&Person::print_sum), &Person::print_sum>
         ("print_sum",{
             VariadicArgument("numbers")
          });
   personClass.registerMethod<decltype(&Person::setAge), &Person::setAge>
         ("setAge", {
             ValueArgument("age", zapi::lang::Type::Long)
          });
   personClass.registerMethod<decltype(&Person::getAge), &Person::getAge>("getAge");
   personClass.registerMethod<decltype(&Person::getName), &Person::getName>("getName");
   personClass.registerMethod<decltype(&Person::addTwoNum), &Person::addTwoNum>
         ("addTwoNum", {
             ValueArgument("num1", zapi::lang::Type::Numeric),
             ValueArgument("num2", zapi::lang::Type::Numeric)
          });
   personClass.registerMethod<decltype(&Person::addSum), &Person::addSum>
         ("addSum",{
             VariadicArgument("numbers")
          });
   personClass.registerMethod<decltype(&Person::protectedMethod), &Person::protectedMethod>
         ("protectedMethod", Modifier::Protected);
   personClass.registerMethod<decltype(&Person::privateMethod), &Person::privateMethod>
         ("privateMethod", Modifier::Private);
   
   personClass.registerMethod<decltype(&Person::concatStr), &Person::concatStr>
         ("concatStr", {
             ValueArgument("lhs", zapi::lang::Type::String),
             ValueArgument("rhs", zapi::lang::Type::String)
          });
   personClass.registerMethod<decltype(&Person::staticShowName), &Person::staticShowName>("staticShowName");
   personClass.registerMethod<decltype(&Person::staticProtectedMethod), &Person::staticProtectedMethod>
         ("staticProtectedMethod", Modifier::Protected);
   personClass.registerMethod<decltype(&Person::staticPrivateMethod), &Person::staticPrivateMethod>
         ("staticPrivateMethod", Modifier::Private);
   personClass.registerMethod<decltype(&Person::makeNewPerson), &Person::makeNewPerson>("makeNewPerson");
   
   
   
   extension.registerClass(personClass);
}

void register_namespace_classes(Extension &extension)
{
   zapi::lang::Class<Address> addressClass("Address");
   Namespace *zapi = extension.findNamespace("zapi");
   zapi->registerClass(addressClass);
   zapi::lang::Class<EmptyClass> emptyCls("EmptyClass");
   zapi->registerClass(emptyCls);
}

void register_construct_and_destruct_classes(Extension &extension)
{
   zapi::lang::Class<ConstructAndDestruct> ConstructAndDestruct("ConstructAndDestruct");
   ConstructAndDestruct.registerMethod
         <decltype(&ConstructAndDestruct::__construct), &ConstructAndDestruct::__construct>("__construct");
   ConstructAndDestruct.registerMethod
         <decltype(&ConstructAndDestruct::__destruct), &ConstructAndDestruct::__construct>("__destruct");
   extension.registerClass(ConstructAndDestruct);
}


} // dummyext
