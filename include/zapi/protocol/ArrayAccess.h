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
// Created by zzu_softboy on 2017/08/23.

#ifndef ZAPI_PROTOCOL_ARRAY_ACCESS_H
#define ZAPI_PROTOCOL_ARRAY_ACCESS_H

#include "zapi/Global.h"

namespace zapi
{

// forward declare with namespace
namespace ds
{
class Variant;
} // ds

namespace protocol
{

using zapi::ds::Variant;

class ZAPI_DECL_EXPORT ArrayAccess
{
public:
   virtual bool offsetExists(Variant offset) = 0;
   virtual void offsetSet(Variant offset, Variant value) = 0;
   virtual Variant offsetGet(Variant offset) = 0;
   virtual void offsetUnset(Variant offset) = 0;
   virtual ~ArrayAccess();
};


} // protocol
} // zapi


#endif // ZAPI_PROTOCOL_ARRAY_ACCESS_H
