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

#include "zapi/Global.h"

#include "php/Zend/zend_modules.h"

namespace zapi
{
namespace bridge
{

class Extension;
namespace internal
{

class ExtensionPrivate
{
public:
   ExtensionPrivate(Extension *extension)
      :m_apiPtr(extension)
   {}
   
   ExtensionPrivate(const char *name, const char *version, int apiversion, Extension *extension);
   
   ExtensionPrivate(const ExtensionPrivate &) = delete;
   ExtensionPrivate(const ExtensionPrivate &&) = delete;
   
public:
   const char *getName() const;
   const char *getVersion() const;
   bool isLocked();
   zend_module_entry *getModule();
   operator zend_module_entry * ()
   {
      return getModule();
   }
   
   void setStartupHandler(const zapi::Callback &handler)
   {
      m_startupHandler = handler;
   }
   
   void setRequestHandler(const zapi::Callback &handler)
   {
      m_requestHandler = handler;
   }
   
   void setShutdownHandler(const zapi::Callback &handler)
   {
      m_shutdownHandler = handler;
   }
   
   void setIdleHandler(const zapi::Callback &handler)
   {
      m_idleHandler = handler;
   }

private:
   bool initialize(int moduleNumber);
   bool shutdown(int moduleNumber);
   static int processStartup(int type, int moduleNumber);
   static int processShutdown(int type, int moduleNumber);
   static int processRequest(int type, int moduleNumber);
   static int processIdle(int type, int moduleNumber);
   static int processMismatch(int type, int moduleNumber);
private:
   ZAPI_DECLARE_PUBLIC(Extension)
   
private:
   Extension *m_apiPtr;
   zapi::Callback m_startupHandler;
   zapi::Callback m_requestHandler;
   zapi::Callback m_idleHandler;
   zapi::Callback m_shutdownHandler;
   zend_module_entry m_entry;
   bool m_locked = false;
   std::unique_ptr<zend_ini_entry_def[]> m_iniEntries = nullptr;
};

} // internal
} // bridge
} // zapi

#endif // ZAPI_BRIDGE_INTERNAL_EXTENSIONPRIVATE_H