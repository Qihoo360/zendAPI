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

#ifndef ZAPI_DS_CALLABLE_VARIANT_H
#define ZAPI_DS_CALLABLE_VARIANT_H

#include "zapi/ds/Variant.h"
#include "zapi/stdext/TypeTraits.h"

namespace zapi
{
namespace ds
{

// define self php val struct
// this for save function in zval

template <typename CallableType>
struct zapi_func_value
{
   zend_refcounted_h gc;
   std::function<CallableType> func;
};

class ZAPI_DECL_EXPORT CallableVariant final : public Variant
{
public:
   template <typename CallableType,
             typename DecayCallableType = typename std::decay<CallableType>::type,
             typename = typename std::enable_if<zapi::stdext::is_function_pointer<DecayCallableType>::value>::type>
   CallableVariant(CallableType callable);
   Variant operator ()() const;
   template <typename ...Args>
   Variant operator ()(Args&&... args);
   virtual ~CallableVariant();
protected:
   Variant exec(int argc, Variant *argv) const;
};

template <typename ...Args>
Variant CallableVariant::operator ()(Args&&... args)
{
   Variant vargs[] = { static_cast<Variant>(args)... };
   return exec(sizeof...(Args), vargs);
}

template <typename CallableType,
          typename DecayCallableType,
          typename>
CallableVariant::CallableVariant(CallableType callable)
{
   zval *self = getUnDerefZvalPtr();
   self->value.ptr = reinterpret_cast<void *>(new zapi_func_value<typename std::remove_pointer<CallableType>::type>());
   Z_TYPE_INFO_P(self) = IS_PTR;
}


} // ds
} // zapi

#endif // ZAPI_DS_CALLABLE_VARIANT_H
