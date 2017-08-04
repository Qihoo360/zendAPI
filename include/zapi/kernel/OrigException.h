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
// Created by softboy on 08/06/2017.

#ifndef ZAPI_KERNEL_ORIGEXCEPTION_H
#define ZAPI_KERNEL_ORIGEXCEPTION_H

#include <string>

#include "php/Zend/zend_types.h"
#include "php/Zend/zend_exceptions.h"
#include "zapi/kernel/Exception.h"

namespace zapi
{
namespace kernel
{

class OrigException : public Exception
{
public:
   /**
    * Constructor
    * @param  object  The object that was thrown
    */
   OrigException(zend_object *object);

   /**
    * Copy constructor
    * @param  exception
    */
   OrigException(const OrigException &exception);

   /**
    * Move constructor
    * @param  exception
    */
   OrigException(OrigException &&exception);
   /**
    * Destructor
    */
   virtual ~OrigException() ZAPI_DECL_NOEXCEPT;

   /**
    * This is _not_ a native exception, it was thrown by a PHP script
    * @return bool
    */
   virtual bool native() const override;

   /**
    * Reactivate the exception
    */
   void reactivate();

   /**
    * Returns the exception code
    *
    * @note   This only works if the exception was originally
    *         thrown in PHP userland. If the native() member
    *         function returns true, this function will not
    *         be able to correctly provide the filename.
    *
    * @return The exception code
    */
   virtual long int getCode() const ZAPI_DECL_NOEXCEPT override;

   /**
    * Retrieve the filename the exception was thrown in
    *
    * @return The filename the exception was thrown in
    */
   virtual const std::string &getFileName() const ZAPI_DECL_NOEXCEPT override;
   
   /**
    * Retrieve the line at which the exception was thrown
    *
    * @return The line number the exception was thrown at
    */
   virtual long int getLine() const ZAPI_DECL_NOEXCEPT override;
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
   
};

namespace
{

inline void process_exception(Exception &exception)
{
   if (exception.native()) {
      zend_throw_exception(zend_exception_get_default(), exception.what(), 0);
   } else if (!exception.report()) {
      // this is not a native exception, so it was originally thrown by a
      // php script, and then not caught by the c++ of the extension, we are
      // going to tell to the exception that it is still active
      OrigException &orig = static_cast<OrigException &>(exception);
      orig.reactivate();
   }
}

}

} // kernel
} // zapi

#endif // ZAPI_KERNEL_ORIGEXCEPTION_H
