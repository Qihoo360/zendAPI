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

#ifndef ZAPI_SYSTEMDETECTION_H
#define ZAPI_SYSTEMDETECTION_H

/*
 * The operating system, must be one of: (ZAPI_OS_x)
 *
 * DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
 * MACOS    - macOS
 * MSDOS    - MS-DOS and windows
 * OS2      - OS/2
 * OS2EMX   - XFree86 on OS/2
 * WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
 * CYGWIN   - Cygwin
 * SOLARIS  - Sun Solaris
 * HPUX     - HP-UX
 * ULTRIX   - DEC Ultrix
 * LINUX    - Linux
 * FREEBSD  - FreeBSD [has variants]
 * NETBSD   - NetBSD
 * OPENBSD  - OpenBSD
 * BSDI     - BSD/OS
 * INTERIX  - Interix
 * IRIX     - SGI Irix
 * OSF      - HP Tru64 UNIX
 * SCO      - SCO OpenServer 5
 * UNIXWARE - UnixWare 7, Open UNIX 8
 * AIX      - AIX
 * HURD     - GNU Hurd
 * DGUX     - DG/UX
 * RELIANT  - Reliant UNIX
 * DYNIX    - DYNIX/ptx
 * QNX      - QNX [has variants]
 * QNX6     - QNX RTP 6.1
 * LYNX     - LynxOS
 * BSD4     - Any BSD 4.4 system
 * UNIX     - Any UNIX BSD/SYSV system
 * HAIKU    - Haiku
 *
 * The following operating system have variants:
 *    FREEBSD  - ZAPI_OS_FREEBSD is defined only when building for FreeBSD with a BSD userland
 *             - ZAPI_OS_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
 */
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define ZAPI_OS_DARWIN
#     define ZAPI_OS_BSD4
#     ifdef __LP64__
#        define ZAPI_OS_DARWIN64
#     else
#        define ZAPI_OS_DARWIN32
#     endif
#     ifdef TARGET_OS_MAC
#        define ZAPI_OS_MACOS
#     endif
#  else
#     error "libnotify has not been ported to this Apple platform"
#  endif
#elif defined(__CYGWIN__)
#  define ZAPI_OS_CYGWIN
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define ZAPI_OS_WIN32
#  define ZAPI_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINAPI_FAMILY)
#     ifndef WINAPI_FAMILY_PC_APP
#        define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#     endif
#     if WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
#        define ZAPI_OS_WINRT
#     elif WINAPI_FAMILY!=WINAPI_FAMILY_PC_APP && WINAPI_FAMILY!=WINAPI_FAMILY_PHONE_APP
#        define ZAPI_OS_WIN32
#     endif
#  else
#     define ZAPI_OS_WIN32
#  endif
#elif defined(__sun) || defined(sun)
#  define ZAPI_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define ZAPI_OS_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define ZAPI_OS_ULTRIX
#elif defined(sinix)
#  define ZAPI_OS_RELIANT
#elif defined(__native_client__)
#  define ZAPI_OS_NACL
#elif defined(__linux__) || defined(__linux)
#  define ZAPI_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#     define ZAPI_OS_FREEBSD
#  endif
#  define ZAPI_OS_FREEBSD_KERNEL
#  define ZAPI_OS_BSD4
#elif defined(__NetBSD__)
#  define ZAPI_OS_NETBSD
#  define ZAPI_OS_BSD4
#elif defined(__bsdi__)
#  define ZAPI_OS_BSDI
#  define ZAPI_OS_BSD4
#elif defined(__INTERIX)
#  define ZAPI_OS_INTERIX
#  define ZAPI_OS_BSD4
#elif defined(__sgi)
#  define ZAPI_OS_IRIX
#elif defined(__osf__)
#  define ZAPI_OS_OSF
#elif defined(_AIX)
#  define ZAPI_OS_AIX
#elif defined(__Lynx__)
#  define ZAPI_OS_LYNX
#elif defined(__GNU__)
#  define ZAPI_OS_HURD
#elif defined(__DGUX__)
#  define ZAPI_OS_DGUX
#elif defined(__QNXNTO__)
#  define ZAPI_OS_QNX
#elif defined(_SEQUENT_)
#  define ZAPI_OS_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define ZAPI_OS_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define ZAPI_OS_UNIXWARE
#elif defined(__srv4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define ZAPI_OS_UNIXWARE
#elif defined(__INTEGRITY)
#  define ZAPI_OS_INTEGRITY
#elif defined(__HAIKU__)
#  define ZAPI_OS_HAIKU
#else
#  error "libnotify has not been ported to this OS"
#endif

#if defined(ZAPI_OS_WIN32) || defined(ZAPI_OS_WIN64) || defined(ZAPI_OS_WINRT)
#  define ZAPI_OS_WIN
#endif

#if defined(ZAPI_OS_WIN)
#  undef ZAPI_OS_UNIX
#elif !defined(ZAPI_OS_UNIX)
#  define ZAPI_OS_UNIX
#endif

#ifdef ZAPI_OS_DARWIN
#  define ZAPI_OS_MAC
#endif
#ifdef ZAPI_OS_DARWIN32
#  define ZAPI_OS_MAC32
#endif
#ifdef  ZAPI_OS_DARWIN64
#  define ZAPI_OS_MAC64
#endif
#ifdef ZAPI_OS_MACOS
#  define ZAPI_OS_MACX
#  define ZAPI_OS_OSX
#endif

#ifdef ZAPI_OS_DARWIN
#  include <Availability.h>
#  include <AvailabilityMacros.h>
#
#  ifdef ZAPI_OS_MACOS
#     if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
#        undef __MAC_OS_X_VERSION_MIN_REQUIRED
#        define __MAC_OS_X_VERSION_MIN_REQUIRED _MAC_10_6
#     endif
#     if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#        undef MAC_OS_X_VERSION_MIN_REQUIRED
#        define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
#     endif
#  endif
#  if !defined(__MAC_10_7)
#     define __MAC_10_7 1070
#  endif
#  if !defined(__MAC_10_8)
#     define __MAC_10_8 1080
#  endif
#  if !defined(__MAC_10_9)
#     define __MAC_10_9 1090
#  endif
#  if !defined(__MAC_10_10)
#     define __MAC_10_10 1010
#  endif
#  if !defined(__MAC_10_11)
#     define __MAC_10_11 1011
#  endif
#  if !defined(__MAC_10_12)
#     define __MAC_10_12 101200
#  endif
#  if !defined(MAC_OS_X_VERSION_10_7)
#     define MAC_OS_X_VERSION_10_7 1070
#  endif
#  if !defined(MAC_OS_X_VERSION_10_8)
#     define MAC_OS_X_VERSION_10_8 1080
#  endif
#  if !defined(MAC_OS_X_VERSION_10_9)
#     define MAC_OS_X_VERSION_10_9 1090
#  endif
#  if !defined(MAC_OS_X_VERSION_10_10)
#       define MAC_OS_X_VERSION_10_10 101000
#  endif
#  if !defined(MAC_OS_X_VERSION_10_11)
#       define MAC_OS_X_VERSION_10_11 101100
#  endif
#  if !defined(MAC_OS_X_VERSION_10_12)
#       define MAC_OS_X_VERSION_10_12 101200
#  endif
#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#     error "This version of the Linux Standard Base is unsupported"
#  endif
#endif
#ifndef ZAPI_LINUXBASE
#  define ZAPI_LINUXBASE
#endif

#endif // ZAPI_SYSTEMDETECTION_H
