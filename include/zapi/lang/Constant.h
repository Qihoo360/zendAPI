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

#ifndef ZAPI_LANG_CONSTANT_H
#define ZAPI_LANG_CONSTANT_H

#include "zapi/Global.h"

// forward declare with namespace
namespace zapi
{
namespace lang
{
namespace internal
{
class ExtensionPrivate;
class ConstantPrivate;
class NamespacePrivate;
} // internal
} // lang
} //zapi
// end forward declare

namespace zapi
{
namespace lang
{

using internal::ConstantPrivate;

class ZAPI_DECL_EXPORT Constant final
{
public:
   Constant(const char *name, std::nullptr_t value = nullptr);
   Constant(const char *name, bool value);
   Constant(const char *name, int32_t value);
   Constant(const char *name, int64_t value);
   Constant(const char *name, double value);
   Constant(const char *name, const char *value);
   Constant(const char *name, const char *value, size_t size);
   Constant(const char *name, const std::string &value);
   Constant(const Constant &other);
   Constant(Constant &&other) ZAPI_DECL_NOEXCEPT;
   Constant &operator=(const Constant &other);
   Constant &operator=(Constant &&other) ZAPI_DECL_NOEXCEPT;
   const std::string &getName() const;
   const zend_constant &getZendConstant() const;
   virtual ~Constant();
private:
   void initialize(const std::string &prefix, int moduleNumber);
   void initialize(int moduleNumber);
private:
   ZAPI_DECLARE_PRIVATE(Constant)
   std::shared_ptr<ConstantPrivate> m_implPtr;
   friend class zapi::lang::internal::ExtensionPrivate;
   friend class zapi::lang::internal::NamespacePrivate;
};

} // lang
} // zapi

#endif //ZAPI_LANG_CONSTANT_H
