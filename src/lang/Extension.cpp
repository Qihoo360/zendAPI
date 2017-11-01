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
// Created by softboy on 7/25/17.

#include <ostream>

#include "zapi/vm/internal/CallablePrivate.h"
#include "zapi/lang/Extension.h"
#include "zapi/lang/internal/ExtensionPrivate.h"
#include "zapi/lang/Ini.h"
#include "zapi/lang/internal/NamespacePrivate.h"
#include "zapi/lang/Function.h"
#include "zapi/lang/Constant.h"
#include "zapi/lang/Namespace.h"
#include "zapi/vm/Closure.h"
#include "zapi/vm/internal/AbstractClassPrivate.h"
#include "php/Zend/zend_constants.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#include <map>

/**
 * We're almost there, we now need to declare an instance of the
 * structure defined above (if building for a single thread) or some
 * sort of impossible to understand magic pointer-to-a-pointer (for
 * multi-threading builds). We make this a static variable because
 * this already is bad enough.
 */
ZEND_DECLARE_MODULE_GLOBALS(zapi)

namespace
{
   using zapi::lang::Extension;
   using zapi::lang::internal::ExtensionPrivate;
   /**
   * Function that must be defined to initialize the "globals"
   * We do not have to initialize anything, but PHP needs to call this
   * method (crazy)
   * @param  globals
   */
   void init_globals(zend_zapi_globals *globals){}
   
   std::map<std::string, Extension *> name2extension;
   std::map<int, Extension *> mid2extension;
   
   int match_module(zval *value)
   {
      zend_module_entry *entry = static_cast<zend_module_entry *>(Z_PTR_P(value));
      auto iter = name2extension.find(entry->name);
      if (iter == name2extension.end()) {
         return ZEND_HASH_APPLY_KEEP;
      }
      mid2extension[entry->module_number] = iter->second;
      return ZEND_HASH_APPLY_KEEP;
   }
   
   Extension *find_module(int mid)
   {
      auto iter = mid2extension.find(mid);
      if (iter != mid2extension.end()) {
         return iter->second;
      }
      zend_hash_apply(&module_registry, match_module);
      iter = mid2extension.find(mid);
      if (iter == mid2extension.end()) {
         return nullptr;
      }
      return iter->second;
   }
}

namespace zapi
{
namespace lang
{

using zapi::lang::Constant;
using zapi::lang::internal::ExtensionPrivate;
using zapi::lang::internal::NamespacePrivate;
using zapi::vm::AbstractClassPrivate;

Extension::Extension(const char *name, const char *version, int apiVersion)
   : m_implPtr(new ExtensionPrivate(name, version, apiVersion, this))
{
   name2extension[name] = this;
}

Extension::~Extension()
{}

Extension &Extension::setStartupHandler(const Callback &callback)
{
   ZAPI_D(Extension);
   implPtr->m_startupHandler = callback;
   return *this;
}

Extension &Extension::setShutdownHandler(const Callback &callback)
{
   ZAPI_D(Extension);
   implPtr->m_shutdownHandler = callback;
   return *this;
}

Extension &Extension::setRequestStartupHandler(const Callback &callback)
{
   ZAPI_D(Extension);
   implPtr->m_requestStartupHandler = callback;
   return *this;
}

Extension &Extension::setRequestShutdownHandler(const Callback &callback)
{
   ZAPI_D(Extension);
   implPtr->m_requestShutdownHandler = callback;
   return *this;
}

Extension &Extension::setInfoHandler(const Callback &callback)
{
   ZAPI_D(Extension);
   implPtr->m_minfoHandler = callback;
   return *this;
}

void *Extension::getModule() 
{
   return getImplPtr()->getModule();
}

bool Extension::isLocked() const
{
   ZAPI_D(const Extension);
   return implPtr->m_locked;
}

const char *Extension::getName() const
{
   ZAPI_D(const Extension);
   return implPtr->m_entry.name;
}

const char *Extension::getVersion() const
{
   ZAPI_D(const Extension);
   return implPtr->m_entry.version;
}

Extension &Extension::registerFunction(const char *name, zapi::ZendCallable function, 
                                       const lang::Arguments &arguments)
{
   getImplPtr()->registerFunction(name, function, arguments);
   return *this;
}

Extension &Extension::registerInterface(const Interface &interface)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_classes.push_back(std::make_shared<Interface>(interface));
   return *this;
}

Extension &Extension::registerInterface(Interface &&interface)
{
   
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_classes.push_back(std::make_shared<Interface>(std::move(interface)));
   return *this;
}

Extension &Extension::registerNamespace(const Namespace &ns)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_namespaces.push_back(std::make_shared<Namespace>(ns));
   return *this;
}

Extension &Extension::registerNamespace(Namespace &&ns)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_namespaces.push_back(std::make_shared<Namespace>(std::move(ns)));
   return *this;
}

Extension &Extension::registerConstant(const Constant &constant)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_constants.push_back(std::make_shared<Constant>(constant));
   return *this;
}

Extension &Extension::registerConstant(Constant &&constant)
{
   ZAPI_D(Extension);
   if (implPtr->m_locked) {
      return *this;
   }
   implPtr->m_constants.push_back(std::make_shared<Constant>(std::move(constant)));
   return *this;
}

Namespace *Extension::findNamespace(const std::string &ns) const
{
   ZAPI_D(const Extension);
   auto begin = implPtr->m_namespaces.begin();
   auto end = implPtr->m_namespaces.end();
   while (begin != end) {
      auto &cur = *begin;
      if (cur->getName() == ns) {
         return cur.get();
      }
      ++begin;
   }
   return nullptr;
}

bool Extension::initialize(int moduleNumber)
{
   return getImplPtr()->initialize(moduleNumber);
}

Extension &Extension::registerIni(const Ini &entry)
{
   ZAPI_D(Extension);
   if (isLocked()) {
      return *this;
   }
   implPtr->m_iniEntries.emplace_back(new Ini(entry));
   return *this;
}

Extension &Extension::registerIni(Ini &&entry)
{
   ZAPI_D(Extension);
   if (isLocked()) {
      return *this;
   }
   implPtr->m_iniEntries.emplace_back(new Ini(std::move(entry)));
   return *this;
}

size_t Extension::getFunctionQuantity() const
{
   ZAPI_D(const Extension);
   return implPtr->m_functions.size();
}

size_t Extension::getIniQuantity() const
{
   ZAPI_D(const Extension);
   return implPtr->m_iniEntries.size();
}

size_t Extension::getConstantQuantity() const
{
   ZAPI_D(const Extension);
   return implPtr->m_constants.size();
}

size_t Extension::getNamespaceQuantity() const
{
   ZAPI_D(const Extension);
   return implPtr->m_namespaces.size();
}

namespace internal
{
ExtensionPrivate::ExtensionPrivate(const char *name, const char *version, int apiversion, Extension *extension)
   : m_apiPtr(extension)
{
   // assign all members (apart from the globals)
   m_entry.size = sizeof(zend_module_entry);
   m_entry.zend_api = ZEND_MODULE_API_NO;
   m_entry.zend_debug = ZEND_DEBUG;
   m_entry.zts = USING_ZTS;
   m_entry.ini_entry = nullptr;
   m_entry.deps = nullptr;
   m_entry.name = name;
   m_entry.functions = nullptr;
   m_entry.module_startup_func = &ExtensionPrivate::processStartup;
   m_entry.module_shutdown_func = &ExtensionPrivate::processShutdown;
   m_entry.request_startup_func = &ExtensionPrivate::processRequestStartup;
   m_entry.request_shutdown_func = &ExtensionPrivate::processRequestShutdown;
   m_entry.info_func = &ExtensionPrivate::processModuleInfo;
   m_entry.version = version;
   m_entry.globals_size = 0;
   m_entry.globals_ctor = nullptr;
   m_entry.globals_dtor = nullptr;
   m_entry.post_deactivate_func = nullptr;
   m_entry.module_started = 0;
   m_entry.type = 0;
   m_entry.handle = nullptr;
   m_entry.module_number = 0;
   m_entry.build_id = const_cast<char *>(static_cast<const char *>(ZEND_MODULE_BUILD_ID));
#ifdef ZTS
   m_entry.globals_id_ptr = nullptr;
#else
   m_entry.globals_ptr = nullptr;
#endif
   if (apiversion == ZAPI_API_VERSION) {
      return;
   }
   // mismatch between api versions, the extension is invalid, we use a
   // different startup function to report to the user
   m_entry.module_startup_func = &ExtensionPrivate::processMismatch;
   // the other callback functions are no longer necessary
   m_entry.module_shutdown_func = nullptr;
   m_entry.request_startup_func = nullptr;
   m_entry.request_shutdown_func = nullptr;
   m_entry.info_func = nullptr;
}

ExtensionPrivate::~ExtensionPrivate()
{
   name2extension.erase(m_entry.name);
   delete[] m_entry.functions;
}

size_t ExtensionPrivate::getFunctionQuantity() const
{
   // now just return global namespaces functions
   size_t ret = m_functions.size();
   for (const std::shared_ptr<Namespace> &ns : m_namespaces) {
      ret += ns->getFunctionQuantity();
   }
   return ret;
}

size_t ExtensionPrivate::getIniQuantity() const
{
   return m_iniEntries.size(); 
}

zend_module_entry *ExtensionPrivate::getModule()
{
   if (m_entry.functions) {
      return &m_entry;
   }
   if (m_entry.module_startup_func == &ExtensionPrivate::processMismatch) {
      return &m_entry;
   }
   size_t count = getFunctionQuantity();
   if (0 == count) {
      return &m_entry;
   }
   int i = 0;
   zend_function_entry *entries = new zend_function_entry[count + 1];
   iterateFunctions([&i, entries](Function &callable){
      callable.initialize(&entries[i]);
      i++;
   });
   for (std::shared_ptr<Namespace> &ns : m_namespaces) {
      ns->m_implPtr->iterateFunctions([&i, entries](const std::string &ns, Function &callable){
         callable.initialize(ns, &entries[i]);
         i++;
      });
   }
   zend_function_entry *last = &entries[count];
   memset(last, 0, sizeof(zend_function_entry));
   m_entry.functions = entries;
   return &m_entry;
}

void ExtensionPrivate::iterateFunctions(const std::function<void(Function &func)> &callback)
{
   for (auto &function : m_functions) {
      callback(*function);
   }
}

void ExtensionPrivate::iterateIniEntries(const std::function<void (lang::Ini &)> &callback)
{
   for (auto &entry : m_iniEntries) {
      callback(*entry);
   }
}

void ExtensionPrivate::iterateConstants(const std::function<void (lang::Constant &)> &callback)
{
   for (auto &constant : m_constants) {
      callback(*constant);
   }
}

void ExtensionPrivate::iterateClasses(const std::function<void(AbstractClass &cls)> &callback)
{
   for (auto &cls : m_classes) {
      callback(*cls);
   }
}

int ExtensionPrivate::processMismatch(INIT_FUNC_ARGS)
{
   Extension *extension = find_module(module_number);
   // @TODO is this really good? we need a method to check compatibility more graceful
   zapi::warning << " Version mismatch between zendAPI and extension " << extension->getName()
                 << " " << extension->getVersion() << " (recompile needed?) " << std::endl;
   return BOOL2SUCCESS(true);
}

int ExtensionPrivate::processRequestStartup(INIT_FUNC_ARGS)
{
   Extension *extension = find_module(module_number);
   if (extension->m_implPtr->m_requestStartupHandler) {
      extension->m_implPtr->m_requestStartupHandler();
   }
   return BOOL2SUCCESS(true);
}

int ExtensionPrivate::processRequestShutdown(SHUTDOWN_FUNC_ARGS)
{
   Extension *extension = find_module(module_number);
   if (extension->m_implPtr->m_requestShutdownHandler) {
      extension->m_implPtr->m_requestShutdownHandler();
   }
   // release call context
   AbstractClassPrivate::sm_contextPtrs.clear();
   return BOOL2SUCCESS(true);
}

int ExtensionPrivate::processStartup(INIT_FUNC_ARGS)
{
   ZEND_INIT_MODULE_GLOBALS(zapi, init_globals, nullptr);
   Extension *extension = find_module(module_number);
   return BOOL2SUCCESS(extension->initialize(module_number));
}

int ExtensionPrivate::processShutdown(SHUTDOWN_FUNC_ARGS)
{
   Extension *extension = find_module(module_number);
   mid2extension.erase(module_number);
   return BOOL2SUCCESS(extension->m_implPtr->shutdown(module_number));
}

void ExtensionPrivate::processModuleInfo(ZEND_MODULE_INFO_FUNC_ARGS)
{
   Extension *extension = find_module(zend_module->module_number);
   if (extension->m_implPtr->m_minfoHandler) {
      extension->m_implPtr->m_minfoHandler();
   }
}

ExtensionPrivate &ExtensionPrivate::registerFunction(const char *name, zapi::ZendCallable function, 
                                                     const Arguments &arguments)
{
   if (m_locked) {
      return *this;
   }
   m_functions.push_back(std::make_shared<Function>(name, function, arguments));
   return *this;
}

bool ExtensionPrivate::initialize(int moduleNumber)
{
   m_zendIniDefs.reset(new zend_ini_entry_def[getIniQuantity() + 1]);
   int i = 0;
   // fill ini entry def
   iterateIniEntries([this, &i, moduleNumber](Ini &iniEntry){
      zend_ini_entry_def *zendIniDef = &m_zendIniDefs[i];
      iniEntry.setupIniDef(zendIniDef, moduleNumber);
      i++;
   });
   memset(&m_zendIniDefs[i], 0, sizeof(m_zendIniDefs[i]));
   zend_register_ini_entries(m_zendIniDefs.get(), moduleNumber);
   
   iterateConstants([moduleNumber](Constant &constant) {
      constant.initialize(moduleNumber);
   });
   // here we register all global classes and interfaces
   iterateClasses([moduleNumber](AbstractClass &cls) {
      cls.initialize(moduleNumber);
   });
   // work with register namespaces
   
   for (std::shared_ptr<Namespace> &ns : m_namespaces) {
      ns->initialize(moduleNumber);
   }
   // initialize closure class
   zapi::vm::Closure::registerToZendNg(moduleNumber);
   
   // remember that we're initialized (when you use "apache reload" it is
   // possible that the processStartup() method is called more than once)
   m_locked = true;
   if (m_startupHandler) {
      m_startupHandler();
   }
   return true;
}

bool ExtensionPrivate::shutdown(int moduleNumber)
{
   zend_unregister_ini_entries(moduleNumber);
   m_zendIniDefs.reset();
   if (m_shutdownHandler) {
      m_shutdownHandler();
   }
   zapi::vm::Closure::unregisterFromZendNg();
   m_locked = false;
   return true;
}


} // internal

} // lang
} // zapi
