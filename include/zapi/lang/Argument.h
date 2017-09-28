// @copyright 2013, 2014 Copernica BV
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
//
// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by softboy on 2017/05/26.

#ifndef ZAPI_LANG_ARGUMENT_H
#define ZAPI_LANG_ARGUMENT_H

#include "zapi/Global.h"
#include <initializer_list>

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
   Argument(const Argument &other);
   Argument(Argument &&other) ZAPI_DECL_NOEXCEPT;
   Argument &operator=(const Argument &other);
   Argument &operator=(Argument &&other) ZAPI_DECL_NOEXCEPT;
   virtual ~Argument();
   bool isNullable() const;
   bool isReference() const;
   bool isRequired() const;
   bool isVariadic() const;
   const char *getName() const;
   Type getType() const;
   const char *getClassName() const;
   
protected:
   Argument(const char *name, Type type, bool required = true, 
            bool byReference = false, bool isVariadic = false);
   Argument(const char *name, const char *className, bool nullable = true, 
            bool required = true, bool byReference = false, bool isVariadic = false);
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
   RefArgument(const char *name, Type type = Type::Null, bool required = true,
               bool isVariadic = false)
      : Argument(name, type, required, true, isVariadic)
   {}
   
   RefArgument(const char *name, const char *className, bool nullable = false, 
               bool required = true, bool isVariadic = false)
      : Argument(name, className, nullable, required, true, isVariadic)
   {}
   
   RefArgument(const RefArgument &argument) 
      : Argument(argument)
   {}
   
   RefArgument(RefArgument &&argument) ZAPI_DECL_NOEXCEPT
      : Argument(std::move(argument))
   {}
   
   virtual ~RefArgument();
};

class ZAPI_DECL_EXPORT ValueArgument : public Argument
{
public:
   ValueArgument(const char *name, Type type = Type::Null, 
                 bool required = true, bool isVariadic = false)
      : Argument(name, type, required, false, isVariadic)
   {}
   
   ValueArgument(const char *name, const char *className, bool nullable = false, 
                 bool required = true, bool isVariadic = false)
      : Argument(name, className, nullable, required, false, isVariadic)
   {}
   
   ValueArgument(const ValueArgument &argument)
      : Argument(argument)
   {}
   
   ValueArgument(ValueArgument &&argument) ZAPI_DECL_NOEXCEPT
      : Argument(std::move(argument))
   {}
   
   virtual ~ValueArgument();
};

class ZAPI_DECL_EXPORT VariadicArgument : public Argument
{
public:
   VariadicArgument(const char *name, Type type = Type::Null, bool isReference = false)
      : Argument(name, type, false, isReference, true)
   {}
   
   VariadicArgument(const char *name, const char *className, bool nullable = false,
                    bool isReference = false)
      : Argument(name, className, nullable, false, isReference, true)
   {}
   
   VariadicArgument(const ValueArgument &argument)
      : Argument(argument)
   {}
   
   VariadicArgument(ValueArgument &&argument) ZAPI_DECL_NOEXCEPT
      : Argument(std::move(argument))
   {}
   
   virtual ~VariadicArgument();
};

} // lang
} // zapi

#endif //ZAPI_LANG_ARGUMENT_H
