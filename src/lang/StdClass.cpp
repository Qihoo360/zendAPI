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
// Created by zzu_softboy on 2017/06/08.

#include "zapi/ds/Variant.h"
#include "zapi/ds/ArrayVariant.h"
#include "zapi/lang/StdClass.h"
#include "zapi/lang/internal/StdClassPrivate.h"
#include "zapi/kernel/NotImplemented.h"
#include "zapi/utils/CommonFuncs.h"

namespace zapi
{
namespace lang
{

namespace internal
{

StdClassPrivate::StdClassPrivate()
{}

} // internal

using zapi::kernel::NotImplemented;
using zapi::lang::internal::StdClassPrivate;

StdClass::StdClass()
   : m_implPtr(new StdClassPrivate)
{}

StdClass::~StdClass()
{}

StdClass::StdClass(const StdClass &object)
   : m_implPtr(new StdClassPrivate)
{}

/**
 * Overridable method that is called right before an object is destructed
 */
void StdClass::__destruct() const
{
   throw NotImplemented();
}

void StdClass::__clone()
{}

/**
 * Overridable method that is called to check if a property is set
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param  key
 * @return bool
 */
bool StdClass::__isset(const std::string &key) const
{
   throw NotImplemented();
}

/**
 * Overridable method that is called to set a new property
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param key
 * @param value
 */
void StdClass::__set(const std::string &key, const Variant &value)
{
   throw NotImplemented();
}

/**
 * Retrieve a property
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param  key
 * @return value
 */
Variant StdClass::__get(const std::string &key) const
{
   throw NotImplemented();
}

/**
 * Remove a member
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param key
 */
void StdClass::__unset(const std::string &key)
{
   throw NotImplemented();
}

/**
 * Call a method
 *
 * This method is called when a method is called from the PHP script that
 * was not explicitly defined. You can use this to catch variable method
 * names, or to support all thinkable method names.
 *
 * @param  method      Name of the method that was called
 * @param  params      The parameters that were passed to the function
 * @return Value       The return value
 */
Variant StdClass::__call(const std::string &method, Parameters &params) const
{
   throw NotImplemented();
}

/**
 * Call the class as if it was a function
 *
 * This method is called when a an object is used with () operators:
 * $object(). You can override this method to make objects callable.
 *
 * @param  params      The parameters that were passed to the function
 * @return Value       The return value
 */
Variant StdClass::__invoke(Parameters &params) const
{
   throw NotImplemented();
}

/**
 * Cast the object to a string
 *
 * This method is called when an object is casted to a string, or when
 * it is used in a string context
 *
 * @return Value       The object as a string
 */
Variant StdClass::__toString() const
{
   throw NotImplemented();
}

/**
 * Cast the object to an integer
 *
 * This method is called when an object is casted to an integer, or when
 * it is used in an integer context
 *
 * @return int         Integer value
 */
Variant StdClass::__toInteger() const
{
   throw NotImplemented();
}

/**
 * Cast the object to a float
 *
 * This method is called when an object is casted to a float, or when it
 * is used in a float context
 *
 * @return double      Floating point value
 */
Variant StdClass::__toDouble() const
{
   throw NotImplemented();
}

/**
 * Cast the object to a boolean
 *
 * This method is called when an object is casted to a bool, or when it
 * is used in a boolean context
 *
 * @return bool
 */
Variant StdClass::__toBool() const
{
   throw NotImplemented();
}

/**
 * Compare the object with a different object
 *
 * Check how a different object compares to this object
 *
 * @param  that        Object to compare with
 * @return int
 */
int StdClass::__compare(const StdClass &object) const
{
   throw NotImplemented();
}

ArrayVariant StdClass::__debugInfo() const
{
   throw NotImplemented();
}

ObjectVariant *StdClass::getObjectZvalPtr() const
{
   if (!m_implPtr->m_objVariant) {
      auto self = this;
      m_implPtr->m_objVariant.reset(new ObjectVariant(const_cast<StdClass *>(self)));
   }
   return m_implPtr->m_objVariant.get();
}

ObjectVariant *StdClass::getObjectZvalPtr()
{
   return const_cast<const StdClass *>(this)->getObjectZvalPtr();
}

zval *StdClass::doCallParent(const char *name, const int argc, Variant *argv, zval *retvalPtr) const
{
   zend_object *object = m_implPtr->m_zendObject;
   if (!object) {
      zapi::error << "invoke StdClass::doCallParent on unbinded nativeObject" << std::endl;
   }
   int result;
   zend_fcall_info fci;
   std::memset(&fci, 0, sizeof(fci));
   zval retval;
   HashTable *funcTable;
   std::unique_ptr<zval[]> params(new zval[argc]);
   zval *curArgPtr = nullptr;
   for (int i = 0; i < argc; i++) {
      params[i] = *argv[i].getUnDerefZvalPtr();
      curArgPtr = &params[i];
      if (Z_TYPE_P(curArgPtr) == IS_REFERENCE && Z_REFCOUNTED_P(Z_REFVAL_P(curArgPtr))) {
          Z_TRY_ADDREF_P(&params[i]); // _call_user_function_ex free call stack will decrease 1
      }
   }
   fci.size = sizeof(fci);
   fci.object = object;
   ZVAL_STRINGL(&fci.function_name, name, std::strlen(name));
   zapi::utils::str_tolower(Z_STRVAL(fci.function_name), Z_STRLEN(fci.function_name));
   fci.retval = retvalPtr ? retvalPtr : &retval;
   fci.param_count = argc;
   fci.params = params.get();
   fci.no_separation = 1;
   
   // setup cache
   zend_fcall_info_cache fcic;
   fcic.initialized = 1;
   zend_class_entry *parentClassType = object->ce->parent;
   if (parentClassType) {
      funcTable = &parentClassType->function_table;
   } else {
      funcTable = EG(function_table);
   }
   if ((fcic.function_handler = reinterpret_cast<zend_function *>(zend_hash_find_ptr(funcTable, Z_STR(fci.function_name)))) == nullptr) {
      /* error at c-level */
      zend_error(E_CORE_ERROR, "Couldn't find implementation for method %s%s%s", 
                 parentClassType ? ZSTR_VAL(parentClassType->name) : "", parentClassType ? "::" : "", name);
   }
   fcic.calling_scope = parentClassType;
   if (object) {
      fcic.called_scope = object->ce;
   } else {
      zend_class_entry *calledScope = zend_get_called_scope(EG(current_execute_data));
      
      if (parentClassType &&
          (!calledScope ||
           !instanceof_function(calledScope, parentClassType))) {
         fcic.called_scope = parentClassType;
      } else {
         fcic.called_scope = calledScope;
      }
   }
   fcic.object = object ? object : nullptr;
   result = zend_call_function(&fci, &fcic);
   zval_ptr_dtor(&fci.function_name);
   if (result == ZAPI_FAILURE) {
      /* error at c-level */
      if (!parentClassType) {
         parentClassType = object ? object->ce : nullptr;
      }
      if (!EG(exception)) {
         zend_error(E_CORE_ERROR, "Couldn't execute method %s%s%s", parentClassType 
                    ? ZSTR_VAL(parentClassType->name) : "", parentClassType ? "::" : "", name);
      }
   }
   if (!retvalPtr) {
      zval_ptr_dtor(&retval);
      return nullptr;
   }
   return retvalPtr;
}

} // lang
} // zapi
