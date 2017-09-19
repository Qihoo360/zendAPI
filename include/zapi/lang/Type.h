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

#include "zapi/PhpHeaders.h"

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
   Numeric     = IS_LONG,  // alias for Type::Long
   Float       = IS_DOUBLE, // alias for Type::Double
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

enum class ClassType : unsigned int
{
   Regular   = 0x00,
   Abstract  = ZEND_ACC_EXPLICIT_ABSTRACT_CLASS, //0x20,
   Final     = ZEND_ACC_FINAL, //0x04,
   Interface = ZEND_ACC_INTERFACE, //0x40
   Trait     = ZEND_ACC_TRAIT // 0x80
};

enum class  Modifier : unsigned long
{
   None              = 0,
   Static            = ZEND_ACC_STATIC,
   Abstract          = ZEND_ACC_ABSTRACT,
   Final             = ZEND_ACC_FINAL,
   Public            = ZEND_ACC_PUBLIC,
   Protected         = ZEND_ACC_PROTECTED,
   Private           = ZEND_ACC_PRIVATE,
   Const             = 0x10000, // we define this number ourself
   MethodModifiers   = Final | Public | Protected | Private | Static,
   PropertyModifiers = Final | Public | Protected | Private | Const | Static,
   // special method type
   Constructor       = ZEND_ACC_CTOR,
   Destructor        = ZEND_ACC_DTOR,
   Clone             = ZEND_ACC_CLONE
};

ZAPI_DECL_EXPORT Modifier operator~(Modifier modifier);
ZAPI_DECL_EXPORT Modifier operator|(Modifier left, Modifier right);
ZAPI_DECL_EXPORT Modifier operator&(Modifier left, Modifier right);
ZAPI_DECL_EXPORT Modifier &operator|=(Modifier &left, Modifier right);
ZAPI_DECL_EXPORT Modifier &operator&=(Modifier &left, Modifier right);
ZAPI_DECL_EXPORT bool operator==(const Modifier left, int value);
ZAPI_DECL_EXPORT bool operator==(int value, const Modifier &right);
ZAPI_DECL_EXPORT bool operator==(Modifier left, Modifier right);

using HashTableDataDeleter = dtor_func_t;

} // lang
} // zapi

#endif //ZAPI_LANG_TYPE_H
