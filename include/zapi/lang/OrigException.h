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
// Created by softboy on 6/8/17.

#ifndef ZAPI_LANG_ORIGEXCEPTION_H
#define ZAPI_LANG_ORIGEXCEPTION_H

#include <string>

#include "php/Zend/zend_types.h"

#include "zapi/lang/Exception.h"

namespace zapi
{
namespace lang
{

class OrigException : public Exception
{
private:
   /**
    * Is this a an exception that was caught by extension C++ code.
    *
    * When the object is initially created, we assume that it will be caught
    * by C++ code. If it later turns out that the zapi can catch this
    * exception after the extension C++ code ran, the variable is set back
    * to false.
    *
    * @var bool
    */
   bool m_handled = true;

   /**
    * The PHP exception code
    * @var    long int
    */
   long int m_code;

   /**
    * PHP source file
    * @var std::string
    */
   std::string m_file;

   /**
    * PHP source line
    * @var long int
    */
   long int m_line;

public:
   /**
    * Constructor
    * @param  object  The object that was thrown
    */
   OrigException(zend_object *object)
      : Exception(std::string{ZSTR_VAL(object->ce->name), ZSTR_LEN(object->ce->name)})
   {
      // the result value from zend and the object zval
      zval result;
      zval properties;
      ZVAL_OBJ(&properties, object);
      // retrieve the message, filename, error code and line number
      auto message = zval_get_string(zend_read_property(Z_OBJCE(properties), &properties,
                                                        ZEND_STRL("message"), 1, &result));
      auto file = zval_get_string(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("file"), 1, &result));
      auto code = zval_get_long(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("code"), 1, &result));
      auto line = zval_get_long(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("line"), 1, &result));
      // store the message, code, filename and line number
      m_message.assign(ZSTR_VAL(message), ZSTR_LEN(message));
      m_code = code;
      m_file.assign(ZSTR_VAL(file), ZSTR_LEN(file));
      m_line = line;
      // clean up message and file strings
      zend_string_release(message);
      zend_string_release(file);
   }
};

} // lang
} // zapi

#endif // ZAPI_LANG_ORIGEXCEPTION_H
