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

} // dummyext
