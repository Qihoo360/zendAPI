#include "ClassDef.h"
#include <string>
#include "zapi/ds/ObjectVariant.h"
#include "zapi/ds/CallableVariant.h"
#include "zapi/lang/Parameters.h"

using zapi::ds::ObjectVariant;
using zapi::ds::StringVariant;
using zapi::ds::CallableVariant;
using zapi::lang::Type;
using zapi::lang::Parameters;
using zapi::protocol::AbstractIterator;


Person::Person()
   : m_name("zzu_softboy"),
     m_age(0)
{}

void Person::showName()
{
   zapi::out << "my name is zapi" << std::endl;
}

void Person::setAge(const NumericVariant &age)
{
   m_age = age.toLong();
}

int Person::getAge()
{
   return m_age;
}

Variant Person::getName()
{
   return m_name;
}

void Person::staticShowName()
{
   zapi::out << "static my name is zapi" << std::endl;
}

StringVariant Person::concatStr(const StringVariant &lhs, const StringVariant &rhs)
{
   return lhs + rhs;
}

void Person::staticProtectedMethod()
{
   
}

void Person::staticPrivateMethod()
{
   
}

void Person::makeNewPerson()
{
   ObjectVariant obj("Person", std::make_shared<Person>());
}

void Person::print_sum(zapi::ds::NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   zapi::out << "the sum is " << result << std::endl;
}

int Person::addSum(NumericVariant argQuantity, ...)
{
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   return result.toLong();
}

void Person::protectedMethod()
{
   
}

void Person::privateMethod()
{
   
}

int Person::addTwoNum(const NumericVariant &num1, const NumericVariant &num2)
{
   return num1 + num2;
}

Address::Address()
   : address("beijing")
{}

void ConstructAndDestruct::__construct()
{
   zapi::out << "constructor been invoked" << std::endl;
}

void ConstructAndDestruct::__destruct()
{
   zapi::out << "destructor been invoked" << std::endl;;
}

void PropsTestClass::setAge(const Variant &value)
{
   NumericVariant age(value);
   age += 1;
   m_age = age.toLong();
}

Variant PropsTestClass::getAge()
{
   return m_age;
}

void PropsTestClass::setName(const Variant &name)
{
   StringVariant str(name);
   str.prepend("zapi:");
   m_name = str.toString();
}

Variant PropsTestClass::getName()
{
   return m_name;
}

Variant MagicMethodClass::__call(const std::string &method, Parameters &params) const
{
   zapi::out << "MagicMethodClass::__call is called" << std::endl;
   if (method == "calculateSum") {
      NumericVariant sum;
      for (int i = 0; i < params.size(); i++) {
         sum += NumericVariant(params.at(i));
      }
      return sum;
   } else {
      return nullptr;
   }
}

Variant MagicMethodClass::__invoke(Parameters &params) const
{
   zapi::out << "MagicMethodClass::__invoke is called" << std::endl;
   NumericVariant sum;
   for (int i = 0; i < params.size(); i++) {
      sum += NumericVariant(params.at(i));
   }
   return sum;
}

void MagicMethodClass::__set(const std::string &key, const Variant &value)
{
   zapi::out << "MagicMethodClass::__set is called" << std::endl;
   if (key == "address") {
      m_address = StringVariant(value).toString();
      m_teamAddressUnset = false;
   } else if (key == "length") {
      m_length = NumericVariant(value).toLong();
   }
}

Variant MagicMethodClass::__get(const std::string &key) const
{
   zapi::out << "MagicMethodClass::__get is called" << std::endl;
   if (key == "prop1") {
      return "zapi";
   } else if(key == "teamName" && !m_teamNameUnset) {
      return "unicornteam";
   } else if (key == "address" && !m_teamAddressUnset) {
      return m_address;
   } else if (key == "length") {
      return m_length;
   }
   return nullptr;
}

bool MagicMethodClass::__isset(const std::string &key) const
{
   zapi::out << "MagicMethodClass::__isset is called" << std::endl;
   if (key == "prop1") {
      return true;
   } else if (key == "teamName" && !m_teamNameUnset) {
      return true;
   } else if (key == "address" && !m_teamAddressUnset) {
      return true;
   } else if (key == "length") {
      return true;
   }
   return false;
}

void MagicMethodClass::__unset(const std::string &key)
{
   zapi::out << "MagicMethodClass::__unset is called" << std::endl;
   if (key == "teamName") {
      m_teamNameUnset = true;
   } else if (key == "address") {
      m_teamAddressUnset = true;
   }
}

Variant MagicMethodClass::__toString() const
{
   zapi::out << "MagicMethodClass::__toString is called" << std::endl;
   return "hello, zapi";
}

Variant MagicMethodClass::__toInteger() const
{
   zapi::out << "MagicMethodClass::__toInteger is called" << std::endl;
   return 2017;
}

Variant MagicMethodClass::__toDouble() const
{
   zapi::out << "MagicMethodClass::__toDouble is called" << std::endl;
   return 3.14;
}

Variant MagicMethodClass::__toBool() const
{
   zapi::out << "MagicMethodClass::__toBool is called" << std::endl;
   return true;
}

ArrayVariant MagicMethodClass::__debugInfo() const
{
   ArrayVariant info;
   info.insert("name", "zapi");
   info.insert("address", "beijing");
   return info;
}

int MagicMethodClass::__compare(const MagicMethodClass &object) const
{
   zapi::out << "MagicMethodClass::__compare is called" << std::endl;
   if (m_length < object.m_length) {
      return -1;
   } else if (m_length == object.m_length) {
      return 0;
   } else {
      return 1;
   }
}

void MagicMethodClass::__clone()
{
   zapi::out << "MagicMethodClass::__clone is called" << std::endl;
}

Variant MagicMethodClass::__callStatic(const std::string &method, Parameters &params)
{
   zapi::out << "MagicMethodClass::__callStatic is called" << std::endl;
   if (method == "staticCalculateSum") {
      NumericVariant sum;
      for (int i = 0; i < params.size(); i++) {
         sum += NumericVariant(params.at(i));
      }
      return sum;
   } else {
      StringVariant str("hello, ");
      str += StringVariant(params.at(0));
      return str;
   }
}

std::string MagicMethodClass::serialize()
{
   zapi::out << "MagicMethodClass::serialize is called" << std::endl;
   return "serialize data";
}

void MagicMethodClass::unserialize(const char *input, size_t size)
{
   zapi::out << "MagicMethodClass::unserialize is called" << std::endl;
   zapi::out << "serialize data : " << input << std::endl;
}

MagicMethodClass::~MagicMethodClass() ZAPI_DECL_NOEXCEPT
{}

Variant ObjectVariantClass::__invoke(Parameters &params) const
{
   zapi::out << "ObjectVariantClass::__invoke invoked" << std::endl;
   StringVariant str(params.at(0).getUnDerefZvalPtr(), true);
   NumericVariant result;
   for (int i = 1; i < params.size(); i++) {
      result += NumericVariant(params.at(i));
   }
   str = "zapi";
   return str;
}

void ObjectVariantClass::forwardInvoke()
{
   ObjectVariant obj("ObjectVariantClass", std::make_shared<ObjectVariantClass>());
   Variant str("xxx");
   zapi::out << "begin invoke ObjectVariant::classInvoke : the text is xxx" << std::endl;
   Variant result = obj(str.makeReferenceByZval(), 123, 456, 222);
   zapi::out << "after invoke ObjectVariant::classInvoke : this text is " << result << std::endl;
   ObjectVariant obj1("NonMagicMethodClass", std::make_shared<NonMagicMethodClass>());
   // obj1(1, 2); // Fatal error: Function name must be a string
}

void ObjectVariantClass::testInstanceOf()
{
   ObjectVariant objA("A", std::make_shared<A>());
   ObjectVariant objB("B", std::make_shared<B>());
   ObjectVariant objC("C", std::make_shared<C>());
   if (objA.instanceOf("A") && objA.instanceOf(objA)) {
      zapi::out << "A is instance of A" << std::endl;
   }
   if (objB.instanceOf("B") && objB.instanceOf(objB)) {
      zapi::out << "B is instance of B" << std::endl;
   }
   if (objC.instanceOf("C") && objC.instanceOf(objC)) {
      zapi::out << "C is instance of C" << std::endl;
   }
   if (objB.instanceOf("A") && objB.instanceOf(objA)) {
      zapi::out << "B is instance of A" << std::endl;
   }
   if (objC.instanceOf("B") && objC.instanceOf(objB)) {
      zapi::out << "C is instance of B" << std::endl;
   }
   if (objC.instanceOf("A") && objC.instanceOf(objA)) {
      zapi::out << "C is instance of A" << std::endl;
   }
   
   if (!objA.instanceOf("B") && !objA.instanceOf(objB)) {
      zapi::out << "A is not instance of B" << std::endl;
   }
   if (!objB.instanceOf("C") && !objB.instanceOf(objC)) {
      zapi::out << "C is not instance of B" << std::endl;
   }
   if (!objA.instanceOf("C") && !objA.instanceOf(objC)) {
      zapi::out << "C is not instance of A" << std::endl;
   }
}

void ObjectVariantClass::testDerivedFrom()
{
   ObjectVariant objA("A", std::make_shared<A>());
   ObjectVariant objB("B", std::make_shared<B>());
   ObjectVariant objC("C", std::make_shared<C>());
   if (!objA.derivedFrom("A") && !objA.derivedFrom(objA)) {
      zapi::out << "A is not derived from A" << std::endl;
   }
   if (!objB.derivedFrom("B") && !objB.derivedFrom(objB)) {
      zapi::out << "B is not derived from B" << std::endl;
   }
   if (!objC.derivedFrom("C") && !objC.derivedFrom(objC)) {
      zapi::out << "C is not derived from C" << std::endl;
   }
   if (objB.derivedFrom("A") && objB.derivedFrom(objA)) {
      zapi::out << "B is derived from A" << std::endl;
   }
   if (objC.derivedFrom("B") && objC.derivedFrom(objB)) {
      zapi::out << "C is derived from B" << std::endl;
   }
   if (objC.derivedFrom("A") && objC.derivedFrom(objA)) {
      zapi::out << "C is derived from A" << std::endl;
   }
   
   if (!objA.derivedFrom("B") && !objA.derivedFrom(objB)) {
      zapi::out << "A is not derived from B" << std::endl;
   }
   if (!objB.derivedFrom("C") && !objB.derivedFrom(objC)) {
      zapi::out << "C is not derived from B" << std::endl;
   }
   if (!objA.derivedFrom("C") && !objA.derivedFrom(objC)) {
      zapi::out << "C is not derived from A" << std::endl;
   }
}

void ObjectVariantClass::testNoArgCall()
{
   ObjectVariant obj("ObjectVariantClass", std::make_shared<ObjectVariantClass>());
   obj.call("printName");
   StringVariant ret = obj.call("getName");
   zapi::out << "the result of ObjectVariantClass::getName is " << ret << std::endl;
}

void ObjectVariantClass::testVarArgsCall()
{
   ObjectVariant obj("ObjectVariantClass", std::make_shared<ObjectVariantClass>());
   obj.call("printSum", 12, 12, 12);
   Variant ret = obj.call("calculateSum", 1, 2, 4);
   zapi::out << "the result of ObjectVariantClass::calculateSum is " << ret << std::endl;
   // test ref arg
   Variant str("zapi");
   zapi::out << "before call by ref arg " << str << std::endl;
   obj.call("changeNameByRef", str.makeReferenceByZval());
   zapi::out << "after call by ref arg " << str << std::endl;
}

void ObjectVariantClass::printName()
{
   zapi::out << "ObjectVariantClass::printName been called" << std::endl;
}

std::string ObjectVariantClass::getName()
{
   zapi::out << "ObjectVariantClass::getName been called" << std::endl;
   return "hello, zapi";
}

void ObjectVariantClass::printSum(NumericVariant argQuantity, ...)
{
   zapi::out << "ObjectVariantClass::printSum been called" << std::endl;
   zapi::out << "got " << argQuantity << " args" << std::endl;
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   zapi::out << "the result is " << result << std::endl;
}

int ObjectVariantClass::calculateSum(NumericVariant argQuantity, ...)
{
   zapi::out << "ObjectVariantClass::calculateSum been called" << std::endl;
   zapi::out << "got " << argQuantity << " args" << std::endl;
   va_list args;
   va_start(args, argQuantity);
   NumericVariant result;
   for (int i = 0; i < argQuantity; ++i) {
      result += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
   zapi::out << "the result is " << result << std::endl;
   return result;
}


void ObjectVariantClass::changeNameByRef(StringVariant &name)
{
   zapi::out << "ObjectVariantClass::changeNameByRef been called" << std::endl;
   if (name.getUnDerefType() == Type::Reference) {
      zapi::out << "get ref arg" << std::endl;
   }
   name = "hello, zapi";
}

void A::printInfo()
{
   zapi::out << "A::printInfo been called" << std::endl;
}

void A::changeNameByRef(StringVariant &name)
{
   zapi::out << "A::changeNameByRef been called" << std::endl;
   if (name.getUnDerefType() == Type::Reference) {
      zapi::out << "get ref arg" << std::endl;
   }
   name = "hello, zapi";
}

void A::privateAMethod()
{
   zapi::out << "A::privateBMethod been called" << std::endl;
}

void A::protectedAMethod()
{
   zapi::out << "A::protectedAMethod been called" << std::endl;
}

void B::printInfo()
{
   zapi::out << "B::printInfo been called" << std::endl;
}

void B::showSomething()
{
   zapi::out << "B::showSomething been called" << std::endl;
   
}

void B::calculateSumByRef(NumericVariant argQuantity, NumericVariant retval, ...)
{
   zapi::out << "C::calculateSumByRef been called" << std::endl;
   zapi::out << "got " << argQuantity << " args" << std::endl;
   if (retval.getUnDerefType() == Type::Reference) {
      zapi::out << "retval is reference arg" << std::endl;
   }
   va_list args;
   va_start(args, retval);
   for (int i = 0; i < argQuantity - 1; ++i) {
      retval += NumericVariant(va_arg(args, zapi_varidic_item_type), false);
   }
}

Variant B::addTwoNumber(NumericVariant &lhs, NumericVariant &rhs)
{
   zapi::out << "B::addTwoNumber been called" << std::endl;
   return lhs + rhs;
}

void B::privateBMethod()
{
   zapi::out << "B::privateBMethod been called" << std::endl;
}

void B::protectedBMethod()
{
   zapi::out << "B::protectedBMethod been called" << std::endl;
   callParent("protectedAMethod");
}

void C::printInfo()
{
   zapi::out << "C::printInfo been called" << std::endl;
   callParent("printInfo");
   callParent("showSomething");
}

void C::testCallParentPassRefArg()
{
   zapi::out << "C::testCallParentPassRefArg been called" << std::endl;
   Variant str("xxxx");
   zapi::out << "before call changeNameByRef : " << str << std::endl;
   callParent("changeNameByRef", str.makeReferenceByZval());
   zapi::out << "after call changeNameByRef : " << str << std::endl;
   // pass arg when variant args
   NumericVariant ret(0);
   zapi::out << "before call calculateSumByRef : " << ret.toLong() << std::endl;
   callParent("calculateSumByRef", ret.makeReferenceByZval(), 12, 2, 33);
   zapi::out << "after call calculateSumByRef : " << ret.toLong() << std::endl;
   
}

void C::testCallParentWithReturn()
{
   zapi::out << "C::testCallParentWithReturn been called" << std::endl;
   Variant ret = callParent("addTwoNumber", 1, 23);
   zapi::out << "after call addTwoNumber get : " << ret << std::endl;
}

void C::testGetObjectVaraintPtr()
{
   zapi::out << "C::testGetObjectVaraintPtr been called" << std::endl;
   ObjectVariant *objZvalPtr = this->getObjectZvalPtr();
   if (objZvalPtr->hasProperty("address")) {
      zapi::out << "property C::address exists" << std::endl;
      zapi::out << "property value : " << objZvalPtr->getProperty("address") << std::endl;
   }
   if (!objZvalPtr->hasProperty("privateName")) {
      zapi::out << "property C::privateName not exists" << std::endl;
   }
   if (objZvalPtr->hasProperty("protectedName")) {
      zapi::out << "property C::protectedName exists" << std::endl;
      zapi::out << "property value : " << objZvalPtr->getProperty("protectedName") << std::endl;
   }
   if (objZvalPtr->methodExist("showSomething")) {
      zapi::out << "method C::showSomething exists" << std::endl;
      objZvalPtr->call("showSomething");
   }
   if (objZvalPtr->methodExist("privateCMethod")) {
      zapi::out << "method C::privateCMethod exists" << std::endl;
      objZvalPtr->call("privateCMethod");
   }
   if (objZvalPtr->methodExist("privateAMethod")) {
      zapi::out << "method C::privateCMethod exists" << std::endl;
      // objZvalPtr->call("privateAMethod"); fata error
   }
   if (objZvalPtr->methodExist("protectedAMethod")) {
      zapi::out << "method C::protectedAMethod exists" << std::endl;
      objZvalPtr->call("protectedAMethod");
   }
   if (objZvalPtr->methodExist("privateBMethod")) {
      zapi::out << "method C::privateBMethod exists" << std::endl;
      // objZvalPtr->call("privateAMethod"); fata error
   }
   if (objZvalPtr->methodExist("protectedBMethod")) {
      zapi::out << "method C::protectedBMethod exists" << std::endl;
      objZvalPtr->call("protectedBMethod");
   }
}

void C::privateCMethod()
{
   zapi::out << "C::privateCMethod been called" << std::endl;
}

void C::protectedCMethod()
{
   zapi::out << "C::protectedCMethod been called" << std::endl;
}

void C::methodOfA()
{
   
}

void C::protectedMethodOfA()
{
   
}

void C::privateMethodOfA()
{
   
}

void VisibilityClass::publicMethod()
{}

void VisibilityClass::protectedMethod()
{}

void VisibilityClass::privateMethod()
{}

void VisibilityClass::finalMethod()
{}

void FinalTestClass::someMethod()
{
}

IterateTestClass::IterateTestClass()
   : AbstractIterator(this)
{
   m_items.push_back(std::make_pair<std::string, std::string>("key1", "value1"));
   m_items.push_back(std::make_pair<std::string, std::string>("key2", "value2"));
   m_items.push_back(std::make_pair<std::string, std::string>("key3", "value3"));
   m_items.push_back(std::make_pair<std::string, std::string>("key4", "value4"));
   m_currentIter = m_items.begin();
}

void AbstractTestClass::normalMethod()
{
   
}

AbstractIterator *IterateTestClass::getIterator()
{
   return this;
}

bool IterateTestClass::valid()
{
   zapi::out << "IterateTestClass::valid called" << std::endl;
   return m_currentIter != m_items.end();
}

Variant IterateTestClass::current()
{
   zapi::out << "IterateTestClass::current called" << std::endl;
   return m_currentIter->second;
}

Variant IterateTestClass::key()
{
   zapi::out << "IterateTestClass::key called" << std::endl;
   return m_currentIter->first;
}

void IterateTestClass::next()
{
   zapi::out << "IterateTestClass::next called" << std::endl;
   m_currentIter++;
}

void IterateTestClass::rewind()
{
   zapi::out << "IterateTestClass::rewind called" << std::endl;
   m_currentIter = m_items.begin();
}

zapi_long IterateTestClass::count()
{
   zapi::out << "IterateTestClass::count called" << std::endl;
   return m_items.size();
}

bool IterateTestClass::offsetExists(Variant offset)
{
   auto begin = m_items.begin();
   auto end = m_items.end();
   std::string key = StringVariant(std::move(offset)).toString();
   while (begin != end) {
      if (begin->first == key) {
         return true;
      }
      begin++;
   }
   return false;
}

void IterateTestClass::offsetSet(Variant offset, Variant value)
{
   auto begin = m_items.begin();
   auto end = m_items.end();
   std::string key = StringVariant(std::move(offset)).toString();
   while (begin != end) {
      if (begin->first == key) {
         begin->second = StringVariant(std::move(value)).toString();
         return;
      }
      begin++;
   }
}

Variant IterateTestClass::offsetGet(Variant offset)
{
   auto begin = m_items.begin();
   auto end = m_items.end();
   std::string key = StringVariant(std::move(offset)).toString();
   while (begin != end) {
      if (begin->first == key) {
         return begin->second;
      }
      begin++;
   }
   return nullptr;
}

void IterateTestClass::offsetUnset(Variant offset)
{
   auto begin = m_items.begin();
   auto end = m_items.end();
   std::string key = StringVariant(std::move(offset)).toString();
   while (begin != end) {
      if (begin->first == key) {
         break;
      }
      begin++;
   }
   if (begin != end) {
      m_items.erase(begin);
   }
   
}

IterateTestClass::~IterateTestClass()
{}

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

void ClosureTestClass::testClosureCallable()
{
   CallableVariant callableVar(print_something);
   //callableVar();
}

Variant ClosureTestClass::getNoArgAndReturnCallable()
{
   return CallableVariant(print_something);
}

Variant ClosureTestClass::getArgAndReturnCallable()
{
   return CallableVariant(have_ret_and_have_arg);
}

ClosureTestClass::~ClosureTestClass()
{}
