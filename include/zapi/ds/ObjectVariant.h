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

#ifndef ZAPI_DS_OBJECT_VARIANT_H
#define ZAPI_DS_OBJECT_VARIANT_H

#include "zapi/ds/Variant.h"

namespace zapi
{
namespace lang
{
class StdClass;
} // lang
} // zapi

namespace zapi
{
namespace ds
{

using zapi::lang::StdClass;

class ZAPI_DECL_EXPORT ObjectVariant final : public Variant
{
public:
   ObjectVariant();
   ObjectVariant(const std::string &className, std::shared_ptr<StdClass> nativeObject);
   ObjectVariant(const Variant &other);
   ObjectVariant(const ObjectVariant &other);
   ObjectVariant(Variant &&other);
   ObjectVariant(ObjectVariant &&other) ZAPI_DECL_NOEXCEPT;

   ObjectVariant &operator =(const ObjectVariant &other);
   ObjectVariant &operator =(const Variant &other);
   ObjectVariant &operator =(ObjectVariant &&other) ZAPI_DECL_NOEXCEPT;
   ObjectVariant &operator =(Variant &&other);
   
   Variant operator()();
   
   ObjectVariant &setProperty(const std::string &name, const Variant &value);
   Variant getProperty(const std::string &name);
   ObjectVariant &setStaticProperty(const std::string &name, const Variant &value);
   Variant getStaticProperty(const std::string &name);
   bool hasProperty(const std::string &name);

   bool isCallable(const char *name) const;
   Variant call(const char *name);
   Variant call(const char *name) const;

   template <typename ...Args>
   Variant call(const char *name, Args&&... args);
   template <typename ...Args>
   Variant call(const char *name, Args&&... args) const;
   
   template <typename ...Args>
   Variant classInvoke(Args&&... args);

   bool instanceOf(const char *className, size_t size) const;
   bool instanceOf(const char *className) const;
   bool instanceOf(const std::string &className) const;

   bool derivedFrom(const char *className, size_t size) const;
   bool derivedFrom(const char *className) const;
   bool derivedFrom(const std::string &className) const;
private:
   Variant exec(const char *name, int argc, Variant *argv);
   Variant exec(const char *name, int argc, Variant *argv) const;
   bool doClassInvoke(int argc, Variant *argv, zval *retval);
};

template <typename ...Args>
Variant ObjectVariant::call(const char *name, Args&&... args) const
{
   Variant vargs[] = { static_cast<Variant>(args)... };
   return exec(name, sizeof...(Args), vargs);
}

template <typename ...Args>
Variant ObjectVariant::call(const char *name, Args&&... args)
{
   return const_cast<const ObjectVariant &>(*this).call(name, args...);
}

template <typename ...Args>
Variant ObjectVariant::classInvoke(Args&&... args)
{
   Variant vargs[] = { Variant(std::forward<Args>(args))... };
   zval result;
   if (doClassInvoke(sizeof...(Args), vargs, &result)) {
      Variant ret(&result);
      zval_dtor(&result);
      return ret;
   }
   return nullptr;
}

} // ds
} // zapi

#endif // ZAPI_DS_OBJECT_VARIANT_H
