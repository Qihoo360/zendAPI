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
// Created by softboy on 2017/08/08.

#ifndef ZAPI_DS_INTERNAL_ARRAY_VARIANT_H
#define ZAPI_DS_INTERNAL_ARRAY_VARIANT_H

#include <utility>
#include <string>
#include <list>
#include <type_traits>

#include "zapi/ds/Variant.h"
#include "zapi/ds/ArrayItemProxy.h"

namespace zapi
{
namespace ds
{

class NumericVariant;
class BoolVarint;
class DoubleVariant;
class StringVariant;

class ZAPI_DECL_EXPORT ArrayVariant final : public Variant
{
public:
   using IndexType = uint32_t;
   using SizeType = uint32_t;
   using KeyType = std::pair<zapi_ulong, std::shared_ptr<std::string>>;
   using DifferenceType = zapi_ptrdiff;
   using ValueType = Variant;
   // forward declare
   class Iterator;
   class ConstIterator;
public:
   ArrayVariant();
   ArrayVariant(const ArrayVariant &other);
   ArrayVariant(ArrayVariant &&other) ZAPI_DECL_NOEXCEPT;
   ArrayVariant(const Variant &other);
   ArrayVariant(Variant &&other) ZAPI_DECL_NOEXCEPT;
   // operators
   ArrayItemProxy operator [](zapi_ulong index);
   template <typename T, 
             typename Selector = typename std::enable_if<std::is_integral<T>::value>::type>
   ArrayItemProxy operator [](T index);
   ArrayItemProxy operator [](const std::string &key);
   ArrayItemProxy operator [](const char *key);
   // modify methods
   Iterator insert(zapi_ulong index, const Variant &value);
   Iterator insert(zapi_ulong index, Variant &&value);
   Iterator insert(const std::string &key, const Variant &value);
   Iterator insert(const std::string &key, Variant &&value);
   Iterator append(const Variant &value);
   Iterator append(Variant &&value);
   void clear() ZAPI_DECL_NOEXCEPT;
   bool remove(zapi_ulong index) ZAPI_DECL_NOEXCEPT;
   bool remove(const std::string &key) ZAPI_DECL_NOEXCEPT;
   Iterator erase(ConstIterator &iter);
   Iterator erase(Iterator &iter);
   // info access
   bool isEmpty() const ZAPI_DECL_NOEXCEPT;
   bool isNull() const ZAPI_DECL_NOEXCEPT;
   SizeType getSize() const ZAPI_DECL_NOEXCEPT;
   SizeType getCapacity() const ZAPI_DECL_NOEXCEPT;
   SizeType count() const ZAPI_DECL_NOEXCEPT;
   Variant getValue(zapi_ulong index) const;
   Variant getValue(const std::string &key) const;
   bool contains(zapi_ulong index) const;
   bool contains(const std::string &key) const;
   zapi_long getNextInsertIndex() const;
   std::list<KeyType> getKeys() const;
   std::list<KeyType> getKeys(const Variant &value, bool strict = false) const;
   std::list<Variant> getValues() const;
   Iterator find(zapi_ulong index);
   Iterator find(const std::string &key);
   ConstIterator find(zapi_ulong index) const;
   ConstIterator find(const std::string &key) const;
   // iterators
   Iterator begin() ZAPI_DECL_NOEXCEPT;
   ConstIterator begin() const ZAPI_DECL_NOEXCEPT;
   ConstIterator cbegin() const ZAPI_DECL_NOEXCEPT;
   Iterator end() ZAPI_DECL_NOEXCEPT;
   ConstIterator end() const ZAPI_DECL_NOEXCEPT;
   ConstIterator cend() const ZAPI_DECL_NOEXCEPT;
   ~ArrayVariant();
   
   // iterator class
   class Iterator
   {
   public:
      using IteratorCategory = std::bidirectional_iterator_tag;
      using DifferenceType = zapi_ptrdiff;
      using ValueType = Variant;
      using ZvalPointer = zval *;
      using ZvalReference = zval &;
   public:
      Iterator(const Iterator &other);
      Iterator(Iterator &&other) ZAPI_DECL_NOEXCEPT;
      ~Iterator();
      Variant getValue() const;
      ZvalReference getZval() const;
      ZvalPointer getZvalPtr() const;
      KeyType getKey() const;
      HashPosition getCurrentPos() const;
      // operators
      Iterator &operator =(const Iterator &other);
      Iterator &operator =(Iterator &&other) ZAPI_DECL_NOEXCEPT;
      ZvalReference operator *();
      ZvalPointer operator->();
      bool operator ==(const Iterator &other);
      bool operator !=(const Iterator &other);
      Iterator &operator ++();
      Iterator operator ++(int);
      Iterator &operator --();
      Iterator operator --(int);
      Iterator operator +(int32_t step) const;
      Iterator operator -(int32_t step) const;
      Iterator &operator +=(int32_t step);
      Iterator &operator -=(int32_t step);
   protected:
      Iterator(_zend_array *array, HashPosition *pos = nullptr);
   protected:
      friend class ArrayVariant;
      _zend_array *m_array;
      uint32_t m_idx = -1; // invalid idx
      bool m_isEnd;
   };
   
   class ConstIterator : public Iterator
   {
   public:
      using IteratorCategory = std::bidirectional_iterator_tag;
      using DifferenceType = zapi_ptrdiff;
      using ValueType = Variant;
      using ZvalPointer = const zval *;
      using ZvalReference = const zval &;
   public:
      ConstIterator(const ConstIterator &other);
      ConstIterator(ConstIterator &&other) ZAPI_DECL_NOEXCEPT;
      ~ConstIterator();
      const Variant getValue() const;
      ZvalReference getZval() const;
      ZvalPointer getZvalPtr() const;
      const KeyType getKey() const;
      // operators
      ConstIterator &operator =(const ConstIterator &other);
      ConstIterator &operator =(ConstIterator &&other) ZAPI_DECL_NOEXCEPT;
      ZvalReference operator *();
      ZvalPointer operator->();
      bool operator ==(const ConstIterator &other) const;
      bool operator !=(const ConstIterator &other) const;
      ConstIterator &operator ++();
      ConstIterator operator ++(int);
      ConstIterator &operator --();
      ConstIterator operator --(int);
      ConstIterator operator +(int32_t step) const;
      ConstIterator operator -(int32_t step) const;
      ConstIterator &operator +=(int32_t step);
      ConstIterator &operator -=(int32_t step);
   protected:
      ConstIterator(_zend_array *array, HashPosition *pos = nullptr);
   protected:
      friend class ArrayVariant;
   };
   
protected:
   _zend_array *getZendArrayPtr() const ZAPI_DECL_NOEXCEPT;
   _zend_array &getZendArray() const ZAPI_DECL_NOEXCEPT;
   uint32_t calculateIdxFromZval(zval *val) const ZAPI_DECL_NOEXCEPT;
   uint32_t findArrayIdx(const std::string &key) const ZAPI_DECL_NOEXCEPT;
   uint32_t findArrayIdx(zapi_ulong index) const ZAPI_DECL_NOEXCEPT;
protected:
   friend class ArrayItemProxy;
   friend class Iterator;
   friend class ConstIterator;
};

template <typename T, typename Selector>
ArrayItemProxy ArrayVariant::operator [](T index)
{
   if (index < 0) {
      index = 0;
   }
   return operator [](static_cast<zapi_ulong>(index));
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_ARRAY_VARIANT_H