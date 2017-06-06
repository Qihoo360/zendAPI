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
// Created by softboy on 5/26/17.

#ifndef ZAPI_LANG_VARIANT_H
#define ZAPI_LANG_VARIANT_H

#include "zapi/Global.h"
#include "zapi/vm/Zval.h"
#include "zapi/lang/Type.h"

#include <vector>
#include <map>

namespace zapi
{
namespace lang
{

/**
 * Base class for values that are stored in the Zend engine. One instance
 * of the value class represents a variable that exists in user space in
 * the PHP environment, for example as global variable, local variable
 * inside a function or as a member of an object or an array.
 *
 * A value can be a scalar or a more complicated structure like an object
 * or an array.
 *
 * Internally, the Zend engine works with "zval" objects for this. These "zval"
 * object hold a reference counter and a reference setting. The PHP-CPP Value
 * class takes care of doing this, so all you need to do is use objects of
 * this class.
 *
 * @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */
class ZAPI_DECL_EXPORT Variant
{
public:
   /**
    * Empty constructor (value = NULL)
    */
   Variant();

   /**
    * Constructor for various types
    */
   Variant(std::nullptr_t value);
   Variant(int16_t value);
   Variant(int32_t value);
   Variant(int64_t value);
   Variant(bool value);
   Variant(char value);
   Variant(const std::string &value);
   Variant(const char *value, int length = -1);
   Variant(double value);

   Variant(Type typeValue) : Variant()
   {
      setType(typeValue);
   }

   template <typename T>
   Variant(const std::vector<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         setRaw(i++, item);
      }
   }

#ifdef ZAPI_COMPILER_INITIALIZER_LISTS
   template <typename T>
   Variant(const std::initializer_list<T> &values) : Variant(Type::Array)
   {
      int i = 0;
      for (auto &item : values)
      {
         setRaw(i++, item);
      }
   }
#endif

   /**
    * Constructor from a map (this will create an associative array)
    * @param  value
    */
   template <typename T>
   Variant(const std::map<std::string, T> &values) : Variant(Type::Array)
   {
      for (auto &iter : values)
      {
         setRaw(iter.first.c_str(), iter.first.size(), iter.second);
      }
   }

   /**
    * Change the internal type of the variable
    *
    * @param Type typeValue
    * @return Variant&
    */
   virtual Variant &setType(Type typeValue) &;

   /**
    * Set a certain property without running any checks (you must already know
    * for sure that this is an array, and that the index is not yet in use)
    *
    * @param int index       Index of the property to set
    * @param Variant& value       Value to set
    */
   void setRaw(int index, const Variant &value);

   /**
    * Set a certain property without any checks (you must already know for
    * sure that this is either an object or an array, and that the index is
    * not yet in use)
    *
    * @param  key         Key of the property to set
    * @param  size        Size of the key
    * @param  value       Value to set
    */
   void setRaw(const char *key, int size, const Variant &value);
};

} // lang
} // zapi

#endif //ZAPI_LANG_VARIANT_H
