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
// Created by softboy on 2017/08/04.

#include "zapi/kernel/Exception.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/kernel/FatalError.h"
#include "zapi/kernel/NotImplemented.h"

namespace zapi
{
namespace kernel
{

Exception::Exception(std::string message) 
   : m_message(std::move(message))
{}

Exception::~Exception()
{};

const char *Exception::what() const ZAPI_DECL_NOEXCEPT
{
   return m_message.c_str();
}

const std::string &Exception::getMessage() const ZAPI_DECL_NOEXCEPT
{
   return m_message;
}

long int Exception::getCode() const ZAPI_DECL_NOEXCEPT
{
   return -1;
}

const std::string &Exception::getFileName() const ZAPI_DECL_NOEXCEPT
{
   static std::string file{"<filename unknown>"};
   return file;
}

long int Exception::getLine() const ZAPI_DECL_NOEXCEPT
{
   return -1;
}

bool Exception::native() const
{
   return true;
}

bool Exception::report() const
{
   return false;
}

OrigException::OrigException(zend_object *object)
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

OrigException::OrigException(const OrigException &exception)
   : Exception("OrigException"),
     m_handled(exception.m_handled)
{}

OrigException::OrigException(OrigException &&exception)
   : Exception("OrigException"),
     m_handled(exception.m_handled)
{
   exception.m_handled = true;
}

OrigException::~OrigException() ZAPI_DECL_NOEXCEPT
{
   if (!m_handled) {
      return;
   }
   zend_clear_exception();
}

bool OrigException::native() const
{
   return false;
}


void OrigException::reactivate()
{
   // it was not handled by extension C++ code
   m_handled = false;
}

long int OrigException::getCode() const ZAPI_DECL_NOEXCEPT 
{
   return m_code;
}

const std::string &OrigException::getFileName() const ZAPI_DECL_NOEXCEPT
{
   return m_file;
}

long int OrigException::getLine() const ZAPI_DECL_NOEXCEPT
{
   return m_line;
}

NotImplemented::NotImplemented()
   : std::exception()
{}

NotImplemented::~NotImplemented() ZAPI_DECL_NOEXCEPT
{}

/**
 * Constructor
 * @param  message
 */
FatalError::FatalError(const std::string &message) 
   : Exception(message)
{}

FatalError::~FatalError()
{}

bool FatalError::native() const
{
   // although it is native, we return 0 because it should not persist
   // as exception, but it should live on as zend_error() in stead
   return false;
}

} // kernel
} // zapi