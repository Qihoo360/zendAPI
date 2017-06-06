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
// Created by softboy on 18/05/2017.
//

#ifndef ZAPI_BRIDGE_EXTENSION_H
#define ZAPI_BRIDGE_EXTENSION_H

#include "zapi/Global.h"

namespace zapi
{
namespace bridge
{

class ExtensionPrivate;

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

   operator void * ()
   {
      return module();
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
   Extension &setRequestHandler(const Callback &callback);

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
   Extension &setIdleHandler(const Callback &callback);

   /**
    * Retrieve the module pointer
    *
    * This is the memory address that should be exported by the get_module()
    * function.
    *
    * @return void*
    */
   void *module();

   virtual ~Extension()
   {}

protected:

   virtual bool locked() const;

private:
   /**
    * The implementation object
    *
    * @var std::unique_ptr<ExtensionPrivate> m_implPtr
    */
   std::unique_ptr<ExtensionPrivate> m_implPtr;
};

} // bridge
} // zapi

#endif //ZAPI_BRIDGE_EXTENSION_H
