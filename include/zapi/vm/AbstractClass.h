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
// Created by zzu_softboy on 08/06/2017.

#ifndef ZAPI_VM_STDCLASSIMPL_H
#define ZAPI_VM_STDCLASSIMPL_H

#include "zapi/Global.h"
#include "zapi/lang/Argument.h"

namespace zapi
{

namespace bridge
{
namespace internal
{
class ExtensionPrivate;
} // internal
} // bridge

namespace lang
{
class StdClass;
class Constant;
class Variant;
class Interface;
namespace internal
{
class NamespacePrivate;
} // internal
} // lang

namespace vm
{
namespace internal
{
class AbstractClassPrivate;
} // internal

using zapi::vm::internal::AbstractClassPrivate;
using zapi::lang::Modifier;
using zapi::lang::ClassType;
using zapi::lang::StdClass;
using zapi::lang::Arguments;
using zapi::lang::Constant;
using zapi::lang::Variant;
using zapi::lang::Interface;

class ZAPI_DECL_EXPORT AbstractClass
{
public:
   AbstractClass(const char *className, ClassType type);
   AbstractClass(const AbstractClass &other);
   AbstractClass(AbstractClass &&other) ZAPI_DECL_NOEXCEPT;
   AbstractClass &operator=(const AbstractClass &other);
   AbstractClass &operator=(AbstractClass &&other) ZAPI_DECL_NOEXCEPT;
   virtual ~AbstractClass();
   
protected:
   void registerMethod(const char *name, zapi::ZendCallable callable, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable0 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable1 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable2 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable3 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable4 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable5 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable6 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable7 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   // static
   void registerMethod(const char *name, const zapi::MethodCallable8 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable9 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable10 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   void registerMethod(const char *name, const zapi::MethodCallable11 &method, Modifier flags = Modifier::None, const Arguments &args = {});
   // abstract
   void registerMethod(const char *name, Modifier flags = Modifier::None, const Arguments &args = {});
   
   void registerProperty(const char *name, std::nullptr_t value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, int16_t value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, int32_t value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, int64_t value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, char value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, const std::string &value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, const char *value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, bool value, Modifier flags = Modifier::Public);
   void registerProperty(const char *name, double value, Modifier flags = Modifier::Public);
   // callback property
   void registerProperty(const char *name, const zapi::GetterMethodCallable0 &getter);
   void registerProperty(const char *name, const zapi::GetterMethodCallable1 &getter);
   void registerProperty(const char *name, const zapi::GetterMethodCallable0 &getter, const zapi::GetterMethodCallable0 &setter);
   void registerProperty(const char *name, const zapi::GetterMethodCallable0 &getter, const zapi::GetterMethodCallable1 &setter);
   void registerProperty(const char *name, const zapi::GetterMethodCallable1 &getter, const zapi::GetterMethodCallable0 &setter);
   void registerProperty(const char *name, const zapi::GetterMethodCallable1 &getter, const zapi::GetterMethodCallable1 &setter);
   void registerInterface(const Interface &interface);
   void registerInterface(Interface &&interface);
   void registerBaseClass(const AbstractClass &base);
   void registerConstant(const Constant &constant);
protected:
   virtual StdClass *construct() const;
   virtual StdClass *clone() const;
   virtual bool clonable() const;
   virtual bool serializable() const;
   virtual bool traversable() const;
   virtual void callClone(StdClass *nativeObject) const;
   virtual void callDestruct(StdClass *nativeObject) const;
   // property
   virtual Variant callGet(StdClass *nativeObject, const std::string &name) const;
   virtual void callSet(StdClass *nativeObject, const std::string &name, const Variant &value) const;
   virtual bool callIsset(StdClass *nativeObject, const std::string &name) const;
   virtual void callUnset(StdClass *nativeObject, const std::string &name) const;
private:
   zend_class_entry *initialize(const std::string &ns, int moduleNumber);
   zend_class_entry *initialize(int moduleNumber);
private:
   ZAPI_DECLARE_PRIVATE(AbstractClass)
   std::shared_ptr<AbstractClassPrivate> m_implPtr;
   friend class zapi::bridge::internal::ExtensionPrivate;
   friend class zapi::lang::internal::NamespacePrivate;
};

} // vm
} // zapi

#endif // ZAPI_VM_STDCLASSIMPL_H
