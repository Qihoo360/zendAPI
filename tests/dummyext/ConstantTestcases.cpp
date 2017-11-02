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
// Created by softboy on 2017/11/02.

#include "ConstantTestcases.h"

namespace dummyext 
{

using zapi::lang::Constant;
using zapi::lang::Namespace;

void register_constant_testcases(Extension &extension)
{
   extension.registerConstant(Constant("MY_CONST", 12333));
   extension.registerConstant(Constant("PI", 3.14));
   Constant nameConst("ZAPI_NAME", "zapi");
   extension.registerConstant(nameConst);
   extension.registerConstant(Constant("ZAPI_VERSION", "v0.0.1"));
   extension.registerConstant(Constant("QIHOO", "beijing qihoo"));
   // register constant in namespace
   Namespace *zapi = extension.findNamespace("zapi");
   Namespace *io = zapi->findNamespace("io");
   io->registerConstant(Constant("IO_TYPE", "ASYNC"));
   io->registerConstant(Constant("NATIVE_STREAM", true));
   zapi->registerConstant(Constant("SYS_VERSION", "0.1.1-alpha"));
}

} // dummyext
