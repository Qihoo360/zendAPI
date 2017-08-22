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
// Created by zzu_softboy on 2017/08/03.

#ifndef ZAPI_PROTOCOL_ABSTRACT_ITERATOR_H
#define ZAPI_PROTOCOL_ABSTRACT_ITERATOR_H

#include "zapi/ds/Variant.h"

namespace zapi
{

// forward declare with namespace
namespace lang
{
class StdClass;
} // lang

namespace protocol
{

namespace internal
{
class AbstractIteratorPrivate;
} // internal

using zapi::lang::StdClass;
using zapi::ds::Variant;
using internal::AbstractIteratorPrivate;

class ZAPI_DECL_EXPORT AbstractIterator
{
public:
   AbstractIterator(StdClass *nativeObject);
   virtual ~AbstractIterator();
   
   virtual bool valid() = 0;
   virtual Variant current() = 0;
   virtual Variant key() = 0;
   virtual void next() = 0;
   virtual void rewind() = 0;
   
protected:
   ZAPI_DECLARE_PRIVATE(AbstractIterator)
   std::unique_ptr<AbstractIteratorPrivate> m_implPtr;
};

} // protocol
} // zapi

#endif // ZAPI_PROTOCOL_ABSTRACT_ITERATOR_H