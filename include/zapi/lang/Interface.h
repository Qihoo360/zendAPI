// @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
// @copyright 2014 Copernica BV
//
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
// Created by softboy on 2017/08/02.

#ifndef ZAPI_LANG_INTERFACE_H
#define ZAPI_LANG_INTERFACE_H

#include "zapi/vm/AbstractClass.h"

namespace zapi
{
namespace lang
{

using zapi::vm::AbstractClass;

class ZAPI_DECL_EXPORT Interface final : public AbstractClass
{
public:
   Interface(const char *name);
   virtual ~Interface();
public:
   Interface &registerMethod(const char *name, const Arguments args = {});
   Interface &registerMethod(const char *name, Modifier flags, const Arguments args = {});
   Interface &registerBaseInterface(const Interface &interface);
protected:
   template<typename ANYTHING> friend class Class;
};

} // lang
} // zapi

#endif //ZAPI_LANG_INTERFACE_H
