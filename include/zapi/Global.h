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

using z_int8 = signed char;
using z_uint8 = unsigned char;
using z_int16 = short;
using z_uint16 = unsigned short;
using z_uint32 = unsigned int;
using z_int32 = int;

#if defined(ZAPI_OS_WIN) && !defined(ZAPI_CC_GNU)
#  define Z_INT64_C(c) c ## i64
#  define Z_UINT64_C(c) c ## ui64
using z_int64 = __int64;
using z_uint64 = unsigned __int64;
#else
#  define Z_INT64_C(c) static_cast<long long>(c ## LL)
#  define Z_UINT64_C(c) static_cast<unsigned long long>(c ## ULL)
using z_int64 = long long;
using z_uint64 = unsigned long long;
#endif

using z_longlong = z_int64;
using z_ulonglong = z_uint64;

using z_uchar = unsigned char;
using z_ushort = unsigned short;
using z_uint = unsigned int;
using z_ulong = unsigned long;

#if defined(__ELF__)
#  define ZAPI_OF_ELF
#endif

#if defined(__MACH__) && defined(__APPLE__)
#  define ZAPI_OF_MACH_O
#endif

inline void zapi_noop(void) {}

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


template <int> struct ZIntegerForSize;
template <> struct ZIntegerForSize<1> { using Unsigned = z_uint8; using Signed = z_int8; };
template <> struct ZIntegerForSize<2> { using Unsigned = z_uint16; using Signed = z_int16; };
template <> struct ZIntegerForSize<4> { using Unsigned = z_uint32; using Singed = z_int32; };
template <> struct ZIntegerForSize<8> { using Unsigned = z_uint64; using Signed = z_int64; };
#if defined(ZAPI_CC_GNU) && defined(__SIZEOF_INT128__)
template <> struct ZIntegerForSize<16>
{
   __extension__ using Unsigned = unsigned __int128;
   __extension__ using Signed = __int128;
};
#endif

template <typename T> struct ZIntegerForSizeof : ZIntegerForSize<sizeof(T)>
{};

using z_registerint = ZIntegerForSize<ZAPI_PROCESSOR_WORDSIZE>::Signed;
using z_registeruint = ZIntegerForSize<ZAPI_PROCESSOR_WORDSIZE>::Unsigned;
using z_uintptr = ZIntegerForSizeof<void*>::Unsigned;
using z_ptrdiff = ZIntegerForSizeof<void*>::Signed;
using z_intptr = z_ptrdiff;

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

#define ZAPI_ASSERT(x) (x);


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
