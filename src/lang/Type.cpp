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
// Created by softboy on 2017/07/26.

#include "zapi/lang/Type.h"

namespace zapi
{
namespace lang
{

Modifier operator~(Modifier modifier)
{
   return static_cast<Modifier>(~static_cast<unsigned int>(modifier));
}

Modifier operator|(Modifier left, Modifier right)
{
   return static_cast<Modifier>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
}

Modifier operator&(Modifier left, Modifier right)
{
   return static_cast<Modifier>(static_cast<unsigned int>(left) & static_cast<unsigned int>(right));
}

Modifier &operator|=(Modifier &left, Modifier right)
{
   left = static_cast<Modifier>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
   return left;
}

Modifier &operator&=(Modifier &left, Modifier right)
{
   left = static_cast<Modifier>(static_cast<unsigned int>(left) & static_cast<unsigned int>(right));
   return left;
}

bool operator==(unsigned long value, Modifier right)
{
   return static_cast<unsigned long>(right) == value;
}

bool operator==(Modifier left, unsigned long value)
{
   return static_cast<unsigned long>(left) == value;
}

bool operator==(Modifier left, Modifier right)
{
   return static_cast<unsigned long>(left) == static_cast<unsigned long>(right);
}

} // lang
} // zapi
