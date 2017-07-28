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
// Created by softboy on 5/26/17.

#ifndef ZAPI_LANG_ARGUMENT_H
#define ZAPI_LANG_ARGUMENT_H

#include "zapi/Global.h"

namespace zapi
{
namespace lang
{

namespace internal
{
class ArgumentPrivate;
} // internal

using internal::ArgumentPrivate;

class ZAPI_DECL_EXPORT Argument
{
public:
   virtual ~Argument();
   bool isNullable() const;
   bool isReference() const;
   bool isRequired() const;
   const char *getName() const;
   Type getType() const;
   const char *getClassName() const;
   
protected:
   Argument(const char *name, Type type, bool required = true, bool byReference = false);
   Argument(const char *name, const char *className, bool nullable = true, 
            bool required = true, bool byReference = false);
   Argument(const Argument &other);
   Argument(Argument &&other);
   Argument &operator=(const Argument &other);
   Argument &operator=(Argument &&other);
   
   ZAPI_DECLARE_PRIVATE(Argument)
   std::unique_ptr<ArgumentPrivate> m_implPtr;
};

#if defined(ZAPI_CC_MSVC) && ZAPI_CC_MSVC < 1800
using Arguments = std::vector<Argument>;
#else
using Arguments = std::initializer_list<Argument>;
#endif

class ZAPI_DECL_EXPORT RefArgument : public Argument
{
public:
   RefArgument(const char *name, Type type = Type::Null, bool required = true)
      : Argument(name, type, required, true)
   {}
   
   RefArgument(const char *name, const char *className, bool nullable = false, bool required = true)
      : Argument(name, className, nullable, required, true)
   {}
   
   RefArgument(const RefArgument &argument) 
      : Argument(argument)
   {}
   
   RefArgument(RefArgument &&argument) ZAPI_DECL_NOEXCEPT
      : Argument(std::move(argument))
   {}
   
   virtual ~RefArgument()
   {}
};

class ZAPI_DECL_EXPORT ValueArgument : public Argument
{
public:
   ValueArgument(const char *name, Type type = Type::Null, bool required = true)
      : Argument(name, type, required, false)
   {}
   
   ValueArgument(const char *name, const char *className, bool nullable = false, bool required = true)
      : Argument(name, className, nullable, required, false)
   {}
   
   ValueArgument(const ValueArgument &argument)
      : Argument(argument)
   {}
   
   ValueArgument(ValueArgument &&argument)
      : Argument(std::move(argument))
   {}
   
   virtual ~ValueArgument()
   {}
};

} // lang
} // zapi

#endif //ZAPI_LANG_ARGUMENT_H
