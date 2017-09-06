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

#include "zapi/lang/StdClass.h"
#include "zapi/lang/internal/StdClassPrivate.h"
#include "zapi/vm/internal/AbstractClassPrivate.h"
#include "zapi/vm/ObjectBinder.h"
#include "zapi/ds/ObjectVariant.h"
#include "zapi/utils/CommonFuncs.h"
#include "zapi/kernel/Exception.h"
#include "zapi/kernel/OrigException.h"
#include "zapi/kernel/FatalError.h"
#include "php/Zend/zend_closures.h"

using zapi::ds::Variant;
using zapi::kernel::Exception;
using zapi::kernel::OrigException;

namespace
{
Variant do_execute(const zval *object, zval *method, int argc, zval *argv)
{
   zval retval;
   zend_object *oldException = EG(exception);
   if (ZAPI_SUCCESS != call_user_function_ex(CG(function_table), const_cast<zval *>(object), method, &retval,
                                             argc, argv, 1, nullptr)) {
      std::string msg("Invalid call to ");
      msg.append(Z_STRVAL_P(method), Z_STRLEN_P(method));
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

using GuardStrType = std::unique_ptr<zend_string, std::function<void(zend_string *)>>;
using zapi::vm::ObjectBinder;
using zapi::vm::internal::AbstractClassPrivate;

ObjectVariant::ObjectVariant()
{
   convert_to_object(getUnDerefZvalPtr());
}

ObjectVariant::ObjectVariant(const Variant &other)
{
   zval *from = const_cast<zval *>(other.getZvalPtr());
   zval *self = getUnDerefZvalPtr();
   if (other.getType() == Type::Object) {
      ZVAL_COPY(self, from);
   } else {
      zval temp;
      // will increase 1 to gc refcount
      ZVAL_DUP(&temp, from);
      // will decrease 1 to gc refcount
      convert_to_object(&temp);
      ZVAL_COPY_VALUE(self, &temp);
   }
}

ObjectVariant::ObjectVariant(const ObjectVariant &other)
   : Variant(other)
{}

ObjectVariant::ObjectVariant(const std::string &className, std::shared_ptr<StdClass> nativeObject)
{
   zend_object *zobject = nativeObject->m_implPtr->m_zendObject;
   if (!zobject) {
      // new construct
      zend_string *clsName = zend_string_init(className.c_str(), className.length(), 0);
      zend_class_entry *entry = zend_fetch_class(clsName, ZEND_FETCH_CLASS_SILENT);
      zend_string_free(clsName);
      if (!entry) {
         throw zapi::kernel::FatalError(std::string("Unknown class name ") + className);
      }
      ObjectBinder *binder = new ObjectBinder(entry, nativeObject,
                                              AbstractClassPrivate::getObjectHandlers(entry), 0);
      zobject = binder->getZendObject();
   }
   zval *self = getUnDerefZvalPtr();
   ZVAL_OBJ(self, zobject);
   Z_ADDREF_P(self);
}

ObjectVariant::ObjectVariant(Variant &&other)
   : Variant(std::move(other))
{
   if (getUnDerefType() != Type::Object) {
      convert_to_object(getUnDerefZvalPtr());
   }
}

ObjectVariant::ObjectVariant(ObjectVariant &&other) ZAPI_DECL_NOEXCEPT
   : Variant(std::move(other))
{}

ObjectVariant &ObjectVariant::operator =(const ObjectVariant &other)
{
   if (this != &other) {
      Variant::operator =(const_cast<zval *>(other.getZvalPtr()));
   }
   return *this;
}

ObjectVariant &ObjectVariant::operator =(const Variant &other)
{
   if (this != &other) {
      zval *self = getZvalPtr();
      zval *from = const_cast<zval *>(other.getZvalPtr());
      // need set gc info
      if (other.getType() == Type::Object) {
         // standard copy
         Variant::operator =(from);
      } else {
         zval temp;
         // will increase 1 to gc refcount
         ZVAL_DUP(&temp, from);
         // will decrease 1 to gc refcount
         convert_to_object(&temp);
         zval_dtor(self);
         ZVAL_COPY_VALUE(self, &temp);
      }
   }
   return *this;
}

ObjectVariant &ObjectVariant::operator =(ObjectVariant &&other) ZAPI_DECL_NOEXCEPT
{
   assert(this != &other);
   m_implPtr = std::move(other.m_implPtr);
   return *this;
}

ObjectVariant &ObjectVariant::operator =(Variant &&other)
{
   assert(this != &other);
   m_implPtr = std::move(other.m_implPtr);
   if (getUnDerefType() != Type::Object) {
      convert_to_object(getUnDerefZvalPtr());
   }
   return *this;
}

ObjectVariant &ObjectVariant::setProperty(const std::string &name, const Variant &value)
{
   zval *self = getUnDerefZvalPtr();
   zend_update_property(Z_OBJCE_P(self), self, name.c_str(), name.length(), 
                        const_cast<zval *>(value.getUnDerefZvalPtr()));
   return *this;
}

Variant ObjectVariant::getProperty(const std::string &name)
{
   zval *self = getUnDerefZvalPtr();
   zval retval;
   return zend_read_property(Z_OBJCE_P(self), self, name.c_str(), name.length(), 1, &retval);
}

ObjectVariant &ObjectVariant::setStaticProperty(const std::string &name, const Variant &value)
{
   zend_update_static_property(Z_OBJCE_P(getUnDerefZvalPtr()), name.c_str(), name.length(), 
                        const_cast<zval *>(value.getUnDerefZvalPtr()));
   return *this;
}

Variant ObjectVariant::getStaticProperty(const std::string &name)
{
   return zend_read_static_property(Z_OBJCE_P(getUnDerefZvalPtr()), name.c_str(), name.length(), 1);
}

bool ObjectVariant::hasProperty(const std::string &name)
{
   int value = 0;
   zval *self = getUnDerefZvalPtr();
   zend_class_entry *scope = Z_OBJCE_P(self);
   zend_class_entry *old_scope = EG(fake_scope);
   EG(fake_scope) = scope;
   if (!Z_OBJ_HT_P(self)->has_property) {
      zend_error_noreturn(E_CORE_ERROR, "Property %s of class %s cannot be read", name.c_str(), ZSTR_VAL(Z_OBJCE_P(self)->name));
   }
   zval nameZval;
   ZVAL_STRINGL(&nameZval, name.c_str(), name.length());
   // @TODO if I have time, I will find this cache, now pass nullptr
   value = Z_OBJ_HT_P(self)->has_property(self, &nameZval, 2, nullptr);
   zval_dtor(&nameZval);
   EG(fake_scope) = old_scope;
   return 1 == value;
}

bool ObjectVariant::isCallable(const char *name) const
{
   if (Type::Object != getType()) {
      return false;
   }
   zval *self = const_cast<zval *>(getZvalPtr());
   zend_class_entry *classEntry = Z_OBJCE_P(self);
   // TODO watch the resource release
   GuardStrType methodName(zend_string_init(name, std::strlen(name), 0),
                           zapi::utils::std_zend_string_force_deleter);
   zapi::utils::str_tolower(ZSTR_VAL(methodName.get()));
   if (zend_hash_exists(&classEntry->function_table, methodName.get())) {
      return true;
   }
   if (nullptr == Z_OBJ_HT_P(self)->get_method) {
      return false;
   }
   union _zend_function *func = Z_OBJ_HT_P(self)->get_method(&Z_OBJ_P(self), methodName.get(), nullptr);
   if (nullptr == func) {
      return false;
   }
   if (!(func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) {
      return true;
   }
   bool result = func->common.scope == zend_ce_closure &&
         zend_string_equals_literal(methodName.get(), ZEND_INVOKE_FUNC_NAME);
   zend_string_release(func->common.function_name);
   zend_free_trampoline(func);
   return result;
}

Variant ObjectVariant::call(const char *name)
{
   return const_cast<const ObjectVariant&>(*this).call(name);
}

Variant ObjectVariant::call(const char *name) const
{
   Variant method(name);
   return do_execute(getZvalPtr(), method.getZvalPtr(), 0, nullptr);
}

bool ObjectVariant::instanceOf(const char *className, size_t size) const
{
   zend_class_entry *thisClsEntry = Z_OBJCE_P(getZvalPtr());
   if (!thisClsEntry) {
      return false;
   }
   GuardStrType clsName(zend_string_init(className, size, 0),
                        zapi::utils::std_zend_string_force_deleter);
   zend_class_entry *clsEntry = zend_lookup_class_ex(clsName.get(), nullptr, 0);
   if (!clsEntry) {
      return false;
   }
   return instanceof_function(thisClsEntry, clsEntry);
}

bool ObjectVariant::instanceOf(const char *className) const
{
   return instanceOf(className, std::strlen(className));
}

bool ObjectVariant::instanceOf(const std::string &className) const
{
   return instanceOf(className.c_str(), className.length());
}

bool ObjectVariant::derivedFrom(const char *className, size_t size) const
{
   zend_class_entry *thisClsEntry = Z_OBJCE_P(getZvalPtr());
   if (!thisClsEntry) {
      return false;
   }
   GuardStrType clsName(zend_string_init(className, size, 0),
                        zapi::utils::std_zend_string_force_deleter);
   zend_class_entry *clsEntry = zend_lookup_class_ex(clsName.get(), nullptr, 0);
   if (!clsEntry) {
      return false;
   }
   if (thisClsEntry == clsEntry) {
      return false;
   }
   return instanceof_function(thisClsEntry, clsEntry);
}

bool ObjectVariant::derivedFrom(const char *className) const
{
   return derivedFrom(className, std::strlen(className));
}

bool ObjectVariant::derivedFrom(const std::string &className) const
{
   return derivedFrom(className.c_str(), className.length());
}

Variant ObjectVariant::exec(const char *name, int argc, Variant *argv)
{
   return const_cast<const ObjectVariant&>(*this).exec(name, argc, argv);
}

Variant ObjectVariant::exec(const char *name, int argc, Variant *argv) const
{
   Variant methodName(name);
   zval params[argc];
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].getZvalPtr();
   }
   return do_execute(getZvalPtr(), methodName.getZvalPtr(), argc, params);
}

} // ds
} // zapi
