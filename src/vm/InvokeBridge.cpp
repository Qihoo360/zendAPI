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
// Created by zzu_softboy on 2017/07/26.

#include <ostream>
#include "zapi/vm/InvokeBridge.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/vm/ObjectBinder.h"

namespace zapi
{
namespace vm
{

StdClass *InvokeBridge::instance(zend_execute_data *execute_data)
{
   return ObjectBinder::retrieveSelfPtr(getThis())->getNativeObject();
}

bool InvokeBridge::checkInvokeArguments(_zend_execute_data *execute_data, _zval_struct *return_value)
{
   uint32_t required = execute_data->func->common.required_num_args;
   uint32_t provided = ZEND_NUM_ARGS();
   // we just check arguments number
   if (provided >= required) {
      return true;
   }
   const char *name = get_active_function_name();
   // TODO
   zapi::warning << name << "() expects at least " << required << " parameter(s), " 
                 << provided << " given" << std::flush;
   RETVAL_NULL();
   return false;
}

void InvokeBridge::yield(_zval_struct *return_value, const lang::Variant &value)
{
   RETVAL_ZVAL(static_cast<zval *>(value), 1, 0);
}

void InvokeBridge::yield(_zval_struct *return_value, std::nullptr_t value)
{
   RETVAL_NULL();
}

Parameters InvokeBridge::retrieveParameters(_zend_execute_data *execute_data)
{
   return Parameters(getThis(), ZEND_NUM_ARGS());
}

void InvokeBridge::handle(Exception &exception)
{
   zapi::kernel::process_exception(exception);
}

} // vm
} // zapi

