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

#ifndef ZAPI_UTILS_COMMON_FUNCS_H
#define ZAPI_UTILS_COMMON_FUNCS_H

#include "zapi/Global.h"

namespace zapi
{
// forward declare with namespace
namespace ds
{
class Variant;
} // ds
} // zapi
// end forward declare

namespace zapi
{
namespace utils
{

ZAPI_DECL_EXPORT void std_php_memory_deleter(void *ptr);
ZAPI_DECL_EXPORT void std_zend_string_deleter(zend_string *str);
ZAPI_DECL_EXPORT void std_zend_string_force_deleter(zend_string *str);

ZAPI_DECL_EXPORT char *str_toupper(char *str) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT char *str_toupper(char *str, size_t length) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT std::string &str_toupper(std::string &str) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT char *str_tolower(char *str) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT char *str_tolower(char *str, size_t length) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT std::string &str_tolower(std::string &str) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT std::string get_zval_type_str(const zval *valuePtr) ZAPI_DECL_NOEXCEPT;
ZAPI_DECL_EXPORT bool zval_type_is_valid(const zval *valuePtr) ZAPI_DECL_NOEXCEPT;

struct ZAPI_DECL_EXPORT VariantKeyLess
{
   bool operator ()(const zapi::ds::Variant &lhs, const zapi::ds::Variant &rhs) const;
};

} // utils
} // zapi

#endif // ZAPI_UTILS_COMMON_FUNCS_H