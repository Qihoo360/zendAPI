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

#ifndef ZAPI_VM_INVOKE_BRIDGE_H
#define ZAPI_VM_INVOKE_BRIDGE_H

#include "zapi/Global.h"
#include "zapi/kernel/Exception.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/lang/Parameters.h"
#include "zapi/lang/Argument.h"
#include "zapi/ds/Variant.h"
#include "zapi/vm/InvokeBridge.h"
#include "zapi/vm/ObjectBinder.h"
#include "zapi/stdext/TypeTraits.h"
#include "php/Zend/zend_exceptions.h"

#include <ostream>
#include <type_traits>

struct _zend_execute_data;
struct _zval_struct;

// forward declare with namespace
namespace zapi
{

namespace ds
{
class Variant;
} // ds
namespace lang
{
class Parameters;
class StdClass;
} // lang

} // zapi
// end forward declare 

using zapi::kernel::Exception;
using zapi::lang::Parameters;
using zapi::lang::Arguments;
using zapi::lang::StdClass;
using zapi::ds::Variant;

namespace
{

void yield(_zval_struct *return_value, const Variant &value)
{
   RETVAL_ZVAL(static_cast<zval *>(value), 1, 0);
}

void yield(_zval_struct *return_value, std::nullptr_t value)
{
   RETVAL_NULL();
}

}

namespace zapi
{
namespace vm
{

template <typename T, typename ::std::decay<T>::type callable, 
          bool HasReturn, bool isMemberFunc>
class InvokeBridgePrivate
{
public:
   virtual ~InvokeBridgePrivate() = default;
private:
   static StdClass *instance(zend_execute_data *execute_data);
   static bool checkInvokeArguments(zend_execute_data *execute_data, zval *return_value);
   static Parameters retrieveParameters(zend_execute_data *execute_data);
   
public:
   //   template <typename T, void(T::*method)()> 
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         (static_cast<T *>(instance(execute_data))->*method)();
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, void(T::*method)(Parameters &params)>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         (static_cast<T *>(instance(execute_data))->*method)(params);
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, Variant(T::*method)()>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         Variant result = (static_cast<T *>(instance(execute_data))->*method)();
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, Variant(T::*method)(Parameters &params)>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         Variant result = (static_cast<T *>(instance(execute_data))->*method)(params);
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, void(T::*method)() const> 
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         (static_cast<T *>(instance(execute_data))->*method)();
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, void(T::*method)(Parameters &params) const>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         (static_cast<T *>(instance(execute_data))->*method)(params);
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, Variant(T::*method)() const>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         Variant result = (static_cast<T *>(instance(execute_data))->*method)();
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <typename T, Variant(T::*method)(Parameters &params) const>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         Variant result = (static_cast<T *>(instance(execute_data))->*method)(params);
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }   
   
   //   template <void(*callable)()>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         callable();
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <Variant(*callable)()>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         Variant result = callable();
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <void(*callable)(Parameters &parameters)>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         callable(params);
   //         yield(return_value, nullptr);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
   
   //   template <Variant(*callable)(Parameters &parameters)>
   //   static void invoke(zend_execute_data *execute_data, zval *return_value)
   //   {
   //      try {
   //         // check invoke arguments
   //         if (!checkInvokeArguments(execute_data, return_value)) {
   //            return;
   //         }
   //         Parameters params = retrieveParameters(execute_data);
   //         Variant result = callable(params);
   //         yield(return_value, result);
   //      } catch (Exception &exception) {
   //         handle(exception);
   //      }
   //   }
};


template <typename T, typename std::decay<T>::type callable, 
          bool HasReturn, bool isMemberFunc>
StdClass *
InvokeBridgePrivate<T, callable, HasReturn, isMemberFunc>::instance(zend_execute_data *execute_data)
{
   return ObjectBinder::retrieveSelfPtr(getThis())->getNativeObject();
}

template <typename T, typename std::decay<T>::type callable, 
          bool HasReturn, bool isMemberFunc>
bool 
InvokeBridgePrivate<T, callable, HasReturn, isMemberFunc>::checkInvokeArguments(_zend_execute_data *execute_data, _zval_struct *return_value)
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


template <typename T, typename std::decay<T>::type callable, 
          bool HasReturn, bool isMemberFunc>
Parameters 
InvokeBridgePrivate<T, callable, HasReturn, isMemberFunc>::retrieveParameters(_zend_execute_data *execute_data)
{
   return Parameters(getThis(), ZEND_NUM_ARGS());
}

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, false>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         callable();
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         zapi::kernel::process_exception(exception);
      }
   }
};

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, true>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
   }
};

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, true, false>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      //      try {
      //         // check invoke arguments
      //         if (!checkInvokeArguments(execute_data, return_value)) {
      //            return;
      //         }
      //         Parameters params = retrieveParameters(execute_data);
      //         (static_cast<T *>(instance(execute_data))->*method)(params);
      //         yield(return_value, nullptr);
      //      } catch (Exception &exception) {
      //         handle(exception);
      //      }
   }
};

template <typename CallableType, typename std::decay<CallableType>::type callable>
class InvokeBridgePrivate <CallableType, callable, true, true>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
   }
};

template <typename CallableType, 
          typename std::decay<CallableType>::type callable,
          typename DecayCallableType = typename std::decay<CallableType>::type>
class InvokeBridge : public InvokeBridgePrivate<DecayCallableType, callable, 
      zapi::stdext::callable_has_return<DecayCallableType>::value, 
      std::is_member_function_pointer<DecayCallableType>::value>
{
};


} // vm
} // zapi

#endif // ZAPI_VM_INVOKE_BRIDGE_H