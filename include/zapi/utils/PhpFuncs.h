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
// Created by softboy on 7/25/17.

#ifndef ZAPI_UTILS_PHPFUNCS_H
#define ZAPI_UTILS_PHPFUNCS_H

#include "zapi/lang/IniEntry.h"

// forward declare with namespace
namespace zapi
{
namespace ds
{
class ArrayItemProxy;
class Variant;
} // ds
} // zapi
// forward declare

namespace zapi
{

// here we define some php function that can been used in c++ space

ZAPI_DECL_EXPORT inline lang::IniValue ini_get(const char *name) 
{
   return lang::IniValue(name, false);
}

ZAPI_DECL_EXPORT inline lang::IniValue ini_get_orig(const char *name)
{
   return lang::IniValue(name, true);
}

ZAPI_DECL_EXPORT bool array_unset(zapi::ds::ArrayItemProxy &&arrayItem);
ZAPI_DECL_EXPORT bool array_isset(zapi::ds::ArrayItemProxy &&arrayItem);

ZAPI_DECL_EXPORT bool empty(const zapi::ds::Variant &value);

} // zapi

#endif // ZAPI_UTILS_PHPFUNCS_H