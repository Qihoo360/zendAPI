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
// Created by softboy on 18/05/2017.
//

#ifndef ZAPI_GLOBAL_H
#define ZAPI_GLOBAL_H

#include "php/main/php.h"
#include "php/Zend/zend_API.h"
#include "php/Zend/zend_types.h"

#include "zapi/Version.h"
#include "zapi/Config.h"
#include "zapi/lang/Type.h"

#include <type_traits>
#include <cstddef>
#include <memory>
#include <functional>
#include <cstdint>

#define ZAPI_STRINGIFY2(x) #x
#define ZAPI_STRINGIFY(x) ZAPI_STRINGIFY2(x)

#include "zapi/SystemDetection.h"
#include "zapi/ProcessorDetection.h"
#include "zapi/CompilerDetection.h"

#if defined(__ELF__)
#  define ZAPI_OF_ELF
#endif

#if defined(__MACH__) && defined(__APPLE__)
#  define ZAPI_OF_MACH_O
#endif

template <typename T>
static inline T *zapi_get_ptr_helper(T *ptr)
{
   return ptr;
}

template <typename T>
static inline typename T::pointer zapi_get_ptr_helper(const T &p)
{
   return p.data();
}

#define ZAPI_DECLARE_PRIVATE(Class)\
   inline Class##Private* getImplPtr()\
   {\
      return reinterpret_cast<Class##Private *>(zapi_get_ptr_helper(implPtr))\
   }\
   inline const Class##Private* getImplPtr() const\
   {\
      return reinterpret_cast<const Class##Provate *>(zapi_get_ptr_helper(implPtr))\
   }\
   friend class Class##Private;

#define ZAPI_DECLARE_PUBLIC(Class)\
   inline Class* getApiPtr()\
   {\
      return static_cast<Class *>(apiPtr);\
   }\
      inline const Class* getApiPtr() const\
   {\
      return static_const<const Class *>(apiPtr);\
   }\
   friend class Class;

#define ZAPI_D(Class) Class##Private * const implPtr = getImplPtr()
#define ZAPI_Q(Class) Class * const apiPtr = getApiPtr()

#ifdef ZAPI_COMPILER_STATIC_ASSERT
#  define ZAPI_STATIC_ASSERT(condition) static_assert(bool(condition), #condition)
#  define ZAPI_STATIC_ASSERT_X(condition, message) static_assert(bool(condition), message)
#else
// Intentionally undefined
template <bool Test> class ZStaticAssertFailure;
template <> class ZStaticAssertFailure<true> {};

#  define ZAPI_STATIC_ASSERT_PRIVATE_JOIN(A, B) ZAPI_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B)
#  define ZAPI_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B) A ## B
#  ifdef __COUNTER__
#     define ZAPI_STATIC_ASSERT(condition) \
   enum {ZAPI_STATIC_ASSERT_PRIVATE_JOIN(z_static_assert_result, __COUNTER__) = sizeof(ZStaticAssertFailure<!!(condition)>)};
#  else
#     define ZAPI_STATIC_ASSERT(condition) \
   enum {ZAPI_STATIC_ASSERT_PRIVATE_JOIN(z_static_assert_result, __LINE__) = sizeof(ZStaticAssertFailure<!!(condition)>)};
#  endif // __COUNTER__
#  define ZAPI_STATIC_ASSERT_X(condition, message) ZAPI_STATIC_ASSERT(condition)
#endif

// define some type alias, because we just API wrapper
#define zapi_long zend_long
#define zapi_ulong zend_ulong

namespace internal
{
namespace swapexceptiontester
{

using std::swap;
template <typename T>
void check_swap(T &t) ZAPI_DECL_NOEXCEPT_EXPR(noexcept(swap(t, t)));

} // swapexceptiontester
} // internal


template <typename T>
inline void swap(T &left, T &right)
ZAPI_DECL_NOEXCEPT_EXPR(noexcept(internal::swapexceptiontester::check_swap(left)))
{
   using std::swap;
   swap(left, right);
}

namespace zapi
{
using HANDLE = void *;
using Callback = std::function<void()>;
} // zapi

#ifdef ZAPI_CC_MSVC
#  define ZAPI_NEVER_INLINE __declspec(noinline)
#  define ZAPI_ALWAYS_INLINE __forceinline
#elif defined(ZAPI_CC_GNU)
#  define ZAPI_NEVER_INLINE __attribute__((noinline))
#  define ZAPI_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#  define ZAPI_NEVER_INLINE
#  define ZAPI_ALWAYS_INLINE inline
#endif

// Some classes do not permit copies to be made of an object. These
// classes contains a private copy constructor and assignment
// operator to disable copying (the compiler gives an error message).
#define ZAPI_DISALE_COPY(Class) \
   Class(const Class &) ZAPI_DECL_EQ_DELETE;\
   Class &operator=(const Class &) ZAPI_DECL_EQ_DELETE;

namespace zapi
{
inline void noop(void) {}
ZAPI_DECL_EXPORT void assert_x(const char *where, const char *what, const char *file, int line) ZAPI_DECL_NOEXCEPT;
} // zapi

#if !defined(ZAPI_ASSERT_X)
#  if defined(ZAPI_NO_DEBUG) && !defined(ZAPI_FORCE_ASSERTS)
#     define ZAPI_ASSERT_X(cond, where, what) do { } while ((false) && (cond))
#  else 
#     define ZAPI_ASSERT_X(cond, where, what) ((!(cond)) ? zapi::assert_x(where, what,__FILE__,__LINE__) : zapi::noop())
#  endif
#endif

#define ZAPI_ASSERT ZEND_ASSERT

#define ZAPI_SUCCESS SUCCESS
#define ZAPI_FAILURE FAILURE

// The way how PHP C API deals with "global" variables is peculiar.
//
// The following macros are supposed to turn into a structure that is going
// to be instantiated for each parallel running request, and for which the
// PHP engine allocates a certain amount of memory, and a magic pointer that
// is passed and should be forwarded to every thinkable PHP function.
//
// We don't use this architecture. We have our own environment object
// that makes much more sense, and that we use. However, the Zend engine
// expects this structure and this structure to exist.
ZEND_BEGIN_MODULE_GLOBALS(zapi)
ZEND_END_MODULE_GLOBALS(zapi)

// And now we're going to define a macro. This also is a uncommon architecture
// from PHP to get access to a variable from the structure above.
#ifdef ZTS
#  define ZAPI_GV(v) TSRM(zapi_globals_id, zapi_globals *, v)
#else
#  define ZAPI_GV(v) (zapi_globals.v)
#endif

// We're almost there, we now need to declare an instance of the
// structure defined above (if building for a single thread) or some
// sort of impossible to understand magic pointer-to-a-pointer (for
// multi-threading builds). We make this a static variable because
// this already is bad enough.
ZEND_EXTERN_MODULE_GLOBALS(zapi)

#endif //ZAPI_GLOBAL_H
