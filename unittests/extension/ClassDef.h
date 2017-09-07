#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H

#include "zapi/ZendApi.h"
#include <string>

using zapi::lang::StdClass;
using zapi::ds::Variant;
using zapi::ds::NumericVariant;
using zapi::ds::StringVariant;

class Person : public StdClass
{
public:
   Person();
   void showName();
   void print_sum(NumericVariant argQuantity, ...);
   void setAge(const NumericVariant &age);
   int getAge();

   Variant getName();
   int addTwoNum(const NumericVariant &num1, const NumericVariant &num2);
   int addSum(NumericVariant argQuantity, ...);
   // access level test method
   void protectedMethod();
   void privateMethod();

   static void staticShowName();
   static StringVariant concatStr(const StringVariant &lhs, const StringVariant &rhs);
   static void staticProtectedMethod();
   static void staticPrivateMethod();
   static void makeNewPerson();
private:
   /**
     *  The initial value
     *  @var    int
     */
   std::string m_name;
   int m_age;
};

class Address : public StdClass
{
public:
   Address();
protected:
   std::string address;
};

class ConstructAndDestruct : public StdClass
{
public:
   void __construct();
   void __destruct();
};

class EmptyClass : public StdClass
{};

class PropsTestClass : public StdClass
{
private:
   int m_age;
   std::string m_name;
public:
   void setAge(const Variant &age);
   Variant getAge();
   void setName(const Variant &name);
   Variant getName();
};

class NonMagicMethodClass : public StdClass
{

};

class MagicMethodClass : public StdClass, public zapi::protocol::Serializable
{
public:
   Variant __call(const std::string &method, Parameters &params) const;
   Variant __invoke(Parameters &params) const;
   void __set(const std::string &key, const Variant &value);
   Variant __get(const std::string &key) const;
   bool __isset(const std::string &key) const;
   void __unset(const std::string &key);
   Variant __toString() const;
   Variant __toInteger() const;
   Variant __toDouble() const;
   Variant __toBool() const;
   int __compare(const MagicMethodClass &object) const;
   void __clone();
   static Variant __callStatic(const std::string &method, Parameters &params);
   virtual std::string serialize();
   virtual void unserialize(const char *input, size_t size);
   virtual ~MagicMethodClass() ZAPI_DECL_NOEXCEPT;
private:
   bool m_teamNameUnset = false;
   bool m_teamAddressUnset = true;
   int m_length = 0;
   std::string m_address;
};

class ObjectVariantClass : public StdClass
{
public:
   Variant __invoke(Parameters &params) const;
   void forwardInvoke();
   void testInstanceOf();
   void testDerivedFrom();
   void testNoArgCall();
   void testVarArgsCall();
   void printName();
   std::string getName();
   void printSum(NumericVariant argQuantity, ...);
   int calculateSum(NumericVariant argQuantity, ...);
};

class A : public StdClass
{};

class B : public StdClass
{};

class C : public StdClass
{};

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
