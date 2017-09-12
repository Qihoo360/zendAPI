// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by softboy on 2017/09/11.

#include "php/sapi/embed/php_embed.h"
#include "gtest/gtest.h"
#include "zapi/ds/CallableVariant.h"
#include <iostream>
#include <string>

using zapi::ds::CallableVariant;

void print_name()
{
   zapi::out << "zapi" << std::endl;
}

int get_height()
{
   return 28;
}

void print_two_number_sum(int lhs, int rhs)
{
   zapi::out << lhs + rhs << std::endl;
}

int add_two_number(int lhs, int rhs)
{
   return lhs + rhs;
}

class Handler
{
public:
   static void print_name()
   {
      zapi::out << "zapi" << std::endl;
   }
   static int get_height()
   {
      return 28;
   }
   static void print_two_number_sum(int lhs, int rhs)
   {
      zapi::out << lhs + rhs << std::endl;
   }
   static int add_two_number(int lhs, int rhs)
   {
      return lhs + rhs;
   }
};

TEST(CallableVariantTest, testConstruct)
{
//   CallableVariant getNameHandler(print_name);
//   CallableVariant getHeight(get_height);
//   CallableVariant printSumHandler(print_two_number_sum);
//   CallableVariant addTwoNumber(add_two_number);
//   CallableVariant clsGetNameHandler(&Handler::print_name);
//   CallableVariant clsGetHeight(&Handler::get_height);
//   CallableVariant clsPrintSumHandler(&Handler::print_two_number_sum);
//   CallableVariant clsAddTwoNumber(&Handler::add_two_number);
   // call it 
   //getNameHandler();
}
