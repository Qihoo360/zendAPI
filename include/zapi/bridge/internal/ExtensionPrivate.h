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

#ifndef ZAPI_BRIDGE_INTERNAL_EXTENSIONPRIVATE_H
#define ZAPI_BRIDGE_INTERNAL_EXTENSIONPRIVATE_H

#include <list>
#include "zapi/Global.h"
#include "php/Zend/zend_modules.h"
#include "zapi/lang/Argument.h"

namespace zapi
{
namespace vm
{

class Callable;

} // vm

namespace bridge
{

class Extension;
class IniEntry;

namespace internal
{

using zapi::lang::Arguments;
using zapi::vm::Callable;
using zapi::bridge::IniEntry;

class ExtensionPrivate
{
public:
   ZAPI_DECLARE_PUBLIC(Extension)
   ExtensionPrivate(Extension *extension)
      :m_apiPtr(extension)
   {}
   
   ExtensionPrivate(const char *name, const char *version, int apiversion, Extension *extension);
   ExtensionPrivate(const ExtensionPrivate &) = delete;
   ExtensionPrivate(const ExtensionPrivate &&) = delete;
   ~ExtensionPrivate();
   
   // methods
   
   ExtensionPrivate &registerFunction(const char *name, zapi::ZendCallable function, const Arguments &arguments = {});
   void iterateFunctions(const std::function<void(Callable &func)> &callback);
   void iterateIniEntries(const std::function<void(IniEntry &ini)> &callback);
   
   const char *getName() const;
   const char *getVersion() const;
   bool isLocked() const;
   zend_module_entry *getModule();
   size_t getFunctionQuantity() const;
   size_t getIniEntryQuantity() const;
   bool initialize(int moduleNumber);
   bool shutdown(int moduleNumber);
   static int processStartup(int type, int moduleNumber);
   static int processShutdown(int type, int moduleNumber);
   static int processRequest(int type, int moduleNumber);
   static int processIdle(int type, int moduleNumber);
   static int processMismatch(int type, int moduleNumber);
   
   // properties
   
   Extension *m_apiPtr;
   zapi::Callback m_startupHandler;
   zapi::Callback m_requestHandler;
   zapi::Callback m_idleHandler;
   zapi::Callback m_shutdownHandler;
   zend_module_entry m_entry;
   bool m_locked = false;
   std::list<std::shared_ptr<IniEntry>> m_iniEntries;
   std::unique_ptr<zend_ini_entry_def[]> m_zendIniDefs = nullptr;
   std::list<std::shared_ptr<Callable>> m_functions;
};

} // internal
} // bridge
} // zapi

#endif // ZAPI_BRIDGE_INTERNAL_EXTENSIONPRIVATE_H