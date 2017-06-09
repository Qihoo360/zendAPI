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
// Created by zzu_softboy on 09/06/2017.

#ifndef ZAPI_UTILS_LOWERCASE_H
#define ZAPI_UTILS_LOWERCASE_H

#include "php/Zend/zend_types.h"
#include "php/Zend/zend_operators.h"

namespace zapi
{
namespace utils
{

class LowerCase
{
private:
   /**
    * The lowercase string
    * @var zend_string
    */
   zend_string *m_value;
public:
   /**
    * Constructor
    * @param value the original value
    */
   LowerCase(zend_string *value)
      : m_value(zend_string_tolower(value))
   {}

   /**
    * No copy'ing or moving
    * @param  that
    */
   LowerCase(const LowerCase &other) = delete;
   LowerCase(LowerCase &&other) = delete;

   /**
    * Destructor
    */
   virtual ~LowerCase()
   {
      zend_string_release(m_value);
   }

   /**
    * Expose internal value
    * @return zend_string*
    */
   zend_string *getValue()
   {
      return m_value;
   }

   /**
    * Cast to a zend_string
    * @return zend_string*
    */
   operator zend_string *()
   {
      return m_value;
   }
};

} //utils
} // zapi

#endif // ZAPI_UTILS_LOWERCASE_H
