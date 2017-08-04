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
// Created by zzu_softboy on 2017/07/27.

#include "zapi/vm/Engine.h"
#include "zapi/vm/ExecStateGuard.h"
#include "zapi/lang/Variant.h"
#include "zapi/kernel/OrigException.h"

namespace zapi
{
namespace vm
{

using zapi::kernel::OrigException;

namespace
{
void op_deleter(zend_op_array* opArray)
{
   if (opArray) {
      destroy_op_array(opArray);
      efree_size(opArray, sizeof(zend_op_array));
   }
}
}

Variant Engine::eval(const std::string &code)
{
   char *evalDesc = zend_make_compiled_string_description("eval()'d code");
   Variant source(code);
   Variant retValue;
   zend_object *origException = EG(exception);
   zend_try {
      std::shared_ptr<zend_op_array> opcodes(zend_compile_string(source, evalDesc), op_deleter);
      if (opcodes) {
         retValue = executeOpCodes(opcodes);
      }
      efree(evalDesc);
   } zend_end_try();
   zend_object *exception = EG(exception);
   if (exception != origException) {
      zend_try {
         zend_exception_error(exception, E_ERROR);
      } zend_end_try();
   }
   return retValue;
}

Variant Engine::executeOpCodes(std::shared_ptr<zend_op_array> opcodes)
{
   zval retValue;
   ZVAL_NULL(&retValue);
   ExecStateGuard execState(0);
   CG(active_op_array) = opcodes.get();
   EG(no_extensions) = 1;
   if (EG(current_execute_data) && !EG(current_execute_data)->symbol_table) {
      zend_rebuild_symbol_table();
   }
   auto *oldException = EG(exception);
   zend_try {
      zend_execute(opcodes.get(), &retValue);
   }zend_end_try();
   if (oldException != EG(exception) && EG(exception)) {
      zend_exception_error(EG(exception), E_ERROR);
   }
   if (ZVAL_IS_NULL(&retValue)) {
      return nullptr;
   }
   return Variant(&retValue);
}

} // vm
} // zapi