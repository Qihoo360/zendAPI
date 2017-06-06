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
// Created by zzu_softboy on 06/06/2017.

#ifndef ZAPI_VM_EXTENSION_H
#define ZAPI_VM_EXTENSION_H

#include <zapi/bridge/Extension.h>
#include "zapi/kernel/ExtensionBase.h"

namespace zapi
{
namespace vm
{

using zapi::kernel::ExtensionBase;
using BridgeExtension = zapi::bridge::Extension;

class Extension : public ExtensionBase
{
protected:
   /**
    * The information that is passed to the Zend engine
    * @var zend_module_entry m_entry
    */
   zend_module_entry m_entry;

   /**
    * Is the object locked? This prevents crashes for 'apache reload'
    * because we then do not have to re-initialize the entire php engine
    *
    * @var bool m_locked
    */
   bool m_locked = false;

public:
   /**
    * Constructor
    * @param zapi::bridge::Extension data Extension object created by the extension programmer
    * @param char * name        Name of the extension
    * @param char * version     Version number
    * @param int apiversion  API version number
    */
   Extension(BridgeExtension *data, const char *name, const char *version, int apiVersion) noexcept;

   Extension(const Extension &extension) = delete;
   Extension(Extension &&extension) = delete;

   virtual ~Extension();

   /**
    * The extension name
    * 
    * @return const char *
    */
   const char *getName();

   /**
    * The extension version
    * @return const char *
    */
   const char *getVersion();

   /**
    * Is the object locked (true) or is it still possible to add more functions,
    * classes and other elements to it?
    * @return bool
    */
   bool locked()
   {
      return m_locked;
   }

   /**
    * Retrieve the module entry
    *
    * This is the memory address that should be exported by the get_module()
    * function.
    *
    * @return zend_module_entry
    */
   zend_module_entry *getModule();

   /**
    * Cast to a module entry
    * @return _zend_module_entry*
    */
   operator zend_module_entry * ()
   {
      return getModule();
   }

private:
   /**
    * Initialize the extension after it was registered
    * @param int moduleNumber
    * @return bool
    */
   bool initialize(int moduleNumber);

   /**
    * Shutdown the extension
    * @param int moduleNumber
    * @return bool
    */
   bool shutdown(int moduleNumber);

   /**
    * Function that is called when the extension initializes
    * @param int type        Module type
    * @param int moduleNumber Module number
    * @return int         0 on success
    */
   static int processStartup(int type, int moduleNumber);

   /**
    * Function that is called when the extension is about to be stopped
    * @param int type Module type
    * @param int moduleNumber Module number
    * @return int
    */
   static int processShutdown(int type, int moduleNumber);

   /**
    * Function that is called when a request starts
    * @param int type        Module type
    * @param int moduleNumber      Module number
    * @return int         0 on success
    */
   static int processRequest(int type, int moduleNumber);

   /**
    * Function that is called when a request is ended
    * @param int type        Module type
    * @param int moduleNumber      Module number
    * @return int         0 on success
    */
   static int processIdle(int type, int moduleNumber);

   /**
    * Function that is called when the PHP engine initializes with a different PHP-CPP
    * version for the libphpcpp.so file than the version the extension was compiled for
    * @param int type        Module type
    * @param int moduleNumber      Module number
    * @return int         0 on success
    */
   static int processMismatch(int type, int moduleNumber);
};

} // vm
} // zapi

#endif //ZAPI_VM_EXTENSION_H
