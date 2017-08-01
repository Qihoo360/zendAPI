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
// Created by zzu_softboy on 2017/08/01.

#include "zapi/lang/Method.h"
#include "zapi/vm/internal/CallablePrivate.h"

namespace zapi
{
namespace lang
{

namespace internal
{

using zapi::vm::internal::CallablePrivate;

class MethodPrivate : public CallablePrivate
{
public:
   void initialize(zend_function_entry *entry, const std::string &className);
   int m_type;
   int m_flags;
   union {
      zapi::MethodCallback0 methodCallback0;
      zapi::MethodCallback1 methodCallback1;
      zapi::MethodCallback2 methodCallback2;
      zapi::MethodCallback3 methodCallback3;
      zapi::MethodCallback4 methodCallback4;
      zapi::MethodCallback5 methodCallback5;
      zapi::MethodCallback6 methodCallback6;
      zapi::MethodCallback7 methodCallback7;
      // add native signature
      zapi::NativeCallback0 methodCallback8;
      zapi::NativeCallback1 methodCallback9;
      zapi::NativeCallback2 methodCallback10;
      zapi::NativeCallback3 methodCallback11;
   } m_callback;
};

} // internal



} // lang
} // zapi