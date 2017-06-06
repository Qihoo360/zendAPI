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
// Created by zzu_softboy on 06/06/2017.

#ifndef ZAPI_VM_ZVAL_H
#define ZAPI_VM_ZVAL_H

#include "zapi/Global.h"

struct _zval_struct;

namespace zapi
{
namespace vm
{

using AlignedZval = typename std::aligned_storage<16>::type;

class Zval
{
private:
   /**
    * The zval buffer
    * @var AlignedZval
    */
   AlignedZval m_buffer;

public:
   /**
    * Cast to a zval
    *
    * @return zend value structure
    */
   operator _zval_struct * () const &
   {
      return const_cast<_zval_struct *>(reinterpret_cast<const _zval_struct *>(&m_buffer));
   }

   /**
    *  Cast to a zval
    *
    *  @return zend value structure
    */
   _zval_struct &operator* () const &
   {
      return *const_cast<_zval_struct *>(reinterpret_cast<const _zval_struct *>(&m_buffer));
   }

   /**
    *  In case this is a reference, dereference it into a zval.. In case it isn't
    *  the actual value is returned.
    *
    *  @return _zval_struct*
    */
   _zval_struct *dereference() const;
};

} // vm
} // zapi

#endif //ZAPI_VM_ZVAL_H
