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
#include "zapi/stdext/Tuple.h"

#include <ostream>
#include <list>
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
using zapi::vm::ObjectBinder;

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

StdClass *instance(zend_execute_data *execute_data)
{
   return ObjectBinder::retrieveSelfPtr(getThis())->getNativeObject();
}

bool check_invoke_arguments(_zend_execute_data *execute_data, _zval_struct *return_value, size_t funcDefinedArgNumber)
{
   uint32_t required = execute_data->func->common.required_num_args;
   uint32_t argNumer = execute_data->func->common.num_args;
   uint32_t provided = ZEND_NUM_ARGS();
   const char *name = get_active_function_name();
   if (funcDefinedArgNumber > argNumer) {
      zapi::warning << name << "native cpp callable definition have " << funcDefinedArgNumber << " parameter(s), " 
                    << "but register meta info given " << argNumer << " parameter(s)." << std::flush;
      RETVAL_NULL();
      return false;
   }
   // we just check arguments number
   if (provided >= required) {
      return true;
   }
   // TODO
   zapi::warning << name << "() expects at least " << required << " parameter(s), " 
                 << provided << " given" << std::flush;
   RETVAL_NULL();
   return false;
}

}

namespace zapi
{
namespace vm
{

template <typename T, typename ::std::decay<T>::type callable, 
          bool isMemberFunc, bool HasReturn, bool HasVariableParam>
class InvokeBridgePrivate
{
public:
   virtual ~InvokeBridgePrivate() = default;
private:
   //   static StdClass *instance(zend_execute_data *execute_data);
   //   static bool checkInvokeArguments(zend_execute_data *execute_data, zval *return_value);
   //   static Parameters retrieveParameters(zend_execute_data *execute_data);
   
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


//template <typename T, typename std::decay<T>::type callable, 
//          bool HasReturn, bool isMemberFunc>
//StdClass *
//InvokeBridgePrivate<T, callable, isMemberFunc, HasReturn, HasVariableParam>::instance(zend_execute_data *execute_data)
//{
//   return ObjectBinder::retrieveSelfPtr(getThis())->getNativeObject();
//}


//template <typename T, typename std::decay<T>::type callable, 
//          bool HasReturn, bool isMemberFunc>
//Parameters 
//InvokeBridgePrivate<T, isMemberFunc, HasReturn, HasVariableParam>::retrieveParameters(_zend_execute_data *execute_data)
//{
//   return Parameters(getThis(), ZEND_NUM_ARGS());
//}

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, false, false>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // no variable param
         constexpr size_t paramNumber = zapi::stdext::callable_params_number<T>::value;
         if (!check_invoke_arguments(execute_data, return_value, paramNumber)) {
            return;
         }
         const size_t argNumber = ZEND_NUM_ARGS();
         zval arguments[argNumber];
         zend_get_parameters_array_ex(argNumber, arguments);
         auto tuple = zapi::stdext::gen_tuple<paramNumber>([&arguments](size_t index){
               return Variant(&arguments[index]);
      });
         zapi::stdext::apply(callable, tuple);
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         zapi::kernel::process_exception(exception);
      }
   }
};

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, false, true>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // variadic params
         constexpr size_t paramNumber = zapi::stdext::callable_params_number<T>::value;
         if (!check_invoke_arguments(execute_data, return_value, paramNumber - 1)) {
            return;
         }
         const size_t argNumber = ZEND_NUM_ARGS();
         zval arguments[argNumber];
         zend_get_parameters_array_ex(argNumber, arguments);
         // 15 arguments is enough ?
         auto tuple = zapi::stdext::gen_tuple<16>(
                  [&arguments, argNumber](size_t index){
            if (index == 0) {
               zval temp;
               ZVAL_LONG(&temp, static_cast<int32_t>(argNumber));
               return temp;
            } else if (index <= argNumber + 1){
               return Variant(&arguments[index - 1]).detach(false);
            } else {
               zval temp;
               ZVAL_NULL(&temp);
               return temp;
            }
         });
         zapi::stdext::apply(callable, tuple);
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         zapi::kernel::process_exception(exception);
      }
   }
};

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, true, false>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // no variable param
         constexpr size_t paramNumber = zapi::stdext::callable_params_number<T>::value;
         if (!check_invoke_arguments(execute_data, return_value, paramNumber)) {
            return;
         }
         const size_t argNumber = ZEND_NUM_ARGS();
         zval arguments[argNumber];
         zend_get_parameters_array_ex(argNumber, arguments);
         auto tuple = zapi::stdext::gen_tuple<paramNumber>(
                  [&arguments](size_t index) {
               return Variant(&arguments[index]); 
         });
         yield(return_value, zapi::stdext::apply(callable, tuple));
      } catch (Exception &exception) {
         zapi::kernel::process_exception(exception);
      }
   }
};

template <typename T, typename std::decay<T>::type callable>
class InvokeBridgePrivate <T, callable, false, true, true>
{
public:
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // variadic params
         constexpr size_t paramNumber = zapi::stdext::callable_params_number<T>::value;
         // for the first marker param
         if (!check_invoke_arguments(execute_data, return_value, paramNumber - 1)) {
            return;
         }
         const size_t argNumber = ZEND_NUM_ARGS();
         zval arguments[argNumber];
         zend_get_parameters_array_ex(argNumber, arguments);
         // 15 arguments is enough ?
         auto tuple = zapi::stdext::gen_tuple<16>(
                  [&arguments, argNumber](size_t index){
            if (index == 0) {
               zval temp;
               ZVAL_LONG(&temp, static_cast<int32_t>(argNumber));
               return temp;
            } else if (index <= argNumber + 1){
               return Variant(&arguments[index - 1]).detach(false);
            } else {
               zval temp;
               ZVAL_NULL(&temp);
               return temp;
            }
         });
         yield(return_value,  zapi::stdext::apply(callable, tuple));
      } catch (Exception &exception) {
         zapi::kernel::process_exception(exception);
      }
   }
};

//template <typename T, typename std::decay<T>::type callable>
//class InvokeBridgePrivate <T, callable, false, true>
//{
//public:
//   static void invoke(zend_execute_data *execute_data, zval *return_value)
//   {
//   }
//};

//template <typename T, typename std::decay<T>::type callable>
//class InvokeBridgePrivate <T, callable, true, false>
//{
//public:
//   static void invoke(zend_execute_data *execute_data, zval *return_value)
//   {
//      //      try {
//      //         // check invoke arguments
//      //         if (!checkInvokeArguments(execute_data, return_value)) {
//      //            return;
//      //         }
//      //         Parameters params = retrieveParameters(execute_data);
//      //         (static_cast<T *>(instance(execute_data))->*method)(params);
//      //         yield(return_value, nullptr);
//      //      } catch (Exception &exception) {
//      //         handle(exception);
//      //      }
//   }
//};

//template <typename CallableType, typename std::decay<CallableType>::type callable>
//class InvokeBridgePrivate <CallableType, callable, true, true>
//{
//public:
//   static void invoke(zend_execute_data *execute_data, zval *return_value)
//   {
//   }
//};

template <typename CallableType, 
          typename std::decay<CallableType>::type callable,
          typename DecayCallableType = typename std::decay<CallableType>::type>
class InvokeBridge : public InvokeBridgePrivate<DecayCallableType, callable,
      std::is_member_function_pointer<DecayCallableType>::value,
      zapi::stdext::callable_has_return<DecayCallableType>::value,
      zapi::stdext::callable_has_variable_param<DecayCallableType>::value>
{
};


} // vm
} // zapi

#endif // ZAPI_VM_INVOKE_BRIDGE_H
