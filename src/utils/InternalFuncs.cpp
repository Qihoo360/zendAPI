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

#include "zapi/utils/InternalFuncs.h"

namespace zapi 
{
namespace internal 
{

bool parse_namespaces(const std::string &ns, std::list<std::string> &parts)
{
   int markPos = 0;
   int cycleSepCount = 0;
   std::string::value_type curChar;
   std::string::size_type len = ns.size();
   if (0 == len) {
      return true;
   }
   if (ns[len - 1] == ':') {
      return false;
   }
   int i = 0;
   for (; i < ns.size(); i++) {
      curChar = ns[i];
      if (':' == curChar) {
         if (cycleSepCount == 0 && (i - markPos > 0)) {
           parts.push_back(ns.substr(markPos, i - markPos));
         }
         cycleSepCount++;
      } else {
         if (cycleSepCount == 1 || cycleSepCount > 2) {
            parts.clear();
            return false;
         } else if (cycleSepCount == 2) {
            markPos = i;
            cycleSepCount = 0;
         }
      }
   }
   if (i > markPos) {
      parts.push_back(ns.substr(markPos, i - markPos));
   }
   return true;
}

}
}
