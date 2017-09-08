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
// Created by zzu_softboy on 06/06/2017.

#ifndef ZAPI_LANG_STDCLASS_H
#define ZAPI_LANG_STDCLASS_H

#include "zapi/Global.h"
#include "zapi/ds/Variant.h"

namespace zapi
{

// forware declare with namespace
namespace ds
{
class Variant;
class ObjectVariant;
} // ds

namespace vm
{
class ObjectBinder;
} // vm

namespace lang
{
namespace internal
{
class StdClassPrivate;
} // internal
} // lang

} // zapi
// end forware declare

namespace zapi
{
namespace lang
{

using zapi::lang::internal::StdClassPrivate;
using zapi::ds::Variant;
using zapi::ds::ObjectVariant;
using zapi::vm::ObjectBinder;
class Parameters;

class ZAPI_DECL_EXPORT StdClass
{
protected:
   /**
    * Constructor
    */
   StdClass();

   /**
    * Copy constructor
    *
    * This copy constructor is explicitly defined to make sure that the
    * copied object does not already have an implementation in the zend engine.
    * Otherwise the copied object has the same object handle as the original
    * object.
    *
    * @param object
    */
   StdClass(const StdClass &object);

public:
   virtual ~StdClass();
   /**
    * Get access to a property by name using the [] operator
    * @param  string
    * @return Value
    */
   Variant operator[](const char *name) const;

   /**
    * Alternative way to access a property using the [] operator
    * @param  string
    * @return Value
    */
   Variant operator[](const std::string &name) const;
   /**
    * Retrieve a property by name
    * @param  string
    * @return Value
    */
   Variant property(const char *name) const;

   /**
    * Retrieve a property by name
    * @param  string
    * @return Value
    */
   Variant property(const std::string &name) const;

   /**
    * Overridable method that is called right before an object is destructed
    */
   void __destruct() const;

   /**
    * Overridable method that is called right after an object is cloned
    *
    * The default implementation does nothing
    */
   void __clone();

   /**
    * Overridable method that is called to check if a property is set
    *
    * The default implementation does nothing, and the script will fall back
    * to accessing the regular object properties
    *
    * @param  key
    * @return bool
    */
   bool __isset(const std::string &key) const;

   /**
    * Overridable method that is called to set a new property
    *
    * The default implementation does nothing, and the script will fall back
    * to accessing the regular object properties
    *
    * @param  key
    * @param  value
    */
   void __set(const std::string &key, const Variant &value);

   /**
    * Retrieve a property
    *
    * The default implementation does nothing, and the script will fall back
    * to accessing the regular object properties
    *
    * @param  key
    * @return value
    */
   Variant __get(const std::string &key) const;

   /**
    * Remove a member
    *
    * The default implementation does nothing, and the script will fall back
    * to accessing the regular object properties
    *
    * @param key
    */
   void __unset(const std::string &key);

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
   Variant __call(const std::string &method, Parameters &params) const;

   /**
    * Call the class as if it was a function
    *
    * This method is called when a an object is used with () operators:
    * $object(). You can override this method to make objects callable.
    *
    * @param  params      The parameters that were passed to the function
    * @return Value       The return value
    */
   Variant __invoke(Parameters &params) const;

   /**
    * Cast the object to a string
    *
    * This method is called when an object is casted to a string, or when
    * it is used in a string context
    *
    * @return Value       The object as a string
    */
   Variant __toString() const;

   /**
    * Cast the object to an integer
    *
    * This method is called when an object is casted to an integer, or when
    * it is used in an integer context
    *
    * @return int Integer value
    */
   Variant __toInteger() const;

   /**
    * Cast the object to a float
    *
    * This method is called when an object is casted to a float, or when it
    * is used in a float context
    *
    * @return double      Floating point value
    */
   Variant __toDouble() const;

   /**
    * Cast the object to a boolean
    *
    * This method is called when an object is casted to a bool, or when it
    * is used in a boolean context
    *
    * @return bool
    */
   Variant __toBool() const;

   /**
    *  Compare the object with a different object
    *
    *  Check how a different object compares to this object
    *
    *  @param  that        Object to compare with
    *  @return int
    */
   int __compare(const StdClass &object) const;
protected:
   template <typename ...Args>
   Variant callParent(const char *name, Args&&... args);
   template <typename ...Args>
   Variant callParent(const char *name, Args&&... args) const;
private:
   zval *doCallParent(const char *name, const int argc, Variant *argv, zval *retval) const;
protected:
   ZAPI_DECLARE_PRIVATE(StdClass)
   friend class Variant;// for Variant(const StdClass &stdClass);
   friend class ObjectBinder; // for set zend_object pointer
   friend class ObjectVariant; // for initialze from StdClass instance
   std::unique_ptr<StdClassPrivate> m_implPtr;
};

template <typename ...Args>
Variant StdClass::callParent(const char *name, Args&&... args) const
{
   Variant vargs[] = { Variant(std::forward<Args>(args))... };
   zval retval;
   std::memset(&retval, 0, sizeof(retval));
   doCallParent(name, sizeof...(Args), vargs, &retval);
   Variant resultVarint(retval);
   zval_dtor(&retval);
   return resultVarint;
}

template <typename ...Args>
Variant StdClass::callParent(const char *name, Args&&... args)
{
   return const_cast<const StdClass &>(*this).callParent(name, std::forward<Args>(args)...);
}

} // lang
} // zapi

#endif //ZAPI_LANG_STDCLASS_H
