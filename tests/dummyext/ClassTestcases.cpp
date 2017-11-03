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
using zapi::lang::RefArgument;
using zapi::lang::Namespace;
using zapi::lang::Interface;
using zapi::lang::Type;

void register_class_testcases(Extension &extension)
{
   register_basic_classes(extension);
   register_construct_and_destruct_classes(extension);
   register_namespace_classes(extension);
   register_inherit_test_classes(extension);
   register_iterator_test_classes(extension);
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

void register_inherit_test_classes(Extension &extension)
{
   Interface interfaceA("InterfaceA");
   Interface interfaceB("InterfaceB");
   Interface interfaceC("InterfaceC");
   interfaceA.registerMethod("methodOfA");
   interfaceA.registerMethod("protectedMethodOfA", Modifier::Protected);
   interfaceA.registerMethod("privateMethodOfA", Modifier::Private);
   interfaceB.registerMethod("methodOfB");
   interfaceB.registerMethod("protectedMethodOfB", Modifier::Protected);
   interfaceB.registerMethod("privateMethodOfB", Modifier::Private);
   interfaceC.registerMethod("methodOfC");
   interfaceC.registerMethod("protectedMethodOfC", Modifier::Protected);
   interfaceC.registerMethod("privateMethodOfC", Modifier::Private);
   
   interfaceC.registerBaseInterface(interfaceB);
   interfaceB.registerBaseInterface(interfaceA);
   extension.registerInterface(interfaceA);
   extension.registerInterface(interfaceB);
   extension.registerInterface(interfaceC);
   
   zapi::lang::Class<A> a("A");
   zapi::lang::Class<B> b("B");
   zapi::lang::Class<C> c("C");
   a.registerMethod<decltype(&A::printInfo), &A::printInfo>("printInfo");
   a.registerMethod<decltype(&A::changeNameByRef), &A::changeNameByRef>
         ("changeNameByRef", {
             RefArgument("name", Type::String)
          });
   a.registerMethod<decltype(&A::privateAMethod), &A::privateAMethod>("privateAMethod", Modifier::Private);
   a.registerMethod<decltype(&A::protectedAMethod), &A::protectedAMethod>("protectedAMethod", Modifier::Protected);
   a.registerProperty("name", "zapi");
   a.registerProperty("protectedName", "protected zapi", Modifier::Protected);
   a.registerProperty("privateName", "private zapi", Modifier::Private);
   b.registerMethod<decltype(&B::privateBMethod), &B::privateBMethod>("privateBMethod", Modifier::Private);
   b.registerMethod<decltype(&B::protectedBMethod), &B::protectedBMethod>("protectedBMethod", Modifier::Protected);
   b.registerMethod<decltype(&B::printInfo), &B::printInfo>("printInfo");
   b.registerMethod<decltype(&B::showSomething), &B::showSomething>("showSomething");
   b.registerMethod<decltype(&B::calculateSumByRef), &B::calculateSumByRef>
         ("calculateSumByRef", {
             RefArgument("result", Type::Long),
             VariadicArgument("numbers", Type::Long)
          });
   b.registerMethod<decltype(&B::addTwoNumber), &B::addTwoNumber>
         ("addTwoNumber", {
             ValueArgument("lhs"),
             ValueArgument("rhs")
          });
   b.registerProperty("propsFromB", "unicornteam", Modifier::Protected);
   c.registerMethod<decltype(&C::printInfo), &C::printInfo>("printInfo");
   c.registerMethod<decltype(&C::testCallParentPassRefArg), &C::testCallParentPassRefArg>("testCallParentPassRefArg");
   c.registerMethod<decltype(&C::testCallParentWithReturn), &C::testCallParentWithReturn>("testCallParentWithReturn");
   c.registerMethod<decltype(&C::testGetObjectVaraintPtr), &C::testGetObjectVaraintPtr>("testGetObjectVaraintPtr");
   c.registerMethod<decltype(&C::privateCMethod), &C::privateCMethod>("privateCMethod", Modifier::Private);
   c.registerMethod<decltype(&C::protectedCMethod), &C::protectedCMethod>("protectedCMethod", Modifier::Protected);
   c.registerMethod<decltype(&C::methodOfA), &C::methodOfA>("methodOfA", Modifier::Public);
   c.registerMethod<decltype(&C::protectedMethodOfA), &C::protectedMethodOfA>("protectedMethodOfA", Modifier::Public);
   c.registerMethod<decltype(&C::privateMethodOfA), &C::privateMethodOfA>("privateMethodOfA", Modifier::Public);
   c.registerProperty("address", "beijing", Modifier::Private);
   b.registerBaseClass(a);
   c.registerBaseClass(b);
   c.registerInterface(interfaceA);
   extension.registerClass(a);
   extension.registerClass(b);
   extension.registerClass(c);
}

void register_iterator_test_classes(Extension &extension)
{
   zapi::lang::Class<IterateTestClass> iterateTestClass("IterateTestClass");
   extension.registerClass(iterateTestClass);
}

} // dummyext
