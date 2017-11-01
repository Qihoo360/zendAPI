// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
// @copyright 2014 Copernica BV
//
// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
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
// Created by softboy on 18/05/2017.
//

#ifndef ZAPI_LANG_EXTENSION_H
#define ZAPI_LANG_EXTENSION_H

#include "zapi/Global.h"
#include "zapi/lang/Argument.h"
#include "zapi/lang/Interface.h"
#include "zapi/vm/InvokeBridge.h"
#include "zapi/vm/AbstractClass.h"
#include "zapi/lang/internal/ExtensionPrivate.h"

#include <list>

// forware declare with namespace
namespace zapi
{
namespace ds
{
class Variant;
} // ds

namespace lang
{
class Parameters;
class Constant;
class Namespace;
class Ini;
template <typename> class Class;
} // lang
} // zapi
// end forware declare

namespace zapi
{
namespace lang
{

using zapi::ds::Variant;
using zapi::lang::Parameters;
using zapi::lang::Arguments;
using zapi::lang::Constant;
using zapi::lang::Class;
using zapi::lang::Interface;
using zapi::lang::Namespace;
using zapi::lang::Ini;
using internal::ExtensionPrivate;
using zapi::vm::AbstractClass;

class ZAPI_DECL_EXPORT Extension
{
public:
  /**
   * Constructor that defines a number of functions right away
   *
   * The first two parameters should be filled by the extension programmer with the
   * name of the extension, and the version number of the extension (like "1.0").
   * The third parameter, apiversion, does not have to be supplied and is best kept
   * to the default value. This third parameter checks whether the PHP-CPP version
   * that is currently installed on the server is the same as the PHP-CPP version
   * that was used to compile the extension with.
   *
   * @param  name        Extension name
   * @param  version     Extension version string
   * @param  apiversion  ZAPI API version (this should always be ZAPI_API_VERSION, so you better not supply it)
   */
   Extension(const char *name, const char *version = "1.0", int apiVersion = ZAPI_API_VERSION);
   Extension(const Extension &extension) = delete;
   Extension(Extension &&extension) = delete;
   virtual ~Extension();
public:
   template <typename T, typename std::decay<T>::type callable>
   Extension &registerFunction(const char *name, const Arguments &args = {});
   
   Extension &registerIni(const Ini &entry);
   Extension &registerIni(Ini &&entry);
   
   template <typename T>
   Extension &registerClass(const Class<T> &nativeClass);
   template <typename T>
   Extension &registerClass(Class<T> &&nativeClass);
   
   Extension &registerInterface(const Interface &interface);
   Extension &registerInterface(Interface &&interface);
   
   Extension &registerNamespace(const Namespace &ns);
   Extension &registerNamespace(Namespace &&ns);
   
   Extension &registerConstant(Constant &&constant);
   Extension &registerConstant(const Constant &constant);
   
   Namespace *findNamespace(const std::string &ns) const;
   
   size_t getIniQuantity() const;
   size_t getFunctionQuantity() const;
   size_t getConstantQuantity() const;
   size_t getNamespaceQuantity() const;
   operator void * ()
   {
      return getModule();
   }
   
  /**
   * Register a function to be called when the PHP engine is ready
   *
   * The callback will be called after all extensions are loaded, and all
   * functions and classes are available, but before the first pageview/request
   * is handled. You can register this callback if you want to be notified
   * when the engine is ready, for example to initialize certain things.
   *
   * @param Callback callback Function to be called
   * @return Extension Same object to allow chaining
   */
   Extension &setStartupHandler(const Callback &callback);
   
  /**
   * Register a function to be called when the PHP engine is going to stop
   *
   * The callback will be called right before the process is going to stop.
   * You can register a function if you want to clean up certain things.
   *
   * @param callback Function to be called
   * @return Extension Same object to allow chaining
   */
   Extension &setShutdownHandler(const Callback &callback);
   
  /**
   * Register a callback that is called at the beginning of each pageview/request
   *
   * You can register a callback if you want to initialize certain things
   * at the beginning of each request. Remember that the extension can handle
   * multiple requests after each other, and you may want to set back certain
   * global variables to their initial variables in front of each request
   *
   * @param Callback callback Function to be called
   * @return Extension Same object to allow chaining
   */
   Extension &setRequestStartupHandler(const Callback &callback);
   
  /**
   * Register a callback that is called to cleanup things after a pageview/request
   *
   * The callback will be called after _each_ request, so that you can clean up
   * certain things and make your extension ready to handle the next request.
   * This method is called onIdle because the extension is idle in between
   * requests.
   *
   * @param Callback callback Function to be called
   * @return Extension Same object to allow chaining
   */
   Extension &setRequestShutdownHandler(const Callback &callback);
   
   Extension &setInfoHandler(const Callback &callback);
   
  /**
   * Retrieve the module pointer
   *
   * This is the memory address that should be exported by the get_module()
   * function.
   *
   * @return void*
   */
   void *getModule();
   const void *getModule() const;
   const char *getName() const;
   const char *getVersion() const;
protected:
   Extension &registerFunction(const char *name, zapi::ZendCallable function, const Arguments &args);
   bool isLocked() const;
private:
   bool initialize(int moduleNumber);
private:
   ZAPI_DECLARE_PRIVATE(Extension)
  /**
   * The implementation object
   *
   * @var std::unique_ptr<ExtensionPrivate> m_implPtr
   */
   std::unique_ptr<internal::ExtensionPrivate> m_implPtr;
};

template <typename T>
Extension &Extension::registerClass(const Class<T> &nativeClass)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   // just shadow copy 
   implPtr->m_classes.push_back(std::shared_ptr<AbstractClass>(new Class<T>(nativeClass)));
   return *this;
}

template <typename T>
Extension &Extension::registerClass(Class<T> &&nativeClass)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_classes.push_back(std::shared_ptr<AbstractClass>(new Class<T>(std::move(nativeClass))));
   return *this;
}

template <typename T, typename std::decay<T>::type callable>
Extension &Extension::registerFunction(const char *name, const Arguments &args)
{
   return registerFunction(name, &zapi::vm::InvokeBridge<T, callable>::invoke, args);
}

} // lang
} // zapi

#endif // ZAPI_LANG_EXTENSION_H
