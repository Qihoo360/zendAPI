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

#ifndef ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H

#include <utility>
#include <string>
#include <type_traits>

#include "zapi/ds/Variant.h"

namespace zapi
{
namespace ds
{

class NumericVariant;
class BoolVarint;
class DoubleVarint;
class StringVariant;
class ArrayItemProxy;

class ZAPI_DECL_EXPORT ArrayVariant : public Variant
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
   // transform constructors
   ArrayVariant(const NumericVariant &value);
   ArrayVariant(const BoolVarint &value);
   ArrayVariant(const DoubleVarint &value);
   ArrayVariant(const StringVariant &value);
   ArrayVariant(const std::string &value);
   ArrayVariant(const char *value);
   template <typename ArithmeticType, 
             typename Selector = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type>
   ArrayVariant(ArithmeticType value);
   // operators
   ArrayItemProxy operator [](zapi_ulong index);
   ArrayItemProxy operator [](const std::string &key);
   Variant operator [](zapi_ulong index) const;
   Variant operator [](const std::string &key) const;
   
   // modify methods
   void clear();
   Variant pop();
   ArrayVariant &push(const Variant &value);
   Iterator insert(zapi_ulong index, const Variant &value);
   Iterator insert(zapi_ulong index, Variant &&value);
   Iterator insert(const std::string &key, const Variant &value);
   Iterator insert(const std::string &key, Variant &&value);
   Iterator append(const Variant &value);
   Iterator append(Variant &&value);
   // info access
   bool isEmpty() const ZAPI_DECL_NOEXCEPT;
   bool isNull() const ZAPI_DECL_NOEXCEPT;
   SizeType getSize() const ZAPI_DECL_NOEXCEPT;
   SizeType count() const ZAPI_DECL_NOEXCEPT;
   Variant getValue(zapi_long index) const;
   Variant getValue(const std::string &key) const;
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
   void deployCopyOnWrite();
   _zend_array *getZendArrayPtr() const ZAPI_DECL_NOEXCEPT;
   _zend_array &getZendArray() const ZAPI_DECL_NOEXCEPT;
   uint32_t calculateIdxFromZval(zval *val) const ZAPI_DECL_NOEXCEPT;
protected:
   friend class ArrayItemProxy;
};

template <typename ArithmeticType, typename Selector>
ArrayVariant::ArrayVariant(ArithmeticType value)
{
   
}

} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_ARRAY_VARIANT_PRIVATE_H