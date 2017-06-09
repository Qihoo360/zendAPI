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
// Created by zzu_softboy on 08/06/2017.

#ifndef ZAPI_UTILS_ARITHMETIC_H
#define ZAPI_UTILS_ARITHMETIC_H

#include <cstdlib>

#include "zapi/lang/Variant.h"

namespace zapi
{
namespace utils
{

using zapi::lang::Variant;

template <template<typename T> class F>
class Arithmetic
{
private:
   /**
    *  Pointer to the original Variant object
    *  @var    Variant
    */
   Variant *m_value;
public:
   /**
    *  Constructor
    *  @param  value       The original object
    */
   Arithmetic(Variant *value) : m_value(value)
   {}

   virtual ~Arithmetic() {}

   /**
    * Apply a number, and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(const Variant &value)
   {
      if (value.isDouble())
      {
         return apply(value.getDoubleValue());
      }
      return apply(value.getNumericValue());
   }

   /**
    * Apply a number, and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(int16_t value)
   {
      // check if the current object is a floating point number
      if (m_value->isDouble()) {
         return Variant(F<double>()(m_value->getDoubleValue(), value));
      }
      // apply to natural numbers
      return Variant(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Apply a number, and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(int32_t value)
   {
      // check if the current object is a floating point number
      if (m_value->isDouble()) {
         return Variant(F<double>()(m_value->getDoubleValue(), value));
      }
      return Variant(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Apply a number, and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(int64_t value)
   {
      // check if the current object is a floating point number
      if (m_value->isDouble()) {
         return Variant(F<double>()(m_value->getDoubleValue(), value));
      }
      return Variant(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Apply a boolean (treat is as 0 or 1), and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(bool value)
   {
      // check if the current object is a floating point number
      if (m_value->isDouble()) {
         return Variant(F<double>()(m_value->getDoubleValue(), value ? 1 : 0));
      }
      return Variant(F<int64_t>()(m_value->getNumericValue(), value ? 1 : 0));
   }

   /**
    * Apply a character (value between '0' and '9'), and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(char value)
   {
      // convert to an integer
      int v = value < '0' || value > '9' ? 0 : value - '0';
      if (m_value->isDouble()) {
         return Variant(F<double>()(m_value->getDoubleValue(), value));
      }
      return Variant(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    *  Apply a string (representing a number), and return a new Variant object after running the arithmetic function
    *  @param  value
    *  @return Variant
    */
   Variant apply(const std::string &value)
   {
      return apply(static_cast<int64_t>(std::stoll(value)));
   }

   /**
    * Apply a string (representing a number), and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(const char *value)
   {
      return apply(std::atoi(value));
   }

   /**
    * Apply a string (representing a number), and return a new Variant object after running the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant apply(double value)
   {
      return Variant(F<double>()(m_value->getDoubleValue(), value));
   }

   /**
    * Assign a different Variant object, applying the arithmetic operation
    * @param  value
    * @return Variant
    */
   Variant &assign(const Variant &value)
   {
      // is this a type a floating point type?
      if (value.isDouble())
      {
         return assign(value.getDoubleValue());
      }
      // we are going to treat it as a numeric (non floating) type
      return assign(value.getNumericValue());;
   }

   /**
    * Assign a 16bit number, applying the arithmetic operation
    * @param  value
    * @return Variant
    */
   Variant &assign(int16_t value)
   {
      if (m_value->isDouble()) {
         return m_value->operator=(F<double>()(m_value->getDoubleValue(), value));
      }
      return m_value->operator=(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Assign 32bit integer, applying the arithmetic operation
    * @param  value
    * @return Variant
    */
   Variant &assign(int32_t value)
   {
      if (m_value->isDouble()) {
         return m_value->operator=(F<double>()(m_value->getDoubleValue(), value));
      }
      return m_value->operator=(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Assign 64bit integer, applying the arithmetic operation
    * @param  value
    * @return Variant
    */
   Variant &assign(int64_t value)
   {
      if (m_value->isDouble()) {
         return m_value->operator=(F<double>()(m_value->getDoubleValue(), value));
      }
      return m_value->operator=(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Assign boolean value - which is treated as 1 or 0 - applying the arithmetic operation
    * @param  value
    * @return Variant
    */
   Variant &assign(bool value)
   {
      if (m_value->isDouble()) {
         return m_value->operator=(F<double>()(m_value->getDoubleValue(), value ? 1: 0));
      }
      return m_value->operator=(F<int64_t>()(m_value->getNumericValue(), value ? 1 : 0));
   }

   /**
    *  Assign a single character - which is treated as an int, and applying the arithmetic function
    *  @param  value
    *  @return Variant
    */
   Variant &assign(char value)
   {
      value = value < '0' || value > '9' ? 0 : value - '0';
      if (m_value->isDouble()) {
         return m_value->operator=(F<double>()(m_value->getDoubleValue(), value));
      }
      return m_value->operator=(F<int64_t>()(m_value->getNumericValue(), value));
   }

   /**
    * Assign a a string - treating it as an integer, and applying the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant &assign(const std::string &value)
   {
      return assign(static_cast<int64_t>(std::stoll(value)));
   }

   /**
    * Assign a string - treating it as an integer, and applying the arithmetic function
    * @param  value
    * @return Variant
    */
   Variant &assign(const char *value)
   {
      return assign(std::atoi(value));
   }

   /**
    * Assign a double, applying the arithmetic operation
    * @param  value
    * @return Value
    */
   Variant &assign(double value)
   {
      return m_value->operator=(F<double>()(m_value->getDoubleValue(), value));
   }
};

} // utils
} // zapi

#endif //ZAPI_UTILS_ARITHMETIC_H
