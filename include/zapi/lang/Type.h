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
// Created by softboy on 5/26/17.

#ifndef ZAPI_LANG_TYPE_H
#define ZAPI_LANG_TYPE_H

namespace zapi
{
namespace lang
{

/**
 * Supported types for variables
 * The values are the same as the ones used internally in Zend
 */
enum class Type : unsigned char
{
   Undefined   = 0,  // Variable is not set
   Null        = 1,  // Null will allow any type
   False       = 2,  // Boolean false
   True        = 3,  // Boolean true
   Numeric     = 4,  // Integer type
   Float       = 5,  // Floating point type
   String      = 6,  // A string obviously
   Array       = 7,  // An array of things
   Object      = 8,  // An object
   Resource    = 9,  // A resource
   Reference   = 10, // Reference to another value (can be any type!)
   Constant    = 11, // A constant value
   ConstantAST = 12, // I think an Abstract Syntax tree, not quite sure
   // "fake types", not quite sure what that means
   Bool        = 13, // You will never get this back as a type
   Callable    = 14 // I don't know why this is a "fake" type
};

} // lang
} // zapi

#endif //ZAPI_LANG_TYPE_H
