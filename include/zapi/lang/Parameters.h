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

#ifndef ZAPI_LANG_PARAMETERS_H
#define ZAPI_LANG_PARAMETERS_H

#include "zapi/Global.h"
#include "zapi/lang/Variant.h"

#include <vector>

namespace zapi
{
namespace lang
{

class StdClass;

/**
 * now this is very bad implemention of parameters class, but it works
 */
class ZAPI_DECL_EXPORT Parameters
{
public:
   using ParamCollectionType = std::vector<Variant>;
   using ValueType = ParamCollectionType::value_type;
   using SizeType = ParamCollectionType::size_type;
   using DifferenceType = ParamCollectionType::difference_type;
   using Reference = ParamCollectionType::reference;
   using ConstReference = ParamCollectionType::const_reference;
   using Pointer = ParamCollectionType::pointer;
   using ConstPointer = ParamCollectionType::const_pointer;
   using Iterator = ParamCollectionType::iterator;
   using ConstIterator = ParamCollectionType::const_iterator;
   using ReverseIterator = ParamCollectionType::reverse_iterator;
   using ConstReverseIterator = ParamCollectionType::const_reverse_iterator;
public:
   Parameters(std::initializer_list<Variant> items)
      : m_data(items)
   {}
   
   Parameters(const Parameters &other)
      : m_data(other.m_data), m_object(other.m_object)
   {}
   
   Parameters(const ParamCollectionType::iterator begin,
              const ParamCollectionType::iterator end)
      : m_data(begin, end)
   {}
   
   Parameters(Parameters &&params) ZAPI_DECL_NOEXCEPT
      : m_data(std::move(params.m_data)), m_object(params.m_object)
   {}
   
   Parameters(StdClass *object) : m_object(object)
   {}
   
   Parameters(zval *thisPtr, uint32_t argc);
   
public:
   
   StdClass *getObject() const
   {
      return m_object;
   }
   
private:
   /**
    *  The base object
    *  @var Base
    */
   StdClass *m_object = nullptr;
   std::vector<Variant> m_data;
};

} // lang
} // zapi

#endif //ZAPI_LANG_PARAMETERS_H
