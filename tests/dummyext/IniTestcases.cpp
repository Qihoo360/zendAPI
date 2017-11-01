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
// Created by softboy on 2017/11/01.

#include "IniTestcases.h"
#include "zapi/lang/Ini.h"

namespace dummyext 
{

void register_ini_testcases(Extension &extension)
{
   // this have no effect we write this in php.ini
   extension.registerIni(zapi::lang::Ini("zapi_author", "xiuxiu"));
   // rewrite in php.ini
   extension.registerIni(zapi::lang::Ini("zapi_team_address", "beijing"));
   // register but empty value
   extension.registerIni(zapi::lang::Ini("zapi_product", ""));
   extension.registerIni(zapi::lang::Ini("zapi_enable_gpu", "off"));
   extension.registerIni(zapi::lang::Ini("zapi_name", "zzu_xiuxiu"));
   extension.registerIni(zapi::lang::Ini("zapi_age", 1));
   // int type
   extension.registerIni(zapi::lang::Ini("zapi_string_value", "stringvalue"));
   extension.registerIni(zapi::lang::Ini("zapi_int_value", 2017));
   extension.registerIni(zapi::lang::Ini("zapi_bool_value", true));
   extension.registerIni(zapi::lang::Ini("zapi_double_value", 3.14));
}

} // dummyext
