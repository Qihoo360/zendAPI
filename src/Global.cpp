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
// Created by zzu_softboy on 05/06/2017.

#include "zapi/Global.h"

#include <iostream>
#include "zapi/kernel/StreamBuffer.h"

namespace zapi
{

void assert_x(const char *where, const char *what, const char *file, int line) ZAPI_DECL_NOEXCEPT
{
   std::cerr <<  "ASSERT failure in " << where << ": \"" << what 
              << "\", file " << file << ", line "<< line << std::endl;
}

static thread_local kernel::StreamBuffer bufferOut(0);
static thread_local kernel::StreamBuffer bufferError(E_ERROR);
static thread_local kernel::StreamBuffer bufferWarning(E_WARNING);
static thread_local kernel::StreamBuffer bufferNotice(E_NOTICE);
static thread_local kernel::StreamBuffer bufferDeprecated(E_DEPRECATED);

thread_local std::ostream out(&bufferOut);
thread_local std::ostream error(&bufferError);
thread_local std::ostream warning(&bufferWarning);
thread_local std::ostream notice(&bufferNotice);
thread_local std::ostream deprecated(&bufferDeprecated);

} // zapi