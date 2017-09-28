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
// Created by zzu_softboy on 2017/08/10.

#include "zapi/utils/PhpFuncs.h"
#include "zapi/ds/ArrayItemProxy.h"
#include "zapi/ds/internal/ArrayItemProxyPrivate.h"
#include "zapi/ds/Variant.h"
#include "zapi/lang/Extension.h"
#include <string>

namespace zapi
{

using zapi::ds::ArrayItemProxy;
using zapi::ds::internal::ArrayItemProxyPrivate;
using zapi::ds::Variant;

bool array_unset(ArrayItemProxy &&arrayItem)
{
   if (!arrayItem.isKeychianOk(false)) {
      return false;
   }
   // everything is ok
   // here we use the pointer to remove
   ArrayItemProxy::KeyType requestKey = arrayItem.m_implPtr->m_requestKey;
   zval *array = arrayItem.m_implPtr->m_array;
   int ret;
   if (requestKey.second) {
      std::string *key = requestKey.second.get();
      ret = zend_hash_str_del(Z_ARRVAL_P(array), key->c_str(), key->length());
   } else {
      ret = zend_hash_index_del(Z_ARRVAL_P(array), requestKey.first);
   }
   return ret == ZAPI_SUCCESS;
}

bool array_isset(ArrayItemProxy &&arrayItem)
{
   bool exist = false;
   if (arrayItem.m_implPtr->m_parent) {
      bool stop = false;
      arrayItem.checkExistRecursive(stop, arrayItem.m_implPtr->m_array, 
                                    arrayItem.m_implPtr->m_apiPtr, true);
      exist = !stop;
   } else {
      exist = nullptr != arrayItem.retrieveZvalPtr(true);
   }
   arrayItem.m_implPtr->m_array = nullptr;
   arrayItem.m_implPtr->m_needCheckRequestItem = false;
   return exist;
}

bool empty(const Variant &value)
{
   return value.isNull() || !value.toBool();
}

std::string php_info_html_esc(const std::string &string)
{
   zend_string *zstring = ::php_info_html_esc(const_cast<char *>(string.c_str()));
   if (zstring) {
      return std::string(ZSTR_VAL(zstring), ZSTR_LEN(zstring));
   }
   return std::string();
}

void php_info_html_esc_write(const std::string &string)
{
   ::php_info_html_esc_write(const_cast<char *>(string.c_str()), string.size());
}

void php_print_info_htmlhead(void)
{
   ::php_print_info_htmlhead();
}

void php_print_info(int flag)
{
   ::php_print_info(flag);
}

void php_print_style(void)
{
   ::php_print_style();
}

void php_info_print_style(void)
{
   ::php_info_print_style();
}

void php_info_print_table_colspan_header(int numCols, const std::string &header)
{
   ::php_info_print_table_colspan_header(numCols, const_cast<char *>(header.c_str()));
}

void php_info_print_table_start(void)
{
   ::php_info_print_table_start();
}

void php_info_print_table_end(void)
{
   ::php_info_print_table_end();
}

void php_info_print_box_start(int bg)
{
   ::php_info_print_box_start(bg);
}

void php_info_print_box_end(void)
{
   ::php_info_print_box_end();
}

void php_info_print_hr(void)
{
   ::php_info_print_hr();
}

void php_info_print_module(const zapi::lang::Extension &extension)
{
   ::php_info_print_module(reinterpret_cast<zend_module_entry *>(const_cast<void *>(extension.getModule())));
}

} // zapi
