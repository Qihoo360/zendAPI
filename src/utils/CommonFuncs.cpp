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
// Created by zzu_softboy on 2017/08/10.

#include "zapi/utils/CommonFuncs.h"
#include <string>
#include <cstring>
#include <cctype>

namespace zapi
{
namespace utils
{

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

} // utils
} // zapi