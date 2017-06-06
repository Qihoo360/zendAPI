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
// Created by zzu_softboy on 19/05/2017.

#ifndef ZAPI_BRIDGE_EXTENSIONBASE_H
#define ZAPI_BRIDGE_EXTENSIONBASE_H

#include "zapi/Global.h"

namespace zapi
{
namespace bridge
{

using zapi::Callback;
class Extension;

/**
 * Base class for ExtensionImpl objects. Common code used by both the Zend
 * and HHVM engine.
 *
 * @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */
class ExtensionBase
{
protected:
   /**
    * Pointer to the extension object that is filled by the extension programmer
    * @var Extension
    */
   Extension *m_data;

   /**
    * Callback that is called after the engine is initialized and before the
    * pageviews are going to be handled
    * @var Callback
    */
   Callback m_startupHandler;

   /**
    * Callback that is called in front of each request
    * @var Callback
    */
   Callback m_requestHandler;

   /**
    * Callback that is called right after each request
    * @var Callback
    */
   Callback m_idleHandler;

   /**
    * Callback that is called right before the engine is closing down
    * @var Callback
    */
   Callback m_shutdownHandler;

public:

   /**
    * Constructor
    * @param data Extension object created by the extension programmer
    */
   ExtensionBase(Extension *data) : m_data(data)
   {}

   ExtensionBase(const ExtensionBase &extension) = delete;
   ExtensionBase(ExtensionBase &&extension) = delete;

   /**
    * Register a function to be called when the PHP engine is ready
    *
    * The callback will be called after all extensions are loaded, and all
    * functions and classes are available, but before the first pageview/request
    * is handled. You can register this callback if you want to be notified
    * when the engine is ready, for example to initialize certain things.
    *
    * @param Callback callback
    */
   void setStartupHandler(const Callback &callback) noexcept
   {
      m_startupHandler = callback;
   }

   /**
    * Register a function to be called when the PHP engine is going to stop
    *
    * The callback will be called right before the process is going to stop.
    * You can register a function if you want to clean up certain things.
    *
    * @param Callback callback
    */
   void setShutdownHandler(const Callback &callback) noexcept
   {
      m_shutdownHandler = callback;
   }

   /**
    * Register a callback that is called at the beginning of each pageview/request
    *
    * You can register a callback if you want to initialize certain things
    * at the beginning of each request. Remember that the extension can handle
    * multiple requests after each other, and you may want to set back certain
    * global variables to their initial variables in front of each request
    *
    * @param Callback callback
    */
   void setRequestHandler(const Callback &callback) noexcept
   {
      m_requestHandler = callback;
   }

   /**
    * Register a callback that is called to cleanup things after a pageview/request
    *
    * The callback will be called after _each_ request, so that you can clean up
    * certain things and make your extension ready to handle the next request.
    * This method is called onIdle because the extension is idle in between
    * requests.
    *
    * @param Callback callback
    */
   void setIdleHandler(const Callback &callback) noexcept
   {
      m_idleHandler = callback;
   }

   virtual ~Extension()
   {}
};

} // bridge
} //zapi

#endif //ZAPI_BRIDGE_EXTENSIONBASE_H