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
// Created by softboy on 5/18/17.

#ifndef ZAPI_COMPILERDETECTION_H
#define ZAPI_COMPILERDETECTION_H

// The compiler, must be one of: (ZAPI_CC_x)

//   SYM      - Digital Mars C/C++ (used to be Symantec C++)
//   MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
//   BOR      - Borland/Turbo C++
//   WAT      - Watcom C++
//   GNU      - GNU C++
//   COMEAU   - Comeau C++
//   EDG      - Edison Design Group C++
//   OC       - CenterLine C++
//   SUN      - Forte Developer, or Sun Studio C++
//   MIPS     - MIPSpro C++
//   DEC      - DEC C++
//   HPACC    - HP aC++
//   USLC     - SCO OUDK and UDK
//   CDS      - Reliant C++
//   KAI      - KAI C++
//   INTEL    - Intel C++ for Linux, Intel C++ for Windows
//   HIGHC    - MetaWare High C/C++
//   PGI      - Portland Group C++
//   GHS      - Green Hills Optimizing C++ Compilers
//   RVCT     - ARM Realview Compiler Suite
//   CLANG    - C++ front-end for the LLVM compiler
//
//   Should be sorted most to least authoritative.

// Symantec C++ is now Digital Mars
#if defined(__DMC__) || defined(__SC__)
#  define ZAPI_CC_SYM
// "explicit" semantics implemented in 8.1e but keyword recognized since 7.5
#  if defined(__SC__) && __SC__ < 0x750
#     error "Compiler not supported"
#  endif
#elif defined(_MSC_VER)
#  ifdef __clang__
#     define ZAPI_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#  endif
#  define ZAPI_CC_MSVC (_MSC_VER)
#  define ZAPI_CC_MSVC_NET
#  define ZAPI_OUTOFLINE_TEMPLATE inline
#  if _MSC_VER < 1600
#     define ZAPI_NO_TEMPLATE_FRIENDS
#  endif
#  define ZAPI_COMPILER_MANGLES_RETURN_TYPE
#  define ZAPI_FUNC_INFO __FUNCSIG__
#  define ZAPI_ALIGNOF(type) __alignof(type)
#  define ZAPI_DECL_ALIGN(n) __declspec(align(n))
#  define ZAPI_ASSUME_IMPL(expr) __assume(expr)
#  define ZAPI_UNREACHABLE_IMPL() __assume(0)
#  define ZAPI_NORETURN __declspec(noreturn)
#  define ZAPI_DECL_DEPRECATED __declspec(deprecated)
#  ifndef ZAPI_CC_CLANG
#     define ZAPI_DECL_DEPRECATED_X(text) __declspec(deprecated(text))
#  endif
#  define ZAPI_DECL_EXPORT __declspec(dllexport)
#  define ZAPI_DECL_IMPORT __declspec(dllimport)
#  if _MSC_VER >= 1800
#     define ZAPI_MAKE_UNCHECKED_ARRAY_ITERATOR(x) stdext::make_unchecked_array_iterator(x)
#  endif
#  if _MSC_VER >= 1500
#     define ZAPI_MAKE_CHECKED_ARRAY_ITERATOR(x, N) stdext::make_checked_array_iterator(x, size_t(N))
#  endif
// Intel C++ disguising as Visual C++: the `using' keyword avoids warnings
#  if defined(__INTEL_COMPILER)
#     define ZAPI_DECL_VARIABLE_DEPRECATED
#     define ZAPI_CC_INTEL  __INTEL_COMPILER
#  endif

// only defined for MSVC since that's the only compiler that actually optimizes for this
// might get overridden further down when ZAPI_COMPILER_NOEXCEPT is detected
#  ifdef __cplusplus
#    define ZAPI_DECL_NOTHROW  throw()
#  endif

#elif defined(__BORLANDC__) || defined(__TURBOC__)
#  define ZAPI_CC_BOR
#  define ZAPI_INLINE_TEMPLATE
#  if __BORLANDC__ < 0x502
#    error "Compiler not supported"
#  endif

#elif defined(__WATCOMC__)
#  define ZAPI_CC_WAT

// ARM Realview Compiler Suite
// RVCT compiler also defines __EDG__ and __GNUC__ (if --gnu flag is given),
// so check for it before that
#elif defined(__ARMCC__) || defined(__CC_ARM)
#  define ZAPI_CC_RVCT
// work-around for missing compiler intrinsics
#  define __is_empty(X) false
#  define __is_pod(X) false
#  define ZAPI_DECL_DEPRECATED __attribute__ ((__deprecated__))
#  ifdef ZAPI_OS_LINUX
#     define ZAPI_DECL_EXPORT __attribute__((visibility("default")))
#     define ZAPI_DECL_IMPORT __attribute__((visibllity("default")))
#     define ZAPI_DECL_HIDDEN __attribute__((visibility("hidden")))
#  else
#     define ZAPI_DECL_EXPORT __declspec(dllexport)
#     define ZAPI_DECL_IMPORT __declspec(dllimport)
#  endif
#elif defined(__GNUC__)
#  define ZAPI_CC_GNU (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#    define ZAPI_CC_MINGW
#  endif
#  if defined(__INTEL_COMPILER)
// Intel C++ also masquerades as GCC
#     define ZAPI_CC_INTEL (__INTEL_COMPILER)
#     ifdef __clang__
// Intel C++ masquerades as Clang masquerading as GCC
#        define ZAPI_CC_CLANG 305
#     endif
#     define ZAPI_ASSUME_IMPL(expr) __assume(expr)
#     define ZAPI_UNREACHABLE_IMPL() __builtin_unreachable()
#     if __INTEL_COMPILER >= 1300 && !defined(__APPLE__)
#        define ZAPI_DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#     endif
#  elif defined(__clang__)
// Clang also masquerades as GCC
#     if defined(__apple_build_version__)
// http://en.wikipedia.org/wiki/Xcode#Toolchain_Versions
#        if __apple_build_version__ >= 7000053
#           define ZAPI_CC_CLANG 306
#        elif __apple_build_version__ >= 6000051
#           define ZAPI_CC_CLANG 305
#        elif __apple_build_version__ >= 5030038
#           define ZAPI_CC_CLANG 304
#        elif __apple_build_version__ >= 5000275
#           define ZAPI_CC_CLANG 303
#        elif __apple_build_version__ >= 4250024
#           define ZAPI_CC_CLANG 302
#        elif __apple_build_version__ >= 3180045
#           define ZAPI_CC_CLANG 301
#        elif __apple_build_version__ >= 2111001
#           define ZAPI_CC_CLANG 300
#        else
#           error "Unknow Apple Clang version"
#        endif
#     else
#        define ZAPI_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#     endif
#     if __has_builtin(__builtin_assume)
#        define ZAPI_ASSUME_IMPL(expr) __builtin_assume(expr)
#     else
#        define ZAPI_ASSUME_IMPL(expr) if (expr) {} else __builtin_unreachable()
#     endif
#     define ZAPI_UNREACHABLE_IMPL() __builtin_unreachable()
#     if !defined(__has_extension)
// Compatibility with older Clang versions
#        define __has_extension __has_feature
#     endif
#     if defined(__APPLE__)
// Apple/clang specific features
#        define ZAPI_DECL_CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#        ifdef __OBJC__
#           define ZAPI_DECL_NS_RETURNS_AUTORELEASED __attribute__((ns_returns_autoreleased))
#        endif
#     endif
#  else
// Plain GCC
#     if ZAPI_CC_GNU >= 405
#        define ZAPI_ASSUME_IMPL(expr) if (expr) {} __builtin_unreachable()
#        define ZAPI_UNREACHABLE_IMPL() __builtin_unreachable()
#        define ZAPI_DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#     endif
#  endif

#  ifdef ZAPI_OS_WIN
#     define ZAPI_DECL_EXPORT __declspec(dllexport)
#     define ZAPI_DECL_IMPORT __declspec(dllimport)
#  elif defined(ZAPI_VISIBILITY_AVAILABLE)
#     define ZAPI_DECL_EXPORT __attribute__((visibility("default")))
#     define ZAPI_DECL_IMPORT __attribute__((visibility("default")))
#     define ZAPI_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif

#  define ZAPI_FUNC_INFO __PRETTY_FUNCTION__
#  define ZAPI_ALIGNOF(type) __alignof__(type)
#  define ZAPI_TYPEOF(expr) __typeof__(expr)
#  define ZAPI_DECL_DEPRECATED __attribute__((__deprecated__))
#  define ZAPI_DECL_ALIGN(n) __attribute((__aligned__(n)))
#  define ZAPI_DECL_UNUSED __attribute__((__unused__))
#  define ZAPI_LIKELY(expr) __builtin_expect(!!(expr), true)
#  define ZAPI_UNLIKELY(expr) __builtin_expect(!!(expr), false)
#  define ZAPI_NORETURN __attribute__((__noreturn__))
#  define ZAPI_REQUIRE_RESULT __attribute__((__warn_unused_result__))
#  define ZAPI_DECL_PURE_FUNCTION __attribute__((pure))
#  define ZAPI_DECL_CONST_FUNCTION __attribute__((const))
#  define ZAPI_PACKED __attribute__ ((__packed__))
#  ifndef __ARM_EABI__
#     define ZAPI_NO_ARM_EABI
#  endif
#  if ZAPI_CC_GNU >= 403 && !defined(ZAPI_CC_CLANG)
#     define ZAPI_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
#  endif
// IBM compiler versions are a bit messy. There are actually two products:
// the C product, and the C++ product. The C++ compiler is always packaged
// with the latest version of the C compiler. Version numbers do not always
// match. This little table (I'm not sure it's accurate) should be helpful:
// C++ product                C product
// C Set 3.1                  C Compiler 3.0
// ...                        ...
// C++ Compiler 3.6.6         C Compiler 4.3
// ...                        ...
// Visual Age C++ 4.0         ...
// ...                        ...
// Visual Age C++ 5.0         C Compiler 5.0
// ...                        ...
// Visual Age C++ 6.0         C Compiler 6.0
// Now:
// __xlC__    is the version of the C compiler in hexadecimal notation
//
// is only an approximation of the C++ compiler version
// __IBMCPP__ is the version of the C++ compiler in decimal notation
//
// but it is not defined on older compilers like C Set 3.1
#elif defined(__xlC__)
#  define ZAPI_CC_XLC
#  define ZAPI_FULL_TEMPLATE_INSTANTIATION
#  if __xlC__ < 0x400
#     error "Compiler not supported"
#  elif __xlC__ >= 0x0600
#     define ZAPI_ALIGNOF(type) __alignof__(type)
#     define ZAPI_TYPEOF(expr) __typeof__(expr)
#     define ZAPI_DECL_ALIGN(n) __attribute__((__aligned__(n)))
#     define ZAPI_PACKED __attribute__((__packed__))
#  endif
// Older versions of DEC C++ do not define __EDG__ or __EDG - observed
// on DEC C++ V5.5-004. New versions do define  __EDG__ - observed on
// Compaq C++ V6.3-002.
// This compiler is different enough from other EDG compilers to handle
// it separately anyway.
#elif defined(__DECCXX) || defined(__DECC)
#  define ZAPI_CC_DEC
// Compaq C++ V6 compilers are EDG-based but I'm not sure about older
// DEC C++ V5 compilers.
#  if defined(__EDG__)
#     define ZAPI_CC_EDG
#  endif
// Compaq has disabled EDG's _BOOL macro and uses _BOOL_EXISTS instead
// - observed on Compaq C++ V6.3-002.
// In any case versions prior to Compaq C++ V6.0-005 do not have bool.
#  if !defined(_BOOL_EXISTS)
#     error "Compiler not supported"
#  endif
// Spurious (?) error messages observed on Compaq C++ V6.5-014.
// Apply to all versions prior to Compaq C++ V6.0-000 - observed on
// DEC C++ V5.5-004.
#  if __DECCXX_VER < 60060000
#     define ZAPI_BROKEN_TEMPLATE_SPECIALIZATION
#  endif
// avoid undefined symbol problems with out-of-line template members
#  define ZAPI_OUTOFLINE_TEMPLATE inline

// The Portland Group C++ compiler is based on EDG and does define __EDG__
// but the C compiler does not
#elif defined(__PGI)
#  define ZAPI_CC_PGI
#  if defined(__EDG__)
#     define ZAPI_CC_EDG
#  endif

// Compilers with EDG front end are similar. To detect them we test:
// __EDG documented by SGI, observed on MIPSpro 7.3.1.1 and KAI C++ 4.0b
// __EDG__ documented in EDG online docs, observed on Compaq C++ V6.3-002
// and PGI C++ 5.2-4
#elif !defined(ZAPI_OS_HPUX) && (defined(__EDG) || defined(__EDG__))
// From the EDG documentation (does not seem to apply to Compaq C++ or GHS C):
// _BOOL
//    Defined in C++ mode when bool is a keyword. The name of this
//    predefined macro is specified by a configuration flag. _BOOL
//    is the default.
// __BOOL_DEFINED
//    Defined in Microsoft C++ mode when bool is a keyword.
#  define ZAPI_CC_EDG
#  if !defined(_BOOL) && !defined(__BOOL_DEFINED) && !defined(__ghs)
#     error "Compiler not supported"
#  endif
// The Comeau compiler is based on EDG and does define __EDG__
#  if defined(__COMO__)
#     define ZAPI_CC_COMEAU
// The `using' keyword was introduced to avoid KAI C++ warnings
// but it's now causing KAI C++ errors instead. The standard is
// unclear about the use of this keyword, and in practice every
// compiler is using its own set of rules. Forget it.
#  elif defined(__KCC)
#     define ZAPI_CC_KAI
// Using the `using' keyword avoids Intel C++ for Linux warnings
#  elif defined(__INTEL_COMPILER)
#     define ZAPI_CC_INTEL (__INTEL_COMPILER)
// Uses CFront, make sure to read the manual how to tweak templates.
#  elif defined(__ghs)
#     define ZAPI_CC_GHS
#     define ZAPI_DECL_DEPRECATED __attribute__ ((__deprecated__))
#     define ZAPI_FUNC_INFO __PRETTY_FUNCTION__
#     define ZAPI_TYPEOF(expr) __typeof__(expr)
#     define ZAPI_ALIGNOF(type) __alignof__(type)
#     define ZAPI_UNREACHABLE_IMPL()
#     if defined(__cplusplus)
#        define ZAPI_COMPILER_AUTO_TYPE
#        define ZAPI_COMPILER_STATIC_ASSERT
#        define ZAPI_COMPILER_RANGE_FOR
#        if __GHS_VERSION_NUMBER >= 201505
#           define ZAPI_COMPILER_ALIGNAS
#           define ZAPI_COMPILER_ALIGNOF
#           define ZAPI_COMPILER_ATOMICS
#           define ZAPI_COMPILER_ATTRIBUTES
#           define ZAPI_COMPILER_AUTO_FUNCTION
#           define ZAPI_COMPILER_CLASS_ENUM
#           define ZAPI_COMPILER_CONSTEXPR
#           define ZAPI_COMPILER_DECLTYPE
#           define ZAPI_COMPILER_DEFAULT_MEMBERS
#           define ZAPI_COMPILER_DELETE_MEMBERS
#           define ZAPI_COMPILER_DELEGATING_CONSTRUCTORS
#           define ZAPI_COMPILER_EXPLICIT_CONVERSIONS
#           define ZAPI_COMPILER_EXPLICIT_OVERRIDES
#           define ZAPI_COMPILER_EXTERN_TEMPLATES
#           define ZAPI_COMPILER_INHERITING_CONSTRUCTORS
#           define ZAPI_COMPILER_INITIALIZER_LISTS
#           define ZAPI_COMPILER_LAMBDA
#           define ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
#           define ZAPI_COMPILER_NOEXCEPT
#           define ZAPI_COMPILER_NULLPTR
#           define ZAPI_COMPILER_RANGE_FOR
#           define ZAPI_COMPILER_RAW_STRINGS
#           define ZAPI_COMPILER_REF_QUALIFIERS
#           define ZAPI_COMPILER_RVALUE_REFS
#           define ZAPI_COMPILER_STATIC_ASSERT
#           define ZAPI_COMPILER_TEMPLATE_ALIAS
#           define ZAPI_COMPILER_THREAD_LOCAL
#           define ZAPI_COMPILER_THREADSAFE_STATICS
#           define ZAPI_COMPILER_UDL
#           define ZAPI_COMPILER_UNICODE_STRINGS
#           define ZAPI_COMPILER_UNIFORM_INIT
#           define ZAPI_COMPILER_UNRESTRICTED_UNIONS
#           define ZAPI_COMPILER_VARIADIC_MACROS
#           define ZAPI_COMPILER_VARIADIC_TEMPLATES
#        endif
#     endif
#  elif defined(__DCC__)
#     define ZAPI_CC_DIAB
#     if !defined(__bool)
#        error "Compiler not supported"
#     endif
// The UnixWare 7 UDK compiler is based on EDG and does define __EDG__
#  elif defined(__USLC__) && defined(__SCO_VERSION__)
#     define ZAPI_CC_USLC
// The latest UDK 7.1.1b does not need this, but previous versions do
#     if !defined(__SCO_VERSION__) || (__SCO_VERSION__ < 302200010)
#        define ZAPI_OUTOFLINE_TEMPLATE inline
#     endif
// Never tested!
#  elif defined(CENTERLINE_CLPP) || defined(OBJECTCENTER)
#     define ZAPI_CC_OC
// CDS++ defines __EDG__ although this is not documented in the Reliant
// documentation. It also follows conventions like _BOOL and this documented
#  elif defined(sinix)
#     define ZAPI_CC_CDS
// The MIPSpro compiler defines __EDG
#  elif defined(__sgi)
#     define ZAPI_CC_MIPS
#     define ZAPI_NO_TEMPLATE_FRIENDS
#     if defined(_COMPILER_VERSION) && (_COMPILER_VERSION > 740)
#        define ZAPI_OUTOFLINE_TEMPLATE inline
#        pragma set woff 3624,3625,3649 // turn off some harmless warnings
#     endif
#  endif
// VxWorks' DIAB toolchain has an additional EDG type C++ compiler
// (see __DCC__ above). This one is for C mode files (__EDG is not defined)
#elif defined(_DIAB_TOOL)
#  define ZAPI_CC_DIAB
#  define ZAPI_FUNC_INFO __PRETTY_FUNCTION__
//  Never tested!
#elif defined(__HIGHC__)
#  define ZAPI_CC_HIGHC
#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#  define ZAPI_CC_SUN
#  define ZAPI_COMPILER_MANGLES_RETURN_TYPE
// 5.0 compiler or better
//  'bool' is enabled by default but can be disabled using -features=nobool
//  in which case _BOOL is not defined
//      this is the default in 4.2 compatibility mode triggered by -compat=4
#  if __SUNPRO_CC >= 0x500
#     define ZAPI_NO_TEMPLATE_TEMPLATE_PARAMETERS
// see http://developers.sun.com/sunstudio/support/Ccompare.html
#     if __SUNPRO_CC >= 0x590
#        define ZAPI_ALIGNOF(type) __alignof__(type)
#        define ZAPI_TYPEOF(expr) __typeof__(expr)
#        define ZAPI_DECL_ALIGN(n) __attribute__((__aligned__(n)))
#     endif
#     if __SUNPRO_CC >= 0x550
#        define ZAPI_DECL_EXPORT __global
#     endif
#     if __SUNPRO_CC < 0x5a0
#        define ZAPI_NO_TEMPLATE_FRIENDS
#     endif
#     if !defined(_BOOL)
#        error "Compiler not supported"
#     endif
// 4.2 compiler or older
#  else
#     error "Compiler not supported"
#  endif
// CDS++ does not seem to define __EDG__ or __EDG according to Reliant
// documentation but nevertheless uses EDG conventions like _BOOL
#elif defined(sinix)
#  define ZAPI_CC_EDG
#  define ZAPI_CC_CDS
#  if !defined(_BOOL)
#     error "Compiler not supported"
#  endif
#  define ZAPI_BROKEN_TEMPLATE_SPECIALIZATION

#elif defined(ZAPI_OS_HPUX)
// __HP_aCC was not defined in first aCC releases
#  if defined(__HP_aCC) || __cplusplus >= 199707L
#     define ZAPI_NO_TEMPLATE_FRIENDS
#     define ZAPI_CC_HPACC
#     define ZAPI_FUNC_INFO __PRETTY_FUNCTION__
#     if __HP_aCC-0 < 060000
#        define ZAPI_NO_TEMPLATE_TEMPLATE_PARAMETERS
#        define ZAPI_DECL_EXPORT __declspec(dllexport)
#        define ZAPI_DECL_IMPORT __declspec(dllimport)
#     endif
#     if __HP_aCC-0 >= 061200
#        define ZAPI_DECL_ALIGN(n) __attribute__((aligned(n)))
#     endif
#     if __HP_aCC-0 >= 062000
#        define ZAPI_DECL_EXPORT __attribute__((visibility("default")))
#        define ZAPI_DECL_HIDDEN __attribute__((visibility("hidden")))
#        define ZAPI_DECL_IMPORT ZAPI_DECL_EXPORT
#     endif
#  else
#     error "Compiler not supported"
#  endif
#else
#  error "libnotify has not been tested with this compiler"
#endif

// C++11 support
//
// Paper              Macro                                     SD-6 macro
// N2341              ZAPI_COMPILER_ALIGNAS
// N2341              ZAPI_COMPILER_ALIGNOF
// N2427              ZAPI_COMPILER_ATOMICS
// N2761              ZAPI_COMPILER_ATTRIBUTES                __cpp_attributes = 200809
// N2541              ZAPI_COMPILER_AUTO_FUNCTION
// N1984 N2546        ZAPI_COMPILER_AUTO_TYPE
// N2437              ZAPI_COMPILER_CLASS_ENUM
// N2235              ZAPI_COMPILER_CONSTEXPR                 __cpp_constexpr = 200704
// N2343 N3276        ZAPI_COMPILER_DECLTYPE                  __cpp_decltype = 200707
// N2346              ZAPI_COMPILER_DEFAULT_MEMBERS
// N2346              ZAPI_COMPILER_DELETE_MEMBERS
// N1986              ZAPI_COMPILER_DELEGATING_CONSTRUCTORS
// N2437              ZAPI_COMPILER_EXPLICIT_CONVERSIONS
// N3206 N3272        ZAPI_COMPILER_EXPLICIT_OVERRIDES
// N1987              ZAPI_COMPILER_EXTERN_TEMPLATES
// N2540              ZAPI_COMPILER_INHERITING_CONSTRUCTORS
// N2672              ZAPI_COMPILER_INITIALIZER_LISTS
// N2658 N2927        ZAPI_COMPILER_LAMBDA                    __cpp_lambdas = 200907
// N2756              ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
// N2855 N3050        ZAPI_COMPILER_NOEXCEPT
// N2431              ZAPI_COMPILER_NULLPTR
// N2930              ZAPI_COMPILER_RANGE_FOR
// N2442              ZAPI_COMPILER_RAW_STRINGS               __cpp_raw_strings = 200710
// N2439              ZAPI_COMPILER_REF_QUALIFIERS
// N2118 N2844 N3053  ZAPI_COMPILER_RVALUE_REFS               __cpp_rvalue_references = 200610
// N1720              ZAPI_COMPILER_STATIC_ASSERT             __cpp_static_assert = 200410
// N2258              ZAPI_COMPILER_TEMPLATE_ALIAS
// N2659              ZAPI_COMPILER_THREAD_LOCAL
// N2660              ZAPI_COMPILER_THREADSAFE_STATICS
// N2765              ZAPI_COMPILER_UDL                       __cpp_user_defined_literals = 200809
// N2442              ZAPI_COMPILER_UNICODE_STRINGS           __cpp_unicode_literals = 200710
// N2640              ZAPI_COMPILER_UNIFORM_INIT
// N2544              ZAPI_COMPILER_UNRESTRICTED_UNIONS
// N1653              ZAPI_COMPILER_VARIADIC_MACROS
// N2242 N2555        ZAPI_COMPILER_VARIADIC_TEMPLATES        __cpp_variadic_templates = 200704
//
// For any future version of the C++ standard, we use only the SD-6 macro.
// For full listing, see
// http://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
//
// C++ extensions:
//    ZAPI_COMPILER_RESTRICTED_VLA       variable-length arrays, prior to __cpp_runtime_arrays

#ifdef __cplusplus
#  if __cplusplus < 201103L && !(defined(ZAPI_CC_MSVC) && ZAPI_CC_MSVC  >= 1800)
#     error "libnotify requires a C++11 compiler and yours does not seem to be that."
#  endif
#endif

#ifdef ZAPI_CC_INTEL
#  define ZAPI_COMPILER_RESTRICTED_VLA
#  define ZAPI_COMPILER_VARIADIC_MACROS // C++11 feature supported as an extension in other modes, too
#  define ZAPI_COMPILER_THREADSAFE_STATICS
#  if __INTEL_COMPILER < 1200
#     define ZAPI_NO_TEMPLATE_FRIENDS
#  endif
#  if __INTEL_COMPILER >= 1310 && !defined(_WIN32)
// ICC supports C++14 binary literals in C, C++98, and C++11 modes
// at least since 13.1, but I can't test further back
#     define ZAPI_COMPILER_BINARY_LITERALS
#  endif
#  if __cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__)
#     if __INTEL_COMPILER >= 1200
#        define ZAPI_COMPILER_AUTO_TYPE
#        define ZAPI_COMPILER_CLASS_ENUM
#        define ZAPI_COMPILER_DECLTYPE
#        define ZAPI_COMPILER_DEFAULT_MEMBERS
#        define ZAPI_COMPILER_DELETE_MEMBERS
#        define ZAPI_COMPILER_EXTERN_TEMPLATES
#        define ZAPI_COMPILER_LAMBDA
#        define ZAPI_COMPILER_RVALUE_REFS
#        define ZAPI_STATIC_ASSERT
#        define ZAPI_VARIADIC_MACROS
#     endif
#     if __INTEL_COMPILER >= 1210
#        define ZAPI_COMPILER_ATTRIBUTES
#        define ZAPI_COMPILER_AUTO_FUNCTION
#        define ZAPI_COMPILER_NULLPTR
#        define ZAPI_COMPILER_TEMPLATE_ALIAS
#        ifndef _CHAR16T // MSVC headers
#           define ZAPI_COMPILER_UNICODE_STRINGS
#        endif
#        define ZAPI_COMPILER_VARIADIC_TEMPLATES
#     endif
#     if __INTEL_COMPILER >= 1300
#        define ZAPI_COMPILER_ATOMICS
//       constexpr support is only partial
//#      define ZAPI_COMPILER_CONSTEXPR
#        define ZAPI_COMPILER_INITIALIZER_LISTS
#        define ZAPI_COMPILER_UNIFORM_INIT
#        define ZAPI_COMPILER_NOEXCEPT
#     endif
#     if __INTEL_COMPILER >= 1400
// Intel issue ID 6000056211, bug DPD200534796
//#        define ZAPI_COMPILER_CONSTEXPR
#        define ZAPI_COMPILER_DELEGATING_CONSTRUCTORS
#        define ZAPI_COMPILER_EXPLICIT_CONVERSIONS
#        define ZAPI_COMPILER_EXPLICIT_OVERRIDES
#        define ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
#        define ZAPI_COMPILER_RANGE_FOR
#        define ZAPI_COMPILER_RAW_STRINGS
#        define ZAPI_COMPILER_REF_QUALIFIERS
#        define ZAPI_COMPILER_UNICODE_STRINGS
#        define ZAPI_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if __INTEL_COMPILER >= 1500
#        if __INTEL_COMPILER * 100 + __INTEL_COMPILER_UPDATE >= 150001
//       the bug mentioned above is fixed in 15.0.1
#           define ZAPI_COMPILER_CONSTEXPR
#        endif
#        define ZAPI_COMPILER_ALIGNAS
#        define ZAPI_COMPILER_ALIGNOF
#        define ZAPI_COMPILER_INHERITING_CONSTRUCTORS
// C++11 thread_local is broken on OS X (Clang doesn't support it either)
#        ifndef ZAPI_OS_OSX
#           define ZAPI_COMPILER_THREAD_LOCAL
#        endif
#        define ZAPI_COMPILER_UDL
#     endif
#     ifdef _MSC_VER
#        if _MSC_VER == 1700
//       <initializer_list> is missing with MSVC 2012 (it's present in 2010, 2013 and up)
#           undef ZAPI_COMPILER_INITIALIZER_LISTS
#        endif
#        if _MSC_VER < 1900
//          ICC disables unicode string support when compatibility mode with MSVC 2013 or lower is active
#           undef ZAPI_COMPILER_UNICODE_STRINGS
//          Even though ICC knows about ref-qualified members, MSVC 2013 or lower doesn't, so
//          certain member functions may be missing from the DLLs.
#           undef ZAPI_COMPILER_REF_QUALIFIERS
//          Disable constexpr unless the MS headers have constexpr in all the right places too
//          (like std::numeric_limits<T>::max())
#           undef ZAPI_COMPILER_CONSTEXPR
#        endif
#     endif
#  endif
#endif // ZAPI_CC_INTEL

#if defined(ZAPI_CC_CLANG) && !defined(ZAPI_CC_INTEL)
// General C++ features
#  define ZAPI_COMPILER_RESTRICTED_VLA
#  define ZAPI_COMPILER_THREADSAFE_STATICS
#  if __has_feature(attribute_deprecated_with_message)
#     define ZAPI_DECL_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#  endif

// Clang supports binary literals in C, C++98 and C++11 modes
// It's been supported "since the dawn of time itself" (cf. commit 179883)
#  if __has_extension(cxx_binary_literals)
#     define ZAPI_COMPILER_BINARY_LITERALS
#  endif

// Variadic macros are supported for gnu++98, c++11, c99 ... since 2.9
#  if ZAPI_CC_CLANG >= 209
#     if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
   || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
   || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
#        define ZAPI_COMPILER_VARIADIC_MACROS
#     endif
#  endif

// C++11 features, see http://clang.llvm.org/cxx_status.html
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
// Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11
#     if __has_feature(cxx_alignas)
#        define ZAPI_COMPILER_ALIGNAS
#        define ZAPI_COMPILER_ALIGNOF
#     endif
#     if __has_feature(cxx_atomic) && __has_include(<atomic>)
#        define ZAPI_COMPILER_ATOMICS
#     endif
#     if __has_feature(cxx_attributes)
#        define ZAPI_COMPILER_ATTRIBUTES
#     endif
#     if __has_feature(cxx_auto_type)
#        define ZAPI_COMPILER_AUTO_FUNCTION
#        define ZAPI_COMPILER_AUTO_TYPE
#     endif
#     if __has_feature(cxx_strong_enums)
#        define ZAPI_COMPILER_CLASS_ENUM
#     endif
#     if __has_feature(cxx_constexpr) && ZAPI_CC_CLANG > 302 // CLANG 3.2 has bad/partial support
#        define ZAPI_COMPILER_CONSTEXPR
#     endif
#     if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#        define ZAPI_COMPILER_DECLTYPE
#     endif
#     if __has_feature(cxx_defaulted_functions)
#        define ZAPI_COMPILER_DEFAULT_MEMBERS
#     endif
#     if __has_feature(cxx_deleted_functions)
#        define ZAPI_COMPILER_DELETE_MEMBERS
#     endif
#     if __has_feature(cxx_delegating_constructors)
#        define ZAPI_COMPILER_DELEGATING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_explicit_conversions)
#        define ZAPI_COMPILER_EXPLICIT_CONVERSIONS
#     endif
#     if __has_feature(cxx_override_control)
#        define ZAPI_COMPILER_EXPLICIT_OVERRIDES
#     endif
#     if __has_feature(cxx_inheriting_constructors)
#        define ZAPI_COMPILER_INHERITING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_generalized_initializers)
#        define ZAPI_COMPILER_INITIALIZER_LISTS
#        define ZAPI_COMPILER_UNIFORM_INIT /* both covered by this feature macro, according to docs */
#     endif
#     if __has_feature(cxx_lambdas)
#        define ZAPI_COMPILER_LAMBDA
#     endif
#     if __has_feature(cxx_noexcept)
#        define ZAPI_COMPILER_NOEXCEPT
#     endif
#     if __has_feature(cxx_nonstatic_member_init)
#        define ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
#     endif
#     if __has_feature(cxx_nullptr)
#        define ZAPI_COMPILER_NULLPTR
#     endif
#     if __has_feature(cxx_range_for)
#        define ZAPI_COMPILER_RANGE_FOR
#     endif
#     if __has_feature(cxx_raw_string_literals)
#        define ZAPI_COMPILER_RAW_STRINGS
#     endif
#     if __has_feature(cxx_reference_qualified_functions)
#        define ZAPI_COMPILER_REF_QUALIFIERS
#     endif
#     if __has_feature(cxx_rvalue_references)
#        define ZAPI_COMPILER_RVALUE_REFS
#     endif
#     if __has_feature(cxx_static_assert)
#        define ZAPI_COMPILER_STATIC_ASSERT
#     endif
#     if __has_feature(cxx_alias_templates)
#        define ZAPI_COMPILER_TEMPLATE_ALIAS
#     endif
#     if __has_feature(cxx_thread_local)
#        if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#           define ZAPI_COMPILER_THREAD_LOCAL
#        endif
#     endif
#     if __has_feature(cxx_user_literals)
#        define ZAPI_COMPILER_UDL
#     endif
#     if __has_feature(cxx_unicode_literals)
#        define ZAPI_COMPILER_UNICODE_STRINGS
#     endif
#     if __has_feature(cxx_unrestricted_unions)
#        define ZAPI_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if __has_feature(cxx_variadic_templates)
#        define ZAPI_COMPILER_VARIADIC_TEMPLATES
#     endif
// Features that have no __has_feature() check
#     if ZAPI_CC_CLANG >= 209 // since clang 2.9
#        define ZAPI_COMPILER_EXTERN_TEMPLATES
#     endif
#  endif

// C++1y features, deprecated macros. Do not update this list.
#  if __cplusplus > 201103L
#     if __has_feature(cxx_generic_lambda)
#        define ZAPI_COMPILER_GENERIC_LAMBDA
#     endif
#     if __has_feature(cxx_init_capture)
#        define ZAPI_COMPILER_LAMBDA_CAPTURES
#     endif
#     if __has_feature(cxx_relaxed_constexpr)
#        define ZAPI_COMPILER_RELAXED_CONSTEXPR_FUNCTIONS
#     endif
#     if __has_feature(cxx_decltype_auto) && __has_feature(cxx_return_type_deduction)
#        define ZAPI_COMPILER_RETURN_TYPE_DEDUCTION
#     endif
#     if __has_feature(cxx_variable_templates)
#        define ZAPI_COMPILER_VARIABLE_TEMPLATES
#     endif
#     if __has_feature(cxx_runtime_array)
#        define ZAPI_COMPILER_VLA
#     endif
#  endif

#  if defined(__has_warning)
#     if __has_warning("-Wunused-private-field")
#        define ZAPI_DECL_UNUSED_MEMBER ZAPI_DECL_UNUSED
#     endif
#  endif
#endif // ZAPI_CC_CLANG

#if defined(ZAPI_CC_GNU) && !defined(ZAPI_CC_INTEL) && !defined(ZAPI_CC_CLANG)
#  define ZAPI_COMPILER_RESTRICTED_VLA
#  define ZAPI_COMPILER_THREADSAFE_STATICS
#  if ZAPI_CC_GNU >= 403
// GCC supports binary literals in C, C++98 and C++11 modes
#     define ZAPI_COMPILER_BINARY_LITERALS
#  endif
#  if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
   || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
   || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
// Variadic macros are supported for gnu++98, c++11, C99 ... since forever (gcc 2.97)
#     define ZAPI_COMPILER_VARIADIC_MACROS
#  endif
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#     if ZAPI_CC_GNU >= 403
// C++11 features supported in GCC 4.3:
#        define ZAPI_COMPILER_DECLTYPE
#        define ZAPI_COMPILER_RVALUE_REFS
#        define ZAPI_COMPILER_STATIC_ASSERT
#     endif
#     if ZAPI_CC_GNU >= 404
// C++11 features supported in GCC 4.4:
#        define ZAPI_COMPILER_AUTO_FUNCTION
#        define ZAPI_COMPILER_AUTO_TYPE
#        define ZAPI_COMPILER_EXTERN_TEMPLATES
#        define ZAPI_COMPILER_UNIFORM_INIT
#        define ZAPI_COMPILER_UNICODE_STRINGS
#        define ZAPI_COMPILER_VARIADIC_TEMPLATES
#     endif
// C++11 features supported in GCC 4.5:
#     if ZAPI_CC_GNU >= 405
#        define ZAPI_COMPILER_EXPLICIT_CONVERSIONS
// GCC 4.4 implements initializer_list but does not define typedefs required
// by the standard.
#        define ZAPI_COMPILER_INITIALIZER_LISTS
#        define ZAPI_COMPILER_LAMBDA
#        define ZAPI_COMPILER_RAW_STRINGS
#        define ZAPI_COMPILER_CLASS_ENUM
#     endif
#     if ZAPI_CC_GNU >= 406
// Pre-4.6 compilers implement a non-final snapshot of N2346, hence default and delete
// functions are supported only if they are public. Starting from 4.6, GCC handles
// final version - the access modifier is not relevant.
#        define ZAPI_COMPILER_DEFAULT_MEMBERS
#        define ZAPI_COMPILER_DELETE_MEMBERS
// C++11 features supported in GCC 4.6:
#        define ZAPI_COMPILER_CONSTEXPR
#        define ZAPI_COMPILER_NULLPTR
#        define ZAPI_COMPILER_UNRESTRICTED_UNIONS
#        define ZAPI_COMPILER_RANGE_FOR
#     endif
#     if ZAPI_CC_GNU >= 407
// GCC 4.4 implemented <atomic> and std::atomic using its old intrinsics.
// However, the implementation is incomplete for most platforms until GCC 4.7:
// instead, std::atomic would use an external lock. Since we need an std::atomic
// we only enable it here
#        define ZAPI_COMPILER_ATOMICS
// GCC 4.6.x has problems dealing with noexcept expressions,
// so turn the feature on for 4.7 and above, only
#        define ZAPI_COMPILER_NOEXCEPT
// C++11 features supported in GCC 4.7:
#        define ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
#        define ZAPI_COMPILER_DELEGATING_CONSTRUCTORS
#        define ZAPI_COMPILER_EXPLICIT_OVERRIDES
#        define ZAPI_COMPILER_TEMPLATE_ALIAS
#        define ZAPI_COMPILER_UDL
#     endif
#     if ZAPI_CC_GNU >= 408
#        define ZAPI_COMPILER_ATTRIBUTES
#        define ZAPI_COMPILER_ALIGNAS
#        define ZAPI_COMPILER_ALIGNOF
#        define ZAPI_COMPILER_INHERITING_CONSTRUCTORS
#        define ZAPI_COMPILER_THREAD_LOCAL
#        if ZAPI_CC_GNU > 408 || __GNUC_PATCHLEVEL__ >= 1
#           define ZAPI_COMPILER_REF_QUALIFIERS
#        endif
#     endif
// C++11 features are complete as of GCC 4.8.1
#  if __cplusplus > 201103L
#     if ZAPI_CC_GNU >= 409
// C++1y features in GCC 4.9 - deprecated, do not update this list */
#        define ZAPI_COMPILER_LAMBDA_CAPTURES
#        define ZAPI_COMPILER_RETURN_TYPE_DEDUCTION
#     endif
#  endif
#  endif
#endif

#if defined(ZAPI_CC_MSVC) && !defined(ZAPI_CC_INTEL)
#  if defined(__cplusplus)
#     if _MSC_VER >= 1400
// C++11 features supported in VC8 = VC2005:
#        define ZAPI_COMPILER_VARIADIC_MACROS
#        ifndef __cplusplus_cli
// 2005 supports the override and final contextual keywords, in
// the same positions as the C++11 variants, but 'final' is
// called 'sealed' instead:
// http://msdn.microsoft.com/en-us/library/0w2w91tf%28v=vs.80%29.aspx
// The behavior is slightly different in C++/CLI, which requires the
// "virtual" keyword to be present too, so don't define for that.
// So don't define ZAPI_COMPILER_EXPLICIT_OVERRIDES (since it's not
// the same as the C++11 version), but define the Q_DECL_* flags
// accordingly:
#           define ZAPI_DECL_OVERRIDE override
#           define ZAPI_DECL_FINAL sealed
#        endif
#     endif
#     if _MSC_VER >= 1600
// C++11 features supported in VC10 = VC2010:
#        define ZAPI_COMPILER_AUTO_FUNCTION
#        define ZAPI_COMPILER_AUTO_TYPE
#        define ZAPI_COMPILER_DECLTYPE
#        define ZAPI_COMPILER_EXTERN_TEMPLATES
#        define ZAPI_COMPILER_LAMBDA
#        define ZAPI_COMPILER_NULLPTR
#        define ZAPI_COMPILER_RVALUE_REFS
#        define ZAPI_COMPILER_STATIC_ASSERT
// MSVC's library has std::initializer_list, but the compiler does not support the braces initialization
//#      define ZAPI_COMPILER_INITIALIZER_LISTS
//#      define ZAPI_COMPILER_UNIFORM_INIT
#     endif
#     if _MSC_VER >= 1700
// C++11 features supported in VC11 = VC2012:
#        undef ZAPI_DECL_OVERRIDE /* undo 2005/2008 settings... */
#        undef ZAPI_DECL_FINAL    /* undo 2005/2008 settings... */
#        define ZAPI_COMPILER_EXPLICIT_OVERRIDES /* ...and use std C++11 now   */
#        define ZAPI_COMPILER_CLASS_ENUM
#        define ZAPI_COMPILER_ATOMICS
#     endif /* VC 11 */
#     if _MSC_VER >= 1800
//  C++11 features in VC12 = VC2013
// Implemented, but can't be used on move special members
//#      define ZAPI_COMPILER_DEFAULT_MEMBERS
#        define ZAPI_COMPILER_DELETE_MEMBERS
#        define ZAPI_COMPILER_DELEGATINTG_CONSTRUCTORS
#        define ZAPI_COMPILER_EXPLICIT_CONVERSIONS
#        define ZAPI_COMPILER_NONSTATIC_MEMBER_INIT
// implemented, but nested initialization fails (eg tst_qvector): http://connect.microsoft.com/VisualStudio/feedback/details/800364/initializer-list-calls-object-destructor-twice
//      #define ZAPI_COMPILER_INITIALIZER_LISTS
// implemented in principle, but has a bug that makes it unusable: http://connect.microsoft.com/VisualStudio/feedback/details/802058/c-11-unified-initialization-fails-with-c-style-arrays
//      #define ZAPI_COMPILER_UNIFORM_INIT
#        define ZAPI_COMPILER_RAW_STRINGS
#        define ZAPI_COMPILER_TEMPLATE_ALIAS
#        define ZAPI_COMPILER_VARIADIC_TEMPLATES
#     endif /* VC 12*/
#     if _MSC_FULL_VER >= 180030324
#        define ZAPI_COMPILER_INITIALIZER_LISTS
#     endif
#     if _MSC_VER >= 1900
// C++11 features in VC14 = VC2015
#        define ZAPI_COMPILER_DEFAULT_MEMBERS
#        define ZAPI_COMPILER_ALIGNAS
#        define ZAPI_COMPILER_ALIGNOF
// Partial support
//#      define ZAPI_COMPILER_CONSTEXPR
#        define ZAPI_COMPILER_INHERITING_CONSTRUCTORS
#        define ZAPI_COMPILER_NOEXCEPT
#        define ZAPI_COMPILER_RANGE_FOR
#        define ZAPI_COMPILER_REF_QUALIFIERS
#        define ZAPI_COMPILER_THREAD_LOCAL
// Broken, see QTBUG-47224 and https://connect.microsoft.com/VisualStudio/feedback/details/1549785
//#      define ZAPI_COMPILER_THREADSAFE_STATICS
#        define ZAPI_COMPILER_UDL
#        define ZAPI_COMPILER_UNICODE_STRINGS
// Uniform initialization is not working yet
//#      define ZAPI_COMPILER_UNIFORM_INIT
#        define ZAPI_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if _MSC_FULL_VER >= 190023419
#        define ZAPI_COMPILER_ATTRIBUTES
// Almost working, see https://connect.microsoft.com/VisualStudio/feedback/details/2011648
//#      define ZAPI_COMPILER_CONSTEXPR
#        define ZAPI_COMPILER_THREADSAFE_STATICS
#        define ZAPI_COMPILER_UNIFORM_INIT
#     endif
#     if _MSC_VER >= 1910
#        define ZAPI_COMPILER_CONSTEXPR
#     endif
#  endif // __cplusplus
#endif // ZAPI_CC_MSVC

#ifdef __cplusplus
#  include <utility>
#  if defined(ZAPI_OS_QNX)
// By default, QNX 7.0 uses libc++ (from LLVM) and
// QNX 6.X uses Dinkumware's libcpp. In all versions,
// it is also possible to use GNU libstdc++.

// For Dinkumware, some features must be disabled
// (mostly because of library problems).
// Dinkumware is assumed when __GLIBCXX__ (GNU libstdc++)
// and _LIBCPP_VERSION (LLVM libc++) are both absent.
#     if !defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION)
// Older versions of libcpp (QNX 650) do not support C++11 features
// _HAS_* macros are set to 1 by toolchains that actually include
// Dinkum C++11 libcpp.
#        if !defined(_HAS_CPP0X) || !_HAS_CPP0X
// Disable C++11 features that depend on library support
#           undef ZAPI_COMPILER_INITIALIZER_LISTS
#           undef ZAPI_COMPILER_RVALUE_REFS
#           undef ZAPI_COMPILER_REF_QUALIFIERS
#           undef ZAPI_COMPILER_UNICODE_STRINGS
#           undef ZAPI_COMPILER_NOEXCEPT
#        endif // !_HAS_CPP0X
#        if !defined(_HAS_NULLPTR_T) || !_HAS_NULLPTR_T
#           undef ZAPI_COMPILER_NULLPTR
#        endif // !_HAS_NULLPTR_T
#        if !defined(_HAS_CONSTEXPR) || !_HAS_CONSTEXPR
// The libcpp is missing constexpr keywords on important functions like std::numeric_limits<>::min()
// Disable constexpr support on QNX even if the compiler supports it
#           undef ZAPI_COMPILER_CONSTEXPR
#        endif // !_HAS_CONSTEXPR
#     endif // !__GLIBCXX__ && !_LIBCPP_VERSION
#  endif // ZAPI_OS_QNX
#  if (defined(ZAPI_CC_CLANG) || defined(ZAPI_CC_INTEL)) && defined(ZAPI_OS_MAC) && defined(__GNUC_LIBSTD__) \
   && ((__GNUC_LIBSTD__-0) * 100 + __GNUC_LIBSTD_MINOR__-0 <= 402)
// Apple has not updated libstdc++ since 2007, which means it does not have
// <initializer_list> or std::move. Let's disable these features
#     undef ZAPI_COMPILER_INITIALIZER_LISTS
#     undef ZAPI_COMPILER_RVALUE_REFS
#     undef ZAPI_COMPILER_REF_QUALIFIERS
// Also disable <atomic>, since it's clearly not there
#     undef ZAPI_COMPILER_ATOMICS
#  endif
#  if defined(ZAPI_CC_CLANG) && defined(ZAPI_CC_INTEL) && ZAPI_CC_INTEL >= 1500
// ICC 15.x and 16.0 have their own implementation of std::atomic, which is activated when in Clang mode
// (probably because libc++'s <atomic> on OS X failed to compile), but they're missing some
// critical definitions. (Reported as Intel Issue ID 6000117277)
#     define __USE_CONSTEXPR 1
#     define __USE_NOEXCEPT 1
#  endif
#  if defined(ZAPI_CC_MSVC) && defined(ZAPI_CC_CLANG)
// Clang and the Intel compiler support more C++ features than the Microsoft compiler
// so make sure we don't enable them if the MS headers aren't properly adapted.
#     ifndef _HAS_CONSTEXPR
#        undef ZAPI_COMPILER_CONSTEXPR
#     endif
#     ifndef _HAS_DECLTYPE
#        undef ZAPI_COMPILER_DECLTYPE
#     endif
#     ifndef _HAS_INITIALIZER_LISTS
#        undef ZAPI_COMPILER_INITIALIZER_LISTS
#     endif
#     ifndef _HAS_NULLPTR_T
#        undef ZAPI_COMPILER_NULLPTR
#     endif
#     ifndef _HAS_RVALUE_REFERENCES
#        undef ZAPI_COMPILER_RVALUES_REFS
#     endif
#     ifndef _HAS_SCOPE_ENUM
#        undef ZAPI_COMPILER_CLASS_ENUM
#     endif
#     ifndef _HAS_TEMPLATE_ALIAS
#        undef ZAPI_COMPILER_TEMPLATE_ALIAS
#     endif
#     ifndef _HAS_VARIADIC_TEMPLATES
#        undef ZAPI_COMPILER_VARIADIC_TEMPLATES
#     endif
#  endif
#  if defined(ZAPI_COMPILER_THREADSAFE_STATICS) && defined(ZAPI_OS_MAC)
// Apple's low-level implementation of the C++ support library
// (libc++abi.dylib, shared between libstdc++ and libc++) has deadlocks. The
// C++11 standard requires the deadlocks to be removed, so this will eventually
// be fixed; for now, let's disable this.
#     undef ZAPI_COMPILER_THREADSAFE_STATICS
#  endif
#endif // __cplusplus

// C++11 keywords and expressions
#ifdef ZAPI_COMPILER_NULLPTR
#  define ZAPI_NULLPTR nullptr
#else
#  define ZAPI_NULLPTR NULL
#endif

#ifdef ZAPI_COMPILER_DEFAULT_MEMBERS
#  define ZAPI_DECL_EQ_DEFAULT = default
#else
#  define ZAPI_DECL_EQ_DEFAULT
#endif

#ifdef ZAPI_COMPILER_DELETE_MEMBERS
#  define ZAPI_DECL_EQ_DELETE = delete
#else
#  define ZAPI_DECL_EQ_DELETE
#endif

// Don't break code that is already using ZAPI_COMPILER_DEFAULT_DELETE_MEMBERS
#if defined(ZAPI_COMPILER_DEFAULT_MEMBERS) && defined(ZAPI_COMPILER_DELETE_MEMBERS)
#  define ZAPI_COMPILER_DEFAULT_DELETE_MEMBERS
#endif

#ifdef ZAPI_COMPILER_CONSTEXPR

#  if defined(__cpp_constexpr) && __cpp_constexpr-0 >= 201304
#     define ZAPI_DECL_CONSTEXPR constexpr
#     define ZAPI_DECL_RELAXED_CONSTEXPR constexpr
#     define ZAPI_CONSTEXPR constexpr
#     define ZAPI_RELAXED_CONSTEXPR constexpr
#  else
#     define ZAPI_DECL_CONSTEXPR constexpr
#     define ZAPI_DECL_RELAXED_CONSTEXPR
#     define ZAPI_CONSTEXPR constexpr
#     define ZAPI_RELAXED_CONSTEXPR const
#  endif
#else
#  define ZAPI_DECL_CONSTEXPR
#  define ZAPI_DECL_RELAXED_CONSTEXPR
#  define ZAPI_CONSTEXPR const
#  define ZAPI_RELAXED_CONSTEXPR const
#endif

#ifdef ZAPI_COMPILER_EXPLICIT_OVERRIDES
#  define ZAPI_DECL_OVERRIDE override
#  define ZAPI_DECL_FINAL final
#else
#  ifndef ZAPI_DECL_OVERRIDE
#     define ZAPI_DECL_OVERRIDE
#  endif
#  ifndef ZAPI_DECL_FINAL
#     define ZAPI_DECL_FINAL
#  endif
#endif

#ifdef ZAPI_COMPILER_NOEXCEPT
#  define ZAPI_DECL_NOEXCEPT noexcept
#  define ZAPI_DECL_NOEXCEPT_EXPR(x) noexcept(x)
#  ifdef ZAPI_DECL_NOTHROW
#     undef ZAPI_DECL_NOTHROW // override with C++11 noexcept if available
#  endif
#else
#  define ZAPI_DECL_NOEXCEPT
#  define ZAPI_DECL_NOEXCEPT_EXPR(x)
#endif

#if defined(ZAPI_COMPILER_ALIGNOF)
#  undef ZAPI_ALIGNOF
#  define ZAPI_ALIGNOF(x) alignof(x)
#endif

#if defined(ZAPI_COMPILER_ALIGNAS)
#  undef ZAPI_DECL_ALIGN
#  define ZAPI_DECL_ALIGN(n) alignas(n)
#endif

#ifndef ZAPI_NORETURN
#  define ZAPI_NORETURN
#endif

#ifndef ZAPI_LIKELY
#  define ZAPI_LIKELY(x) (x)
#endif

#ifndef ZAPI_UNLIKELY
#  define ZAPI_UNLIKELY(x) (x)
#endif

#ifndef ZAPI_ASSUME_IMPL
#  define ZAPI_ASSUME_IMPL(expr) ZAPI_noop()
#endif

#ifndef ZAPI_UNREACHABLE_IMPL
#  define ZAPI_UNREACHABLE_IMPL() ZAPI_noop()
#endif

#ifndef ZAPI_ALLOC_SIZE
#  define ZAPI_ALLOC_SIZE(x)
#endif

#ifndef ZAPI_REQUIRED_RESULT
#  define ZAPI_REQUIRED_RESULT
#endif

#ifndef ZAPI_DECL_DEPRECATED
#  define ZAPI_DECL_DEPRECATED
#endif

#ifndef ZAPI_DECL_VARIABLE_DEPRECATED
#  define ZAPI_DECL_VARIABLE_DEPRECATED ZAPI_DECL_DEPRECATED
#endif

#ifndef ZAPI_DECL_DEPRECATED_X
#  define ZAPI_DECL_DEPRECATED_X(text) ZAPI_DECL_DEPRECATED
#endif

#ifndef ZAPI_DECL_EXPORT
#  define ZAPI_DECL_EXPORT
#endif

#ifndef ZAPI_DECL_IMPORT
#  define ZAPI_DECL_IMPORT
#endif

#ifndef ZAPI_DECL_HIDDEN
#  define ZAPI_DECL_HIDDEN
#endif

#ifndef ZAPI_DECL_UNUSED
#  define ZAPI_DECL_UNUSED
#endif

#ifndef ZAPI_DECL_UNUSED_MEMBER
#  define ZAPI_DECL_UNUSED_MEMBER
#endif

#ifndef ZAPI_FUNC_INFO
#  if defined(ZAPI_OS_SOLARIS) || defined(ZAPI_CC_XLC)
#     define ZAPI_FUNC_INFO __FILE__ "(line number unavailable)"
#  else
#     define ZAPI_FUNC_INFO __FILE__ ":" ZAPI_STRINGIFY(__LINE__)
#  endif
#endif

#ifndef ZAPI_DECL_CF_RETURNS_RETAINED
#  define ZAPI_DECL_CF_RETURNS_RETAINED
#endif

#ifndef ZAPI_DECL_NS_RETURNS_AUTORELEASED
#  define ZAPI_DECL_NS_RETURNS_AUTORELEASED
#endif

#ifndef ZAPI_DECL_PURE_FUNCTION
#  define ZAPI_DECL_PURE_FUNCTION
#endif

#ifndef ZAPI_DECL_CONST_FUNCTION
#  define ZAPI_DECL_CONST_FUNCTION
#endif

#ifndef ZAPI_MAKE_UNCHECKED_ARRAY_ITERATOR
#  define ZAPI_MAKE_UNCHECKED_ARRAY_ITERATOR(x) (x)
#endif

#ifndef ZAPI_MAKE_CHECKED_ARRAY_ITERATOR
#  define ZAPI_MAKE_CHECKED_ARRAY_ITERATOR(x, N) (x)
#endif

// SG10's SD-6 feature detection and some useful extensions from Clang and GCC
// https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
// http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
#ifdef __has_builtin
#  define ZAPI_HAS_BUILTIN(x) __has_builtin(x)
#else
#  define ZAPI_HAS_BUILTIN(x) 0
#endif

#ifdef __has_attribute
#  define ZAPI_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#  define ZAPI_HAS_ATTRIBUTE(x) 0
#endif

#ifdef __has_cpp_attribute
#  define ZAPI_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define ZAPI_HAS_CPP_ATTRIBUTE(x) 0
#endif

#ifdef __has_include
#  define ZAPI_HAS_INCLUDE(x) __has_include(x)
#else
#  define ZAPI_HAS_INCLUDE(x) 0
#endif

#ifdef __has_include_next
#  define ZAPI_HAS_INCLUDE_NEXT(x) __has_include_next(x)
#else
#  define ZAPI_HAS_INCLUDE_NEXT(x) 0
#endif

// Warning/diagnostic handling
#define ZAPI_DO_PRAGMA(text) _Pragma(#text)
#if defined(ZAPI_CC_INTEL) && defined(ZAPI_CC_MSVC)
// icl.exe: Intel compiler on Windows
#  undef ZAPI_DO_PRAGMA
#  define ZAPI_WARNING_PUSH                  __pragma(warning(push))
#  define ZAPI_WARNING_POP                   __pragma(warning(pop))
#  define ZAPI_WARNING_DISABLE_MSVC(number)
#  define ZAPI_WARNING_DISABLE_INTEL(number) __pragma(warning(disable: number))
#  define ZAPI_WARNING_DISABLE_CLANG(text)
#  define ZAPI_WARNING_DISABLE_GCC(text)
#  define ZAPI_WARNING_DISABLE_DEPRECATED    ZAPI_WARNING_DISABLE_INTEL(1478 1786)
#elif ZAPI_CC_INTEL
// icc: Intel compiler on Linux or OS X
#  define ZAPI_WARNING_PUSH                  ZAPI_DO_PRAGMA(warning(push))
#  define ZAPI_WARNING_POP                   ZAPI_DO_PRAGMA(warning(pop))
#  define ZAPI_WARNING_DISABLE_INTEL(number) ZAPI_DO_PRAGMA(warning(disable: number))
#  define ZAPI_WARNING_DISABLE_MSVC(number)
#  define ZAPI_WARNING_DISABLE_CLANG(text)
#  define ZAPI_WARNING_DISABLE_GCC(text)
#  define ZAPI_WARNING_DISABLE_DEPRECATED    ZAPI_WARNING_DISABLE_INTEL(1478 1786)
#elif defined(ZAPI_CC_MSVC) && _MSC_VER >= 1500 && !defined(ZAPI_CC_CLANG)
#  undef ZAPI_DO_PRAGMA                           /* not needed */
#  define ZAPI_WARNING_PUSH                  __pragma(warning(push))
#  define ZAPI_WARNING_POP                   __pragma(warning(pop))
#  define ZAPI_WARNING_DISABLE_MSVC(number)  __pragma(warning(disable: number))
#  define ZAPI_WARNING_DISABLE_INTEL(number)
#  define ZAPI_WARNING_DISABLE_CLANG(text)
#  define ZAPI_WARNING_DISABLE_GCC(text)
#  define ZAPI_WARNING_DISABLE_DEPRECATED    ZAPI_WARNING_DISABLE_MSVC(4996)
#elif defined(ZAPI_CC_CLANG)
#  define ZAPI_WARNING_PUSH                  ZAPI_DO_PRAGMA(clang diagnostic push)
#  define ZAPI_WARNING_POP                   ZAPI_DO_PRAGMA(clang diagnostic pop)
#  define ZAPI_WARNING_DISABLE_CLANG(text)   ZAPI_DO_PRAGMA(clang diagnostic ignored text)
#  define ZAPI_WARNING_DISABLE_GCC(text)
#  define ZAPI_WARNING_DISABLE_INTEL(number)
#  define ZAPI_WARNING_DISABLE_MSVC(number)
#  define ZAPI_WARNING_DISABLE_DEPRECATED    ZAPI_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#elif defined(ZAPI_CC_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define ZAPI_WARNING_PUSH                  ZAPI_DO_PRAGMA(GCC diagnostic push)
#  define ZAPI_WARNING_POP                   ZAPI_DO_PRAGMA(GCC diagnostic pop)
#  define ZAPI_WARNING_DISABLE_GCC(text)     ZAPI_DO_PRAGMA(GCC diagnostic ignored text)
#  define ZAPI_WARNING_DISABLE_CLANG(text)
#  define ZAPI_WARNING_DISABLE_INTEL(number)
#  define ZAPI_WARNING_DISABLE_MSVC(number)
#  define ZAPI_WARNING_DISABLE_DEPRECATED    ZAPI_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#else // All other compilers, GCC < 4.6 and MSVC < 2008
#  define ZAPI_WARNING_PUSH
#  define ZAPI_WARNING_POP
#  define ZAPI_WARNING_DISABLE_INTEL(number)
#  define ZAPI_WARNING_DISABLE_CLANG(text)
#  define ZAPI_WARNING_DISABLE_GCC(text)
#  define ZAPI_WARNING_DISABLE_MSVC(number)
#  define ZAPI_WARNING_DISABLE_DEPRECATED
#endif

#ifdef ZAPI_COMPILER_RVALUE_REFS
#  define ZAPI_move(x) std::move(x)
#else
#  define ZAPI_move(x) (x)
#endif

#if ZAPI_HAS_CPP_ATTRIBUTE(fallthrough)
#  define ZAPI_FALLTHROUGH() [[fallthrough]]
#elif defined(__cplusplus)
// Clang can not parse namespaced attributes in C mode, but defines __has_cpp_attribute
#  if ZAPI_HAS_CPP_ATTRIBUTE(clang::fallthrough)
#     define ZAPI_FALLTHROUGH() [[clang::fallthrough]]
#  elif ZAPI_HAS_CPP_ATTRIBUTE(gnu::fallthrough)
#     define ZAPI_FALLTHROUGH() [[gnu::fallthrough]]
#  endif
#endif

#ifndef ZAPI_FALLTHROUGH
#  if defined(ZAPI_CC_GNU) && ZAPI_CC_GNU >= 700
#    define ZAPI_FALLTHROUGH() __attribute__((fallthrough))
#  else
#    define ZAPI_FALLTHROUGH() (void)0
#  endif
#endif

#endif //ZAPI_COMPILERDETECTION_H
