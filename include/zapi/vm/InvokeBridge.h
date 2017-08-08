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
#include "zapi/lang/Parameters.h"
#include "zapi/lang/Argument.h"
#include "zapi/ds/Variant.h"

struct _zend_execute_data;
struct _zval_struct;

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

namespace vm
{

using zapi::kernel::Exception;
using zapi::lang::Parameters;
using zapi::lang::Arguments;
using zapi::lang::StdClass;
using zapi::ds::Variant;

class ZAPI_DECL_EXPORT InvokeBridge
{
public:
   virtual ~InvokeBridge() = default;
private:
   static StdClass *instance(zend_execute_data *execute_data);
   static bool checkInvokeArguments(zend_execute_data *execute_data, zval *return_value);
   static Parameters retrieveParameters(zend_execute_data *execute_data);
   static void handle(Exception &exception);
   static void yield(_zval_struct *return_value, std::nullptr_t value);
   static void yield(_zval_struct *return_value, const Variant &value);
   
public:
   template <typename T, void(T::*method)()> 
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         (static_cast<T *>(instance(execute_data))->*method)();
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, void(T::*method)(Parameters &params)>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         (static_cast<T *>(instance(execute_data))->*method)(params);
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, Variant(T::*method)()>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         Variant result = (static_cast<T *>(instance(execute_data))->*method)();
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, Variant(T::*method)(Parameters &params)>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         Variant result = (static_cast<T *>(instance(execute_data))->*method)(params);
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, void(T::*method)() const> 
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         (static_cast<T *>(instance(execute_data))->*method)();
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, void(T::*method)(Parameters &params) const>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         (static_cast<T *>(instance(execute_data))->*method)(params);
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, Variant(T::*method)() const>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         Variant result = (static_cast<T *>(instance(execute_data))->*method)();
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <typename T, Variant(T::*method)(Parameters &params) const>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         Variant result = (static_cast<T *>(instance(execute_data))->*method)(params);
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }   
   
   template <void(*callable)()>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         callable();
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <Variant(*callable)()>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         Variant result = callable();
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <void(*callable)(Parameters &parameters)>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         callable(params);
         yield(return_value, nullptr);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
   
   template <Variant(*callable)(Parameters &parameters)>
   static void invoke(zend_execute_data *execute_data, zval *return_value)
   {
      try {
         // check invoke arguments
         if (!checkInvokeArguments(execute_data, return_value)) {
            return;
         }
         Parameters params = retrieveParameters(execute_data);
         Variant result = callable(params);
         yield(return_value, result);
      } catch (Exception &exception) {
         handle(exception);
      }
   }
};

} // vm
} // zapi

#endif // ZAPI_VM_INVOKE_BRIDGE_H