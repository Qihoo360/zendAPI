// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
// @copyright 2014 Copernica BV
//
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
// Created by zzu_softboy on 07/06/2017.

#ifndef ZAPI_KERNEL_EXCEPTION_H
#define ZAPI_KERNEL_EXCEPTION_H

#include <exception>
#include <string>

#include "zapi/Global.h"

namespace zapi
{
namespace kernel
{

class ZAPI_DECL_EXPORT Exception : public std::exception
{
public:
   /**
    * Constructor
    *
    * @param  message The exception message
    */
   Exception(std::string message);
   /**
    * Destructor
    */
   virtual ~Exception();

   /**
    * Overridden what method
    * @return const char *
    */
   virtual const char *what() const ZAPI_DECL_NOEXCEPT override;

   /**
    *  Returns the message of the exception.
    *  @return &string
    */
   const std::string &getMessage() const ZAPI_DECL_NOEXCEPT;

   /**
    * Returns the exception code
    *
    * @return The exception code
    */
   virtual long int getCode() const ZAPI_DECL_NOEXCEPT;

   /**
    * Retrieve the filename the exception was thrown in
    *
    * @note   This only works if the exception was originally
    *         thrown in PHP userland. If the native() member
    *         function returns true, this function will not
    *         be able to correctly provide the filename.
    *
    * @return The filename the exception was thrown in
    */
   virtual const std::string &getFileName() const ZAPI_DECL_NOEXCEPT;

   /**
    * Retrieve the line at which the exception was thrown
    */
   virtual long int getLine() const ZAPI_DECL_NOEXCEPT;

   /**
    * Is this a native exception (one that was thrown from C++ code)
    * @return bool
    */
   virtual bool native() const;
   /**
    * Report this error as a fatal error
    * @return bool
    */
   virtual bool report() const;
protected:
   /**
    * The exception message
    * @var    char*
    */
    std::string m_message;
};

} // kernel
} // zapi

#endif //ZAPI_KERNEL_EXCEPTION_H
