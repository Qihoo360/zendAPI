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
// Created by zzu_softboy on 2017/08/22.

#ifndef ZAPI_VM_INTERFACES_H
#define ZAPI_VM_INTERFACES_H

#include "zapi/Global.h"

namespace zapi
{

// forward declare with namespace
namespace kernel
{
class AbstractIterator;
} // kernel

namespace ds
{
class Variant;
} // ds

namespace vm
{

using zapi::kernel::AbstractIterator;
using zapi::ds::Variant;

class ZAPI_DECL_EXPORT Countable
{
public:
   virtual zapi_long count() = 0;
};

class ZAPI_DECL_EXPORT Serializable
{
public:
   virtual std::string serialize() = 0;
   virtual void unserialize(const char *input, size_t size) = 0;
};

class ZAPI_DECL_EXPORT Traversable
{
   virtual AbstractIterator *getIterator() = 0;
};

class ZAPI_DECL_EXPORT ArrayAccess
{
public:
   virtual bool offsetExists(const Variant &offset) = 0;
   virtual void offsetSet(const Variant &offset, const Variant &value) = 0;
   virtual Variant offsetGet(const Variant &offset) = 0;
   virtual Variant offsetUnset(const Variant &offset) = 0;
   virtual ~ArrayAccess();
};

} // vm
} // zapi

#endif // ZAPI_VM_INTERFACES_H