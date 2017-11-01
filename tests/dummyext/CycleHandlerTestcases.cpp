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

#include "CycleHandlerTestcases.h"
#include <vector>
#include <string>

namespace dummyext 
{

using zapi::ds::ArrayVariant;

static std::vector<std::string> infos;

void register_cyclehandler_testcases(Extension &extension)
{
   extension.setInfoHandler(&dummyext::module_info_handler);
   extension.setStartupHandler(&dummyext::startup_handler);
   extension.setShutdownHandler(&dummyext::shutdown_handler);
   extension.setRequestStartupHandler(&dummyext::request_startup_handler);
   extension.setRequestShutdownHandler(&dummyext::request_shutdown_handler);
   extension.registerFunction<decltype(&dummyext::get_ext_handler_msgs), &dummyext::get_ext_handler_msgs>("get_ext_handler_msgs");
}

void startup_handler()
{
   add_mhandler_info("module startup handler called");
}

void shutdown_handler()
{
   add_mhandler_info("module shutdown handler called");
   //   assert(infos.size() == 5);
   //   assert(infos[2] == "module info handler called");
   //   assert(infos[3] == "request shutdown handler called");
   //   assert(infos[4] == "module shutdown handler called");
}

void request_startup_handler()
{
   add_mhandler_info("request startup handler called");
}

void request_shutdown_handler()
{
   add_mhandler_info("request shutdown handler called");
}

void module_info_handler()
{
   add_mhandler_info("module info handler called");
   zapi::php_info_print_table_start();
   zapi::php_info_print_table_row(2, "dummyext support", "enabled");
   zapi::php_info_print_table_row(2, "dummyext team", "qcoreteam");
   zapi::php_info_print_table_end();
}

void add_mhandler_info(const std::string &msg)
{
   infos.push_back(msg);
}

Variant get_ext_handler_msgs()
{
   ArrayVariant ret;
   auto begin = infos.begin();
   while (begin != infos.end()) {
      ret.append(*begin);
      begin++;
   }
   return ret;
}

} // dummyext
