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

#ifndef ZAPI_UTILS_PHPFUNCS_H
#define ZAPI_UTILS_PHPFUNCS_H

#include "zapi/lang/Ini.h"
#include "php/ext/standard/info.h"

// forward declare with namespace
namespace zapi
{
namespace ds
{
class ArrayItemProxy;
class Variant;
} // ds

namespace lang
{
class Extension;
} // lang

} // zapi
// forward declare

namespace zapi
{

// here we define some php function that can been used in c++ space

ZAPI_DECL_EXPORT inline lang::IniValue ini_get(const char *name) 
{
   return lang::IniValue(name, false);
}

ZAPI_DECL_EXPORT inline lang::IniValue ini_get_orig(const char *name)
{
   return lang::IniValue(name, true);
}

ZAPI_DECL_EXPORT bool array_unset(zapi::ds::ArrayItemProxy &&arrayItem);
ZAPI_DECL_EXPORT bool array_isset(zapi::ds::ArrayItemProxy &&arrayItem);

ZAPI_DECL_EXPORT bool empty(const zapi::ds::Variant &value);
ZAPI_DECL_EXPORT std::string php_info_html_esc(const std::string &string);
ZAPI_DECL_EXPORT void php_info_html_esc_write(const std::string &string);
ZAPI_DECL_EXPORT void php_print_info_htmlhead(void);
ZAPI_DECL_EXPORT void php_print_info(int flag);
ZAPI_DECL_EXPORT void php_print_style(void);
ZAPI_DECL_EXPORT void php_info_print_style(void);
ZAPI_DECL_EXPORT void php_info_print_table_colspan_header(int numCols, const std::string &header);
ZAPI_DECL_EXPORT void php_info_print_table_start(void);
ZAPI_DECL_EXPORT void php_info_print_table_end(void);
ZAPI_DECL_EXPORT void php_info_print_box_start(int bg);
ZAPI_DECL_EXPORT void php_info_print_box_end(void);
ZAPI_DECL_EXPORT void php_info_print_hr(void);
ZAPI_DECL_EXPORT void php_info_print_module(const zapi::lang::Extension &extension);

template <typename... Args>
ZAPI_DECL_EXPORT void php_info_print_table_header(int numCols, Args&&... args)
{
   ::php_info_print_table_header(numCols, std::forward<Args>(args)...);
}

template <typename... Args>
ZAPI_DECL_EXPORT void php_info_print_table_row(int numCols, Args&&... args)
{
   ::php_info_print_table_row(numCols, std::forward<Args>(args)...);
}

template <typename... Args>
ZAPI_DECL_EXPORT void php_info_print_table_row_ex(int numCols, const char *str,  Args&&... args)
{
   ::php_info_print_table_row_ex(numCols, str, std::forward<Args>(args)...);
}

} // zapi

#endif // ZAPI_UTILS_PHPFUNCS_H
