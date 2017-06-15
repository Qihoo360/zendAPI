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

#include "php/Zend/zend_types.h"
#include "php/Zend/zend_variables.h"

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
   Undefined   = IS_UNDEF,  // Variable is not set
   Null        = IS_NULL,  // Null will allow any type
   False       = IS_FALSE,  // Boolean false
   True        = IS_TRUE,  // Boolean true
   Long        = IS_LONG,  // Integer type
   Double      = IS_DOUBLE,  // Floating point type
   String      = IS_STRING,  // A string obviously
   Array       = IS_ARRAY,  // An array of things
   Object      = IS_OBJECT,  // An object
   Resource    = IS_RESOURCE,  // A resource
   Reference   = IS_REFERENCE, // Reference to another value (can be any type!)
   Constant    = IS_CONSTANT, // A constant value
   ConstantAST = IS_CONSTANT_AST, // I think an Abstract Syntax tree, not quite sure
   // "fake types", not quite sure what that means
   Boolean     = _IS_BOOL, // You will never get this back as a type
   Callable    = IS_CALLABLE, // I don't know why this is a "fake" type
   // internal types
   Indirect    = IS_INDIRECT,
   Ptr         = IS_PTR,
   Error       = _IS_ERROR
};

using HashTableDataDeleter = dtor_func_t;

} // lang
} // zapi

#endif //ZAPI_LANG_TYPE_H
