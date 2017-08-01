// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// Copyright 2013, 2014 Copernica BV
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by softboy on 2017/08/01.

#ifndef ZAPI_LANG_METHOD_H
#define ZAPI_LANG_METHOD_H

#include "zapi/Global.h"
#include "zapi/vm/Callable.h"

namespace zapi
{
namespace lang
{

namespace internal
{
class MethodPrivate;
} // internal

using internal::MethodPrivate;

class ZAPI_DECL_EXPORT Method : public zapi::vm::Callable
{
public:
   Method(const char *name, zapi::ZendCallable callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback0 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback1 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback2 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback3 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback4 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback5 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback6 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback7 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback8 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback9 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback10 callback, int flags, const Arguments &args);
   Method(const char *name, zapi::MethodCallback11 callback, int flags, const Arguments &args);
   Method(const char *name, int flags, const Arguments &args);
   Method(const Method &other);
   Method(Method &&other);
   virtual ~Method();
   virtual Variant invoke(Parameters &parameters);
private:
   ZAPI_DECLARE_PRIVATE(Method)
};

} // lang
} // zapi

#endif // ZAPI_LANG_METHOD_H