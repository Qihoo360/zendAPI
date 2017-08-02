// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
// Copyright 2013, 2014 Copernica BV
// Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
// Created by softboy on 2017/08/01.

#ifndef ZAPI_LANG_PROPERTY_H
#define ZAPI_LANG_PROPERTY_H

#include "zapi/Global.h"

namespace zapi
{
namespace lang
{

namespace internal
{
class PropertyPrivate;
} // internal

using internal::PropertyPrivate;

class ZAPI_DECL_EXPORT Property
{
public:
   Property(const zapi::GetterMethodCallable0 &getter);
   Property(const zapi::GetterMethodCallable1 &getter);
   Property(const zapi::SetterMethodCallable0 &setter);
   Property(const zapi::SetterMethodCallable1 &setter);
   Property(const zapi::GetterMethodCallable0 &getter, const zapi::SetterMethodCallable0 &setter);
   Property(const zapi::GetterMethodCallable0 &getter, const zapi::SetterMethodCallable1 &setter);
   Property(const zapi::GetterMethodCallable1 &getter, const zapi::SetterMethodCallable0 &setter);
   Property(const zapi::GetterMethodCallable1 &getter, const zapi::SetterMethodCallable1 &setter);
   Property(const Property &other) = delete;
   virtual ~Property();
private:
   ZAPI_DECLARE_PRIVATE(Property)
   std::unique_ptr<PropertyPrivate> m_implPtr;   
};

} // lang
} // zapi

#endif // ZAPI_LANG_PROPERTY_H