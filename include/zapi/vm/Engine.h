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
// Created by zzu_softboy on 26/07/2017.

#ifndef ZAPI_VM_ENGING_H
#define ZAPI_VM_ENGING_H

#include "zapi/Global.h"
#include "php/Zend/zend_compile.h"

// forward declare with namespace
namespace zapi
{

namespace ds
{
class Variant;
} // ds
} // zapi
// end forward declare

namespace zapi
{
namespace vm
{

using zapi::ds::Variant;

class ZAPI_DECL_EXPORT Engine
{
public:
   
   enum class ScriptExecuteType : unsigned int
   {
      Eval        = ZEND_EVAL,
      Include     = ZEND_INCLUDE,
      IncludeOnce = ZEND_INCLUDE_ONCE,
      Require     = ZEND_REQUIRE,
      RequireOnce = ZEND_REQUIRE_ONCE
   };

public:
   static Variant eval(const std::string &code);
private:
   static Variant executeOpCodes(std::shared_ptr<zend_op_array> opcodes);
};

} // vm
} // zapi

#endif // ZAPI_VM_ENGING_H