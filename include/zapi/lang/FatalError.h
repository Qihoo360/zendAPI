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
// Created by zzu_softboy on 07/06/2017.

#ifndef ZAPI_LANG_FATALERROR_H
#define ZAPI_LANG_FATALERROR_H

#include "zapi/lang/Exception.h"

namespace zapi
{
namespace lang
{

class ZAPI_DECL_EXPORT FatalError : public Exception
{
public:
   /**
    * Constructor
    * @param  message
    */
   FatalError(const std::string &message) : Exception(message)
   {}

   /**
    * Destructor
    */
   virtual ~FatalError() = default;

   /**
    * Is this a native exception (one that was thrown from C++ code)
    * @return bool
    */
   virtual bool native() const override
   {
      // although it is native, we return 0 because it should not persist
      // as exception, but it should live on as zend_error() in stead
      return false;
   }

   /**
    * Report this error as a fatal error
    * @return bool
    */
   virtual bool report() const override;
};

} // lang
} // zapi

#endif //ZAPI_LANG_FATALERROR_H
