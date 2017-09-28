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

#ifndef ZAPI_VM_ITERATOR_BRIDGE_H
#define ZAPI_VM_ITERATOR_BRIDGE_H

#include "zapi/Global.h"
#include "zapi/ds/Variant.h"

// forward declare with namespace
namespace zapi
{

namespace protocol
{
class AbstractIterator;
} // protocol

} // zapi
// end forward declare

namespace zapi
{
namespace vm
{

using zapi::protocol::AbstractIterator;
using zapi::ds::Variant;

class IteratorBridge
{
public:
   IteratorBridge(zval *object, AbstractIterator *iterator);
   ~IteratorBridge();
   zend_object_iterator *getZendIterator();
   static zend_object_iterator_funcs *getIteratorFuncs();
private:
   bool valid();
   Variant &current();
   Variant key();
   void next();
   void rewind();
   void invalidate();
private:
   static IteratorBridge *getSelfPtr(zend_object_iterator *iterator);
   static void destructor(zend_object_iterator *iterator);
   static int valid(zend_object_iterator *iterator);
   static zval *current(zend_object_iterator *iterator);
   static void key(zend_object_iterator *iterator, zval *data);
   static void next(zend_object_iterator *iterator);
   static void rewind(zend_object_iterator *iterator);
   static void invalidate(zend_object_iterator *iterator);
private:
   zend_object_iterator m_iterator;
   AbstractIterator *m_userspaceIterator;
   Variant m_current;
};

} // vm
} // zapi

#endif // ZAPI_VM_ITERATOR_BRIDGE_H
