#include "ExtFuncs.h"
#include "ClassDef.h"
#include <type_traits>
#include <iostream>

namespace dummyext
{

using zapi::ds::Variant;
using zapi::ds::StringVariant;
using zapi::ds::NumericVariant;
using zapi::lang::Parameters;
using zapi::lang::Arguments;
using zapi::lang::ValueArgument;
using zapi::lang::RefArgument;
using zapi::lang::VariadicArgument;
using zapi::lang::Constant;
using zapi::lang::Type;
using zapi::ds::ArrayVariant;
using zapi::lang::Constant;
using zapi::lang::Interface;
using zapi::lang::Modifier;
using zapi::lang::Namespace;
using zapi::ds::StringVariant;
using zapi::lang::Extension;

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
struct S {
   double operator()(char, int&);
   float operator()(int) { return 1.0;}
};

template<class T>
typename std::result_of<T(int)>::type f(T& t)
{
   std::cout << "overload of f for callable T\n";
   return t(0);
}

template<class T, class U>
int f(U u)
{
   std::cout << "overload of f for non-callable T\n";
   return u;
}

void register_const(Extension &extension)
{
   extension.registerConstant(Constant("MY_CONST", 12333));
   extension.registerConstant(Constant("PI", 3.14));
   Constant nameConst("ZAPI_NAME", "zapi");
   extension.registerConstant(nameConst);
   extension.registerConstant(Constant("ZAPI_VERSION", "v0.0.1"));
   extension.registerConstant(Constant("QIHOO", "beijing qihoo"));
   // php 7.0 support const array
}

void register_funcs(Extension &extension)
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
}

void register_ns_io(Namespace &io)
{
   io.registerFunction<decltype(&dummyext::calculate_sum), &dummyext::calculate_sum>
         ("calculate_sum", {
             VariadicArgument("numbers")
          });
   io.registerFunction<decltype(&dummyext::print_name), &dummyext::print_name>
         ("print_name", {
             ValueArgument("name", zapi::lang::Type::String)
          });
   // for test varidic
   io.registerFunction<decltype(&dummyext::print_sum), &dummyext::print_sum>
         ("print_sum", {
             VariadicArgument("numbers")
          });
   io.registerFunction<decltype(&dummyext::show_something), &dummyext::show_something>("show_something");
   io.registerConstant(Constant("IO_TYPE", "ASYNC"));
   io.registerConstant(Constant("NATIVE_STREAM", true));
}

void register_ns_zapi(Namespace &zapi)
{
   zapi.registerFunction<decltype(&dummyext::get_name), &dummyext::get_name>("get_name");
   zapi.registerConstant(Constant("SYS_VERSION", "0.1.1-alpha"));
   zapi.registerFunction<decltype(&dummyext::show_something), &dummyext::show_something>("show_something");
   zapi::lang::Class<EmptyClass> emptyCls("EmptyClass");
   zapi.registerClass(emptyCls);
}

void register_construct_and_destruct(Extension &extension)
{
   zapi::lang::Class<ConstructAndDestruct> ConstructAndDestruct("ConstructAndDestruct");
   ConstructAndDestruct.registerMethod
         <decltype(&ConstructAndDestruct::__construct), &ConstructAndDestruct::__construct>("__construct");
   ConstructAndDestruct.registerMethod
         <decltype(&ConstructAndDestruct::__destruct), &ConstructAndDestruct::__construct>("__destruct");
   extension.registerClass(ConstructAndDestruct);
}

void register_props_test_cls(Extension &extension)
{
   zapi::lang::Class<PropsTestClass> propsTestClass("PropsTestClass");
   propsTestClass.registerProperty("nullProp", nullptr);
   propsTestClass.registerProperty("trueProp", true);
   propsTestClass.registerProperty("falseProp", false);
   propsTestClass.registerProperty("numProp", 2017);
   propsTestClass.registerProperty("doubleProp", 3.1415);
   propsTestClass.registerProperty("strProp", "zapi");
   propsTestClass.registerProperty("str1Prop", std::string("zapi"));
   
   propsTestClass.registerProperty("staticNullProp", nullptr, Modifier::Static);
   propsTestClass.registerProperty("staticTrueProp", true, Modifier::Static);
   propsTestClass.registerProperty("staticFalseProp", false, Modifier::Static);
   propsTestClass.registerProperty("staticNumProp", 2012, Modifier::Static);
   propsTestClass.registerProperty("staticDoubleProp", 3.1415, Modifier::Static);
   propsTestClass.registerProperty("staticStrProp", "static zapi", Modifier::Static);
   propsTestClass.registerProperty("staticStr1Prop", std::string("static zapi"), Modifier::Static);
   
   propsTestClass.registerProperty("MATH_PI", 3.14, Modifier::Const);
   
   propsTestClass.registerProperty("name", &PropsTestClass::getName, &PropsTestClass::setName);
   propsTestClass.registerProperty("age", &PropsTestClass::getAge, &PropsTestClass::setAge);
   
   extension.registerClass(propsTestClass);
}

void register_magic_method_cls(Extension &extension)
{
   zapi::lang::Class<NonMagicMethodClass> nonMagicMethodClass("NonMagicMethodClass");
   zapi::lang::Class<MagicMethodClass> magicMethodClass("MagicMethodClass");
   extension.registerClass(nonMagicMethodClass);
   extension.registerClass(magicMethodClass);
}

void register_object_variant_cls(Extension &extension)
{
   zapi::lang::Class<ObjectVariantClass> objectVariantClass("ObjectVariantClass");
   objectVariantClass.registerMethod<decltype(&ObjectVariantClass::forwardInvoke), &ObjectVariantClass::forwardInvoke>("forwardInvoke");
   extension.registerClass(objectVariantClass);
}

void register_inherit_cls(Extension &extension)
{
   zapi::lang::Class<A> a("A");
   zapi::lang::Class<B> b("B");
   zapi::lang::Class<C> c("C");
   b.registerBaseClass(a);
   c.registerBaseClass(b);
   extension.registerClass(a);
   extension.registerClass(b);
   extension.registerClass(c);
}

void register_cls(Extension &extension)
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
   
   //   Interface infoInterface("InfoProvider");
   //   //   infoInterface.registerMethod("getName");
   //   personClass.registerInterface(infoInterface);
   //   extension.registerInterface(infoInterface);
   extension.registerClass(personClass);
   register_construct_and_destruct(extension);
   register_props_test_cls(extension);
   register_magic_method_cls(extension);
   register_object_variant_cls(extension);
   register_inherit_cls(extension);
}



} // dummyext
