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
   m_implPtr->setStartupHandler(callback);
   return *this;
}

Extension &Extension::setRequestHandler(const Callback &callback)
{
   m_implPtr->setRequestHandler(callback);
   return *this;
}

Extension &Extension::setIdleHandler(const Callback &callback)
{
   m_implPtr->setIdleHandler(callback);
   return *this;
}

Extension &Extension::setShutdownHandler(const Callback &callback)
{
   m_implPtr->setShutdownHandler(callback);
   return *this;
}

void *Extension::getModule() const
{
   return static_cast<void *>(m_implPtr->getModule());
}

bool Extension::isLocked() const
{
   return m_implPtr->isLocked();
}

} // bridge
} // zapi