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

#include "zapi/utils/CommonFuncs.h"
#include "zapi/ds/Variant.h"
#include "zapi/lang/Type.h"
#include <string>
#include <cstring>
#include <cctype>

namespace zapi
{
namespace utils
{

using zapi::ds::Variant;
using zapi::lang::Type;

void std_php_memory_deleter(void *ptr)
{
   efree(ptr);  
}

void std_zend_string_deleter(zend_string *str)
{
   zend_string_release(str);  
}

void std_zend_string_force_deleter(zend_string *str)
{
   zend_string_free(str);  
}

char *str_toupper(char *str) ZAPI_DECL_NOEXCEPT
{
   return str_tolower(str, std::strlen(str));
}

char *str_toupper(char *str, size_t length) ZAPI_DECL_NOEXCEPT
{
   char *ptr = str;
   while (length--) {
      *ptr = std::toupper(*ptr);
      ptr++;
   }
   return str;
}

std::string &str_toupper(std::string &str) ZAPI_DECL_NOEXCEPT
{
   str_toupper(const_cast<char *>(str.data()), str.length());
   return str;
}

char *str_tolower(char *str) ZAPI_DECL_NOEXCEPT
{
   return str_tolower(str, std::strlen(str));
}

char *str_tolower(char *str, size_t length) ZAPI_DECL_NOEXCEPT
{
   char *ptr = str;
   while (length--) {
      *ptr = std::tolower(*ptr);
      ptr++;
   }
   return str;
}

std::string &str_tolower(std::string &str) ZAPI_DECL_NOEXCEPT
{
   str_tolower(const_cast<char *>(str.data()), str.length());
   return str;
}

std::string get_zval_type_str(const zval *valuePtr) ZAPI_DECL_NOEXCEPT
{
   switch (Z_TYPE_P(valuePtr)) {
   case IS_ARRAY:
      return "Array";
   case IS_TRUE:
   case IS_FALSE:
   case _IS_BOOL:
      return "Boolean";
   case IS_CALLABLE:
      return "Callable";
   case IS_CONSTANT:
      return "Constant";
   case IS_DOUBLE:
      return "Double";
   case IS_LONG:
      return "Numeric";
   case IS_OBJECT:
      return "Object";
   case IS_REFERENCE:
      return "Reference";
   case IS_RESOURCE:
      return "Resource";
   case IS_STRING:
      return "String";
   case IS_PTR:
      return "Pointer";
   default:
      return "Unknow";
   }
}

bool zval_type_is_valid(const zval *valuePtr) ZAPI_DECL_NOEXCEPT
{
   switch (Z_TYPE_P(valuePtr)) {
   case IS_UNDEF:
   case IS_ARRAY:
   case IS_TRUE:
   case IS_FALSE:
   case _IS_BOOL:
   case IS_CALLABLE:
   case IS_CONSTANT:
   case IS_DOUBLE:
   case IS_LONG:
   case IS_OBJECT:
   case IS_REFERENCE:
   case IS_RESOURCE:
   case IS_STRING:
   case IS_PTR:
      return true;
   default:
      return false;
   }
}

bool VariantKeyLess::operator ()(const zapi::ds::Variant &lhs, const zapi::ds::Variant &rhs) const
{
   Type ltype = lhs.getType();
   Type rtype = rhs.getType();
   bool result;
   if (ltype == Type::String && rtype == Type::String) {
      result = std::strcmp(Z_STRVAL_P(lhs.getZvalPtr()), Z_STRVAL_P(rhs.getZvalPtr())) < 0;
   } else if (ltype == Type::String && rtype == Type::Long) {
      zval temp;
      ZVAL_COPY_VALUE(&temp, rhs.getZvalPtr());
      convert_to_string(&temp);
      result = std::strcmp(Z_STRVAL_P(lhs.getZvalPtr()), Z_STRVAL_P(&temp)) < 0;
      zval_dtor(&temp);
   } else if (ltype == Type::Long && rtype == Type::String) {
      zval temp;
      ZVAL_COPY_VALUE(&temp, lhs.getZvalPtr());
      convert_to_string(&temp);
      result = std::strcmp(Z_STRVAL_P(&temp), Z_STRVAL_P(rhs.getZvalPtr())) < 0;
      zval_dtor(&temp);
   } else {
      result = Z_LVAL_P(lhs.getZvalPtr()) < Z_LVAL_P(rhs.getZvalPtr());
   }
   return result;
}

} // utils
} // zapi
