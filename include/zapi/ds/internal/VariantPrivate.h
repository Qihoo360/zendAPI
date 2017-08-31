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
// Created by softboy on 2017/08/06.

#ifndef ZAPI_DS_INTERNAL_VARIANT_PRIVATE_H
#define ZAPI_DS_INTERNAL_VARIANT_PRIVATE_H

#include <type_traits>
#include "php/Zend/zend_types.h"
#include <memory>

namespace zapi
{
namespace ds
{
namespace internal
{

class VariantPrivate
{
public:
   VariantPrivate()
   {
      std::memset(&m_buffer, 0, sizeof(m_buffer));
   }

   operator _zval_struct *() const &;
   operator const _zval_struct *() const &;
   _zval_struct &operator*() const &;
   _zval_struct *dereference() const;
   std::aligned_storage<16>::type m_buffer;
};

} // internal
} // ds
} // zapi

#endif // ZAPI_DS_INTERNAL_VARIANT_PRIVATE_H