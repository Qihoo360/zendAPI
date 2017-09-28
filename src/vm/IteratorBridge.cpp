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

#include "zapi/vm/IteratorBridge.h"
#include "zapi/protocol/AbstractIterator.h"

namespace zapi
{
namespace vm
{

IteratorBridge::IteratorBridge(zval *object, AbstractIterator *iterator)
   : m_userspaceIterator(iterator)
{
   zend_iterator_init(&m_iterator);
   ZVAL_COPY(&m_iterator.data, object);
   m_iterator.funcs = getIteratorFuncs();
}

IteratorBridge::~IteratorBridge()
{
   invalidate();
   zval_ptr_dtor(&m_iterator.data);
   zend_iterator_dtor(&m_iterator);
}

zend_object_iterator *IteratorBridge::getZendIterator()
{
   return &m_iterator;
}

zend_object_iterator_funcs *IteratorBridge::getIteratorFuncs()
{
   static zend_object_iterator_funcs funcs;
   static bool initialized = false;
   if (initialized) {
      return &funcs;
   }
   funcs.dtor = &IteratorBridge::destructor;
   funcs.valid = &IteratorBridge::valid;
   funcs.get_current_data = &IteratorBridge::current;
   funcs.get_current_key = &IteratorBridge::key;
   funcs.move_forward = &IteratorBridge::next;
   funcs.rewind = &IteratorBridge::rewind;
   funcs.invalidate_current = &IteratorBridge::invalidate;
   initialized = true;
   return &funcs;   
}

bool IteratorBridge::valid()
{
   return m_userspaceIterator->valid();
}

Variant &IteratorBridge::current()
{
   return m_current = m_userspaceIterator->current();
}

Variant IteratorBridge::key()
{
   return m_userspaceIterator->key();
}

void IteratorBridge::next()
{
   return m_userspaceIterator->next();
}

void IteratorBridge::rewind()
{
   return m_userspaceIterator->rewind();
}

void IteratorBridge::invalidate()
{
   m_current.invalidate();
}

IteratorBridge *IteratorBridge::getSelfPtr(zend_object_iterator *iterator)
{
   return reinterpret_cast<IteratorBridge *>(iterator);
}

void IteratorBridge::destructor(zend_object_iterator *iterator)
{
   getSelfPtr(iterator)->~IteratorBridge();
}

int IteratorBridge::valid(zend_object_iterator *iterator)
{
   return getSelfPtr(iterator)->valid() ? ZAPI_SUCCESS : ZAPI_FAILURE;
}

zval *IteratorBridge::current(zend_object_iterator *iterator)
{
   return getSelfPtr(iterator)->current().getZvalPtr();
}

void IteratorBridge::key(zend_object_iterator *iterator, zval *data)
{
   Variant retValue(getSelfPtr(iterator)->key());
   zval val = retValue.detach(true);
   ZVAL_ZVAL(data, &val, 1, 1);
}

void IteratorBridge::next(zend_object_iterator *iterator)
{
   getSelfPtr(iterator)->next();
}

void IteratorBridge::rewind(zend_object_iterator *iterator)
{
   getSelfPtr(iterator)->rewind();
}

void IteratorBridge::invalidate(zend_object_iterator *iterator)
{
   getSelfPtr(iterator)->invalidate();
}

} // vm
} // zapi
