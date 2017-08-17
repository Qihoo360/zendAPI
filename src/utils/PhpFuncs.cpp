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
// Created by zzu_softboy on 2017/08/10.

#include "zapi/utils/PhpFuncs.h"
#include "zapi/ds/ArrayItemProxy.h"
#include "zapi/ds/internal/ArrayItemProxyPrivate.h"
#include <string>

namespace zapi
{

using zapi::ds::ArrayItemProxy;
using zapi::ds::internal::ArrayItemProxyPrivate;

bool array_unset(ArrayItemProxy &&arrayItem)
{
   if (!arrayItem.isKeychianOk(false)) {
      return false;
   }
   // everything is ok
   // here we use the pointer to remove
   ArrayItemProxy::KeyType requestKey = arrayItem.m_implPtr->m_requestKey;
   zval *array = arrayItem.m_implPtr->m_array;
   int ret;
   if (requestKey.second) {
      std::string *key = requestKey.second.get();
      ret = zend_hash_str_del(Z_ARRVAL_P(array), key->c_str(), key->length());
   } else {
      ret = zend_hash_index_del(Z_ARRVAL_P(array), requestKey.first);
   }
   return ret == ZAPI_SUCCESS;
}

bool array_isset(ArrayItemProxy &&arrayItem)
{
   bool exist = false;
   if (arrayItem.m_implPtr->m_parent) {
      bool stop = false;
      arrayItem.checkExistRecursive(stop, arrayItem.m_implPtr->m_array, 
                                    arrayItem.m_implPtr->m_apiPtr, true);
      exist = !stop;
   } else {
      exist = nullptr != arrayItem.retrieveZvalPtr(true);
   }
   arrayItem.m_implPtr->m_array = nullptr;
   arrayItem.m_implPtr->m_needCheckRequestItem = false;
   return exist;
}

} // zapi