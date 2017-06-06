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

#include "zapi/vm/Extension.h"
#include <map>
#include <string>

#ifdef ZTS
#  include "TSRM.h"
#endif

namespace zapi
{
namespace vm
{


// We're almost there, we now need to declare an instance of the
// structure defined above (if building for a single thread) or some
// sort of impossible to understand magic pointer-to-a-pointer (for
// multi-threading builds). We make this a static variable because
// this already is bad enough.
ZEND_DECLARE_MODULE_GLOBALS(zapi)

/**
 * Function that must be defined to initialize the "globals"
 * We do not have to initialize anything, but PHP needs to call this
 * method (crazy)
 *
 * @param zend_zapi_globals globals
 */
static void init_globals(zend_zapi_globals *globals);

// The *startup() and *shutdown() callback functions are passed a moduleNumber
// variable. However, there does not seem to be a decent API call in Zend to
// get back the original module_entry linked to this number. So we have to
// look up entries in a hash table to find the right module entry. To make things
// even worse, the records in this hash table are copies of the original
// zend_module_entry structure, so we can also not hide the C++ extension
// object pointer in the entry that we created ourselves.
// We have an ugly solution, we keep track of a map of all C++ extension names
// and their associated extension object, and a map of all module number and
// the linked extension object.
static std::map<std::string, Extension *> name2extension;
static std::map<int, Extension *> number2extension;

/**
 * Handler function that is used in combination with zend_hash_apply()
 *
 * This function is called when we need to find an extension object based on
 * an extension number. We loop through the list of all registered modules, and
 * for each module we check if we know the extension based on the name
 *
 * @param value
 * @return
 */
static int setup_number_module_map_handler(zval *value)
{
   // retrieve the module entry from the zval
   zend_module_entry *entry = static_cast<zend_module_entry *>(Z_PTR_P(value));
   std::map<std::string, Extension*>::iterator iter = name2extension.find(entry->name);
   if (iter == name2extension.end()) {
      return ZEND_HASH_APPLY_KEEP;
   }
   // we have the extension, store in combination with the number
   number2extension[entry->module_number] = iter->second;
   // done
   return ZEND_HASH_APPLY_KEEP;
}

/**
 *  Find an extension based on the module number
 *  @param int moduleNumber
 *  @return Extension*
 */
static Extension *find_extension_by_number(int moduleNumber)
{
   std::map<int, Extension *>::iterator iter = number2extension.find(moduleNumber);
   if (iter != number2extension.end()) {
      return iter->second;
   }
   // no, not yet, loop through all modules
   zend_hash_apply(&module_registry, setup_number_module_map_handler);
   iter = number2extension.find(moduleNumber);
   if (iter == number2extension.end()) {
      return nullptr;
   }
   return iter->second;
}

} // vm
} // zapi