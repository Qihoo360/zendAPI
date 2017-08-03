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
// Created by zzu_softboy on 2017/06/08.

#include "zapi/lang/StdClass.h"
#include "zapi/lang/Variant.h"
#include "zapi/lang/internal/StdClassPrivate.h"
#include "zapi/kernel/NotImplemented.h"

namespace zapi
{
namespace lang
{

namespace internal
{

StdClassPrivate::StdClassPrivate()
{}

} // internal

using zapi::kernel::NotImplemented;

StdClass::StdClass()
   : m_implPtr(new StdClassPrivate)
{}

StdClass::~StdClass()
{}

StdClass::StdClass(const StdClass &object)
   : m_implPtr(nullptr)
{}

/**
 * Overridable method that is called right before an object is destructed
 */
void StdClass::__destruct() const
{
   throw NotImplemented();
}

/**
 * Overridable method that is called to check if a property is set
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param  key
 * @return bool
 */
bool StdClass::__isset(const Variant &key) const
{
   throw NotImplemented();
}

/**
 * Overridable method that is called to set a new property
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param key
 * @param value
 */
void StdClass::__set(const Variant &key, const Variant &value) const
{
   throw NotImplemented();
}

/**
 * Retrieve a property
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param  key
 * @return value
 */
Variant StdClass::__get(const Variant &key) const
{
   throw NotImplemented();
}

/**
 * Remove a member
 *
 * The default implementation does nothing, and the script will fall back
 * to accessing the regular object properties
 *
 * @param key
 */
void StdClass::__unset(const Variant &key) const
{
   throw NotImplemented();
}

/**
 * Call a method
 *
 * This method is called when a method is called from the PHP script that
 * was not explicitly defined. You can use this to catch variable method
 * names, or to support all thinkable method names.
 *
 * @param  method      Name of the method that was called
 * @param  params      The parameters that were passed to the function
 * @return Value       The return value
 */
Variant StdClass::__call(const char *method, Parameters &params) const
{
   throw NotImplemented();
}

/**
 * Call the class as if it was a function
 *
 * This method is called when a an object is used with () operators:
 * $object(). You can override this method to make objects callable.
 *
 * @param  params      The parameters that were passed to the function
 * @return Value       The return value
 */
Variant StdClass::__invoke(Parameters &params) const
{
   throw NotImplemented();
}

/**
 * Cast the object to a string
 *
 * This method is called when an object is casted to a string, or when
 * it is used in a string context
 *
 * @return Value       The object as a string
 */
Variant StdClass::__toString() const
{
   throw NotImplemented();
}

/**
 * Cast the object to an integer
 *
 * This method is called when an object is casted to an integer, or when
 * it is used in an integer context
 *
 * @return int         Integer value
 */
Variant StdClass::__toInteger() const
{
   throw NotImplemented();
}

/**
 * Cast the object to a float
 *
 * This method is called when an object is casted to a float, or when it
 * is used in a float context
 *
 * @return double      Floating point value
 */
Variant StdClass::__toDouble() const
{
   throw NotImplemented();
}

/**
 * Cast the object to a boolean
 *
 * This method is called when an object is casted to a bool, or when it
 * is used in a boolean context
 *
 * @return bool
 */
Variant StdClass::__toBool() const
{
   throw NotImplemented();
}

/**
 * Compare the object with a different object
 *
 * Check how a different object compares to this object
 *
 * @param  that        Object to compare with
 * @return int
 */
int StdClass::__compare(const StdClass &object) const
{
   throw NotImplemented();
}

} // lang
} // zapi