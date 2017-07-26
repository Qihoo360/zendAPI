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
// Created by softboy on 7/25/17.

#include "zapi/kernel/IniEntry.h"
#include "zapi/bridge/Extension.h"
#include "zapi/bridge/internal/ExtensionPrivate.h"

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
   using ExtensionPrivate = zapi::bridge::internal::ExtensionPrivate;
  /**
   * Function that must be defined to initialize the "globals"
   * We do not have to initialize anything, but PHP needs to call this
   * method (crazy)
   * @param  globals
   */
   void init_global(zend_zapi_globals *globals){}
   
   std::map<std::string, ExtensionPrivate *> name2extension;
   std::map<int, ExtensionPrivate *> number2extension;
   
   int match_module(zval *value)
   {
      
   }
   
   ExtensionPrivate *find_module(int number)
   {
      
   }
}

namespace zapi
{
namespace bridge
{

Extension::Extension(const char *name, const char *version, int apiVersion)
   : m_implPtr(new ExtensionPrivate(name, version, apiVersion, this))
{}

Extension &Extension::setStartupHandler(const Callback &callback)
{
   getImplPtr()->setStartupHandler(callback);
   return *this;
}

Extension &Extension::setRequestHandler(const Callback &callback)
{
   getImplPtr()->setRequestHandler(callback);
   return *this;
}

Extension &Extension::setIdleHandler(const Callback &callback)
{
   getImplPtr()->setIdleHandler(callback);
   return *this;
}

Extension &Extension::setShutdownHandler(const Callback &callback)
{
   getImplPtr()->setShutdownHandler(callback);
   return *this;
}

void *Extension::getModule()
{
   return static_cast<void *>(getImplPtr()->getModule());
}

bool Extension::isLocked() const
{
   return getImplPtr()->isLocked();
}

const char *Extension::getName() const
{
   return getImplPtr()->getName();
}

const char *Extension::getVersion() const
{
   return getImplPtr()->getVersion();
}

Extension &Extension::registerFunction(const char *name, zapi::ZendCallable function, const lang::Arguments &arguments)
{
   getImplPtr()->registerFunction(name, function, arguments);
   return *this;
}

namespace internal
{
ExtensionPrivate::ExtensionPrivate(const char *name, const char *version, int apiversion, Extension *extension)
   : m_apiPtr(extension)
{
   name2extension[name] = this;
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
   m_entry.request_startup_func = &ExtensionPrivate::processRequest;
   m_entry.request_shutdown_func = &ExtensionPrivate::processIdle;
   m_entry.info_func = nullptr;
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
}

ExtensionPrivate::~ExtensionPrivate()
{
   name2extension.erase(m_entry.name);
   delete[] m_entry.functions;
}

const char *ExtensionPrivate::getName() const
{
   return m_entry.name;
}

const char *ExtensionPrivate::getVersion() const
{
   return m_entry.version;
}

zend_module_entry *ExtensionPrivate::getModule()
{
   if (m_entry.functions) {
      return &m_entry;
   }
   if (m_entry.module_startup_func == &ExtensionPrivate::processMismatch) {
      return &m_entry;
   }
   
}

int ExtensionPrivate::processIdle(int type, int moduleNumber)
{}

int ExtensionPrivate::processMismatch(int type, int moduleNumber)
{}

int ExtensionPrivate::processRequest(int type, int moduleNumber)
{}

int ExtensionPrivate::processStartup(int type, int moduleNumber)
{}

int ExtensionPrivate::processShutdown(int type, int moduleNumber)
{}

ExtensionPrivate &ExtensionPrivate::registerFunction(const char *name, zapi::ZendCallable function, 
                                                     const Arguments &arguments)
{
   if (isLocked()) {
      return *this;
   }
   return *this;
}

} // internal

} // bridge
} // zapi