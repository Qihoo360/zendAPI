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
// Created by zzu_softboy on 26/07/2017.

#ifndef ZAPI_VM_EXEC_STATE_GUARD_H
#define ZAPI_VM_EXEC_STATE_GUARD_H

#include "zapi/Global.h"
#include "php/Zend/zend_compile.h"

namespace zapi
{
namespace vm
{

class ExecStateGuard
{
public:
   ExecStateGuard() = delete;
   ExecStateGuard(int noExtensions)
   {
      m_activeOpArray = CG(active_op_array) ? CG(active_op_array) : nullptr;
      m_retValue = EG(current_execute_data) ? EG(current_execute_data)->return_value: nullptr;
      m_opline = EG(current_execute_data) ? EG(current_execute_data)->opline : nullptr;
      m_noExtensions = noExtensions;
   }
   
   ~ExecStateGuard()
   {
      EG(no_extensions) = m_noExtensions;
      if (EG(current_execute_data)) {
         EG(current_execute_data)->opline = m_opline;
         EG(current_execute_data)->return_value = m_retValue;
      }
      CG(active_op_array) = m_activeOpArray;
   }
   
private:
   zend_op_array *m_activeOpArray;
   zval *m_retValue;
   const zend_op *m_opline;
   int m_noExtensions;
};

} // vm
} // zapi

#endif // ZAPI_VM_EXEC_STATE_GUARD_H