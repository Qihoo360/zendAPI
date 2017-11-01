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

#include "zapi/ZendApi.h"
#include "IniTestcases.h"
#include "CycleHandlerTestcases.h"
#include "NamespaceTestcases.h"
#include "FunctionTestcases.h"
#include "ClassTestcases.h"

using zapi::lang::Extension;

extern "C" {

ZAPI_DECL_EXPORT void *get_module() 
{
   static Extension extension("dummyext", "1.0");
   dummyext::register_ini_testcases(extension);
   dummyext::register_cyclehandler_testcases(extension);
   dummyext::register_namespace_testcases(extension);
   dummyext::register_function_testcases(extension);
   dummyext::register_class_testcases(extension);
   return extension;
}

}
