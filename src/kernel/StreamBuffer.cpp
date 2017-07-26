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
// Created by softboy on 26/07/2017.

#include <string>
#include "zapi/kernel/StreamBuffer.h"

namespace zapi
{
namespace kernel
{

StreamBuffer::StreamBuffer(int error)
   : m_error(error)
{
   setp(m_buffer.begin(), m_buffer.end());
}

int StreamBuffer::overflow(int c)
{
   std::char_traits<char>::int_type eof = std::char_traits<char>::eof();
   if (m_error) {
      return c;
   }
   if (c == eof) {
      return sync(), eof;
   }
   *pptr() = c;
   pbump(1);
   return sync() == -1 ? eof : c;
}

int StreamBuffer::sync()
{
   size_t size = pptr() - pbase();
   if (m_error) {
      zend_error(m_error, "%.*s", static_cast<int>(size), pbase());
   } else {
      zend_write(pbase(), size);
   }
   // reset the buffer
   pbump(-size);
   return 0;
}

} // kernel
} // zapi