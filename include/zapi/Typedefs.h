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

#ifndef ZAPI_TYPEDEFS_H
#define ZAPI_TYPEDEFS_H

#include <functional>

struct _zend_execute_data;
struct _zval_struct;

namespace zapi
{

namespace lang
{

class Parameters;
class Variant;
class StdClass;

} // lang

using HANDLE = void *;
using Callback = std::function<void()>;
using ZendCallable = void(*)(struct _zend_execute_data *executeData, struct _zval_struct *returnValue);

// native function signature type alias
using NativeCallable0 = void (*)();
using NativeCallable1 = void (*)(lang::Parameters &);
using NativeCallable2 = lang::Variant (*)();
using NativeCallable3 = lang::Variant (*)(lang::Parameters &);

// class method signature type alias
using MethodCallable0  = void (lang::StdClass::*)();
using MethodCallable1  = void (lang::StdClass::*)(lang::Parameters &);
using MethodCallable2  = lang::Variant (lang::StdClass::*)();
using MethodCallable3  = lang::Variant (lang::StdClass::*)(lang::Parameters &);
using MethodCallable4  = void (lang::StdClass::*)() const;
using MethodCallable5  = void (lang::StdClass::*)(lang::Parameters &) const;
using MethodCallable6  = lang::Variant (lang::StdClass::*)() const;
using MethodCallable7  = lang::Variant (lang::StdClass::*)(lang::Parameters &) const;
using MethodCallable8  = NativeCallable0;
using MethodCallable9  = NativeCallable1;
using MethodCallable10 = NativeCallable2;
using MethodCallable11 = NativeCallable3;

// class getter and setter signature type alias
using GetterMethodCallable0 = lang::Variant (lang::StdClass::*)();
using GetterMethodCallable1 = lang::Variant (lang::StdClass::*)() const;
using SetterMethodCallable0 = void (lang::StdClass::*)(const lang::Variant &value);
using SetterMethodCallable1 = void (lang::StdClass::*)(const lang::Variant &value) const;

} // zapi

#endif