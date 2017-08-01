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
// Created by zzu_softboy on 27/05/2017.

#ifndef ZAPI_KERNEL_STREAMBUFFER_H
#define ZAPI_KERNEL_STREAMBUFFER_H

#include <streambuf>
#include <array>
#include <ostream>
#include <mutex>

#include "zapi/Global.h"

namespace zapi
{
namespace kernel
{

class ZAPI_DECL_EXPORT StreamBuffer : public std::streambuf
{
public:
   StreamBuffer(int error);
   StreamBuffer(const StreamBuffer &buffer) = delete;
   StreamBuffer(StreamBuffer &&buffer) = delete;
   StreamBuffer &operator=(const StreamBuffer &buffer) = delete;
   StreamBuffer &operator=(StreamBuffer &&buffer) = delete;
   virtual ~StreamBuffer();
protected:
   virtual int overflow(int c = EOF) override;
   virtual int sync() override;
   
private:
   int m_error;
   std::array<char, 1024> m_buffer{};
   static std::mutex m_mutex;
};

} // kernel
} // zapi

#endif // ZAPI_KERNEL_STREAMBUFFER_H
