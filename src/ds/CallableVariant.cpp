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
// Created by softboy on 2017/08/21.

#include "zapi/ds/CallableVariant.h"
#include "zapi/kernel/Exception.h"
#include "zapi/kernel/OrigException.h"
#include <string>

using zapi::ds::Variant;
using zapi::kernel::Exception;
using zapi::kernel::OrigException;

namespace
{
Variant do_execute(const zval *object, zval *func, int argc, zval *argv)
{
   zval retval;
   zend_object *oldException = EG(exception);
   if (ZAPI_SUCCESS != call_user_function_ex(CG(function_table), const_cast<zval *>(object), func, &retval, 
                                             argc, argv, 1, nullptr)) {
      std::string msg("Invalid call to ");
      msg.append(Z_STRVAL_P(func), Z_STRLEN_P(func));
      throw Exception(std::move(msg));
      return nullptr; // just for prevent some compiler warnings
   } else {
      // detect whether has exception throw from PHP code, if we got,
      // we throw an native c++ exception, let's c++ code can
      // handle it
      if (oldException != EG(exception) && EG(exception)) {
         throw OrigException(EG(exception));
      }
      if (Z_ISUNDEF(retval)) {
         return nullptr;
      }
      // wrap the retval into Variant
      Variant result(&retval);
      // here we decrease the refcounter
      zval_ptr_dtor(&retval);
      return result;
   }
}
}

namespace zapi
{
namespace ds
{

Variant CallableVariant::exec(int argc, Variant *argv) const
{
   zval params[argc];
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].getZvalPtr();
   }
   return do_execute(nullptr, const_cast<zval *>(getZvalPtr()), argc, params);
}

Variant CallableVariant::operator ()() const
{
   return do_execute(nullptr, const_cast<zval *>(getZvalPtr()), 0, nullptr);
}

} // ds
} // zapi