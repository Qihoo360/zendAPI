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
// Created by zzu_softboy on 27/05/2017.

#include "zapi/lang/Parameters.h"
#include "zapi/vm/ObjectBinder.h"

namespace zapi
{
namespace lang
{

using zapi::vm::ObjectBinder;

Parameters::Parameters(zval *thisPtr, uint32_t argc)
   : Parameters(nullptr != thisPtr ? ObjectBinder::retrieveSelfPtr(thisPtr)->getNativeObject() : nullptr)
{
   m_data.reserve(argc);
   zval arguments[argc];
   zend_get_parameters_array_ex(argc, arguments);
   for (uint32_t i = 0; i < argc; i++)
   {
      // append value
      m_data.emplace_back(&arguments[i]);
   }
}

Parameters::Reference Parameters::at(SizeType pos)
{
   return m_data.at(pos);
}

Parameters::ConstReference Parameters::at(SizeType pos) const
{
   return m_data.at(pos);   
}

bool Parameters::empty() const ZAPI_DECL_NOEXCEPT
{
   return m_data.empty();
}

Parameters::SizeType Parameters::size() const ZAPI_DECL_NOEXCEPT
{
   return m_data.size();
}

} // lang
} // zapi
