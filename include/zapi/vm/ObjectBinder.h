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
// Created by zzu_softboy on 2017/08/03.

#ifndef ZAPI_VM_OBJECT_BINDER_H
#define ZAPI_VM_OBJECT_BINDER_H

#include "zapi/Global.h"

namespace zapi
{

namespace lang
{
class StdClass;
} // lang

namespace vm
{

using zapi::lang::StdClass;
class ObjectBinder
{
public:
   ObjectBinder(zend_class_entry *entry, std::shared_ptr<StdClass> nativeObject,
                const zend_object_handlers *objectHandlers, uint32_t refCount);
   ~ObjectBinder();
   void destroy();
   zend_object *getZendObject() const;
   StdClass *getNativeObject() const;
   static ObjectBinder *retrieveSelfPtr(const zend_object *object);
   static ObjectBinder *retrieveSelfPtr(zval *object);
   static constexpr size_t calculateZendObjectOffset()
   {
      return offsetof(Container, m_zendObject);
   }

private:
   struct Container
   {
      ObjectBinder *m_self;
      zend_object m_zendObject;
   } *m_container;
   std::shared_ptr<StdClass> m_nativeObject;
};

} // vm
} // zapi

#endif // ZAPI_VM_OBJECT_BINDER_H
