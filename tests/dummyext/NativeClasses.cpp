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
// Created by softboy on 2017/11/03.

#include "NativeClasses.h"

namespace dummyext 
{

using zapi::lang::ObjectVariant;
using zapi::ds::StringVariant;
using zapi::lang::Type;
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

// for class and interface inherit classes

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

// for test class iterator


IterateTestClass::IterateTestClass()
   : AbstractIterator(this)
{
   m_items.push_back(std::make_pair<std::string, std::string>("key1", "value1"));
   m_items.push_back(std::make_pair<std::string, std::string>("key2", "value2"));
   m_items.push_back(std::make_pair<std::string, std::string>("key3", "value3"));
   m_items.push_back(std::make_pair<std::string, std::string>("key4", "value4"));
   m_currentIter = m_items.begin();
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

} // dummyext
