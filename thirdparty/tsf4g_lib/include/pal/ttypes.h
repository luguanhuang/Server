/*
 **  @file $RCSfile: ttypes.h,v $
 **  general description of this module
 **  $Id: ttypes.h,v 1.2 2009-01-23 09:34:20 kent Exp $
 **  @author $Author: kent $
 **  @date $Date: 2009-01-23 09:34:20 $
 **  @version $Revision: 1.2 $
 **  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
 **  @note Platform: Linux
 */

#ifndef TTYPES_H
#define TTYPES_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>

#if !defined(_WIN32) && !defined(_WIN64)
    #include <stdint.h>
    #include <inttypes.h>

    #undef PRIdPTR
    #if defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64)
        #define PRIdPTR "lu"
    #else
        #define PRIdPTR "u"
    #endif

    #undef PRIu64
    #if defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64)
        #define PRIu64 "lu"
    #else
        #define PRIu64 "llu"
    #endif

#else // #if !defined(_WIN32) && !defined(_WIN64)

    //The stdint declaras
    typedef signed char int8_t;
    typedef signed short int16_t;
    typedef signed int int32_t;
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;

    typedef signed __int64 int64_t;
    typedef unsigned __int64 uint64_t;

    /* The ISO C99 standard specifies that these macros must only be
       defined if explicitly requested.  */

    # if __WORDSIZE == 64
    #  define __PRI64_PREFIX	"l"
    #  define __PRIPTR_PREFIX	"l"
    # else
    #  define __PRI64_PREFIX	"ll"
    #  define __PRIPTR_PREFIX
    # endif

    /* Macros for printing format specifiers.  */

    /* Decimal notation.  */
    # define PRId8		"d"
    # define PRId16		"d"
    # define PRId32		"d"
    # define PRId64		__PRI64_PREFIX "d"

    # define PRIdLEAST8	"d"
    # define PRIdLEAST16	"d"
    # define PRIdLEAST32	"d"
    # define PRIdLEAST64	__PRI64_PREFIX "d"

    # define PRIdFAST8	"d"
    # define PRIdFAST16	__PRIPTR_PREFIX "d"
    # define PRIdFAST32	__PRIPTR_PREFIX "d"
    # define PRIdFAST64	__PRI64_PREFIX "d"

    # define PRIi8		"i"
    # define PRIi16		"i"
    # define PRIi32		"i"
    # define PRIi64		__PRI64_PREFIX "i"

    # define PRIiLEAST8	"i"
    # define PRIiLEAST16	"i"
    # define PRIiLEAST32	"i"
    # define PRIiLEAST64	__PRI64_PREFIX "i"

    # define PRIiFAST8	"i"
    # define PRIiFAST16	__PRIPTR_PREFIX "i"
    # define PRIiFAST32	__PRIPTR_PREFIX "i"
    # define PRIiFAST64	__PRI64_PREFIX "i"

    /* Octal notation.  */
    # define PRIo8		"o"
    # define PRIo16		"o"
    # define PRIo32		"o"
    # define PRIo64		__PRI64_PREFIX "o"

    # define PRIoLEAST8	"o"
    # define PRIoLEAST16	"o"
    # define PRIoLEAST32	"o"
    # define PRIoLEAST64	__PRI64_PREFIX "o"

    # define PRIoFAST8	"o"
    # define PRIoFAST16	__PRIPTR_PREFIX "o"
    # define PRIoFAST32	__PRIPTR_PREFIX "o"
    # define PRIoFAST64	__PRI64_PREFIX "o"

    /* Unsigned integers.  */
    # define PRIu8		"u"
    # define PRIu16		"u"
    # define PRIu32		"u"
    # define PRIu64		__PRI64_PREFIX "u"

    # define PRIuLEAST8	"u"
    # define PRIuLEAST16	"u"
    # define PRIuLEAST32	"u"
    # define PRIuLEAST64	__PRI64_PREFIX "u"

    # define PRIuFAST8	"u"
    # define PRIuFAST16	__PRIPTR_PREFIX "u"
    # define PRIuFAST32	__PRIPTR_PREFIX "u"
    # define PRIuFAST64	__PRI64_PREFIX "u"

    /* lowercase hexadecimal notation.  */
    # define PRIx8		"x"
    # define PRIx16		"x"
    # define PRIx32		"x"
    # define PRIx64		__PRI64_PREFIX "x"

    # define PRIxLEAST8	"x"
    # define PRIxLEAST16	"x"
    # define PRIxLEAST32	"x"
    # define PRIxLEAST64	__PRI64_PREFIX "x"

    # define PRIxFAST8	"x"
    # define PRIxFAST16	__PRIPTR_PREFIX "x"
    # define PRIxFAST32	__PRIPTR_PREFIX "x"
    # define PRIxFAST64	__PRI64_PREFIX "x"

    /* UPPERCASE hexadecimal notation.  */
    # define PRIX8		"X"
    # define PRIX16		"X"
    # define PRIX32		"X"
    # define PRIX64		__PRI64_PREFIX "X"

    # define PRIXLEAST8	"X"
    # define PRIXLEAST16	"X"
    # define PRIXLEAST32	"X"
    # define PRIXLEAST64	__PRI64_PREFIX "X"

    # define PRIXFAST8	"X"
    # define PRIXFAST16	__PRIPTR_PREFIX "X"
    # define PRIXFAST32	__PRIPTR_PREFIX "X"
    # define PRIXFAST64	__PRI64_PREFIX "X"

    /* Macros for printing `intmax_t' and `uintmax_t'.  */
    # define PRIdMAX	__PRI64_PREFIX "d"
    # define PRIiMAX	__PRI64_PREFIX "i"
    # define PRIoMAX	__PRI64_PREFIX "o"
    # define PRIuMAX	__PRI64_PREFIX "u"
    # define PRIxMAX	__PRI64_PREFIX "x"
    # define PRIXMAX	__PRI64_PREFIX "X"

    /* Macros for printing `intptr_t' and `uintptr_t'.  */
    # define PRIdPTR	__PRIPTR_PREFIX "d"
    # define PRIiPTR	__PRIPTR_PREFIX "i"
    # define PRIoPTR	__PRIPTR_PREFIX "o"
    # define PRIuPTR	__PRIPTR_PREFIX "u"
    # define PRIxPTR	__PRIPTR_PREFIX "x"
    # define PRIXPTR	__PRIPTR_PREFIX "X"

    /* Macros for scanning format specifiers.  */

    /* Signed decimal notation.  */
    # define SCNd8		"hhd"
    # define SCNd16		"hd"
    # define SCNd32		"d"
    # define SCNd64		__PRI64_PREFIX "d"

    # define SCNdLEAST8	"hhd"
    # define SCNdLEAST16	"hd"
    # define SCNdLEAST32	"d"
    # define SCNdLEAST64	__PRI64_PREFIX "d"

    # define SCNdFAST8	"hhd"
    # define SCNdFAST16	__PRIPTR_PREFIX "d"
    # define SCNdFAST32	__PRIPTR_PREFIX "d"
    # define SCNdFAST64	__PRI64_PREFIX "d"

    /* Signed decimal notation.  */
    # define SCNi8		"hhi"
    # define SCNi16		"hi"
    # define SCNi32		"i"
    # define SCNi64		__PRI64_PREFIX "i"

    # define SCNiLEAST8	"hhi"
    # define SCNiLEAST16	"hi"
    # define SCNiLEAST32	"i"
    # define SCNiLEAST64	__PRI64_PREFIX "i"

    # define SCNiFAST8	"hhi"
    # define SCNiFAST16	__PRIPTR_PREFIX "i"
    # define SCNiFAST32	__PRIPTR_PREFIX "i"
    # define SCNiFAST64	__PRI64_PREFIX "i"

    /* Unsigned decimal notation.  */
    # define SCNu8		"hhu"
    # define SCNu16		"hu"
    # define SCNu32		"u"
    # define SCNu64		__PRI64_PREFIX "u"

    # define SCNuLEAST8	"hhu"
    # define SCNuLEAST16	"hu"
    # define SCNuLEAST32	"u"
    # define SCNuLEAST64	__PRI64_PREFIX "u"

    # define SCNuFAST8	"hhu"
    # define SCNuFAST16	__PRIPTR_PREFIX "u"
    # define SCNuFAST32	__PRIPTR_PREFIX "u"
    # define SCNuFAST64	__PRI64_PREFIX "u"

    /* Octal notation.  */
    # define SCNo8		"hho"
    # define SCNo16		"ho"
    # define SCNo32		"o"
    # define SCNo64		__PRI64_PREFIX "o"

    # define SCNoLEAST8	"hho"
    # define SCNoLEAST16	"ho"
    # define SCNoLEAST32	"o"
    # define SCNoLEAST64	__PRI64_PREFIX "o"

    # define SCNoFAST8	"hho"
    # define SCNoFAST16	__PRIPTR_PREFIX "o"
    # define SCNoFAST32	__PRIPTR_PREFIX "o"
    # define SCNoFAST64	__PRI64_PREFIX "o"

    /* Hexadecimal notation.  */
    # define SCNx8		"hhx"
    # define SCNx16		"hx"
    # define SCNx32		"x"
    # define SCNx64		__PRI64_PREFIX "x"

    # define SCNxLEAST8	"hhx"
    # define SCNxLEAST16	"hx"
    # define SCNxLEAST32	"x"
    # define SCNxLEAST64	__PRI64_PREFIX "x"

    # define SCNxFAST8	"hhx"
    # define SCNxFAST16	__PRIPTR_PREFIX "x"
    # define SCNxFAST32	__PRIPTR_PREFIX "x"
    # define SCNxFAST64	__PRI64_PREFIX "x"

    /* Macros for scanning `intmax_t' and `uintmax_t'.  */
    # define SCNdMAX	__PRI64_PREFIX "d"
    # define SCNiMAX	__PRI64_PREFIX "i"
    # define SCNoMAX	__PRI64_PREFIX "o"
    # define SCNuMAX	__PRI64_PREFIX "u"
    # define SCNxMAX	__PRI64_PREFIX "x"

    /* Macros for scaning `intptr_t' and `uintptr_t'.  */
    # define SCNdPTR	__PRIPTR_PREFIX "d"
    # define SCNiPTR	__PRIPTR_PREFIX "i"
    # define SCNoPTR	__PRIPTR_PREFIX "o"
    # define SCNuPTR	__PRIPTR_PREFIX "u"
    # define SCNxPTR	__PRIPTR_PREFIX "x"

#endif // #if !defined(_WIN32) && !defined(_WIN64) #else

#if defined(_WIN32) || defined(_WIN64)
    #include <wchar.h>
    #include <limits.h>

    #if _MSC_VER >= 1400
        #include <crtdefs.h>
    #else
        typedef int intptr_t;
        typedef unsigned long ULONG_PTR;
        typedef ULONG_PTR *LPULONG_PTR;
        typedef ULONG_PTR *PULONG_PTR;
        typedef ULONG_PTR DWORD_PTR;
        typedef ULONG_PTR *LPDWORD_PTR;
        typedef ULONG_PTR *PDWORD_PTR;
    #endif

#endif // #if defined(_WIN32) || defined(_WIN64)

/* PS Method. */

#if defined(_WIN32) || defined(_WIN64)

    typedef __int64 longlong;
    typedef unsigned __int64 ulonglong;

    typedef int socklen_t;
    typedef int ssize_t;
    typedef long suseconds_t;

    typedef __int64 off64_t;
    typedef long off_t;

    #ifndef __int8_t_defined
    # define __int8_t_defined
    typedef signed char int8_t;
    typedef short int int16_t;
    typedef int int32_t;

    #if _MSC_VER >= 1300
    typedef long long int64_t;
    #else /* _MSC_VER */
    typedef __int64 int64_t;
    #endif /* _MSC_VER */
    #endif

    /* Unsigned.  */
    typedef unsigned char uint8_t;
    typedef unsigned char u_int8_t;

    typedef unsigned short int uint16_t;
    typedef unsigned short int u_int16_t;

    #ifndef __uint32_t_defined
    typedef unsigned int uint32_t;
    typedef unsigned int u_int32_t;
    # define __uint32_t_defined
    #endif

    #if _MSC_VER >= 1300
    typedef unsigned long long uint64_t;
    typedef unsigned long long u_int64_t;
    #else /* _MSC_VER */
    typedef unsigned __int64 uint64_t;
    typedef unsigned __int64 u_int64_t;
    #endif /* _MSC_VER */

    typedef int mode_t;

    typedef int key_t;

    typedef int pid_t;
    typedef int uid_t;
    typedef int gid_t;

#else // #if defined(_WIN32) || defined(_WIN64)

    /* call invention. */
    #define CALLBACK	__stdcall
    #define WINAPI		__stdcall

    /* basic macros. */
    #define CONST 		const
    #define VOID		void

    /* basic data type definition. */
    typedef long long longlong;
    typedef unsigned long long ulonglong;

    #ifndef PAL_NO_WIN_TYPES
        typedef CONST void *LPCVOID;
        typedef CONST void *PCVOID;

        typedef int HANDLE;
        typedef HANDLE *LPHANDLE;
        typedef HANDLE *PHANDLE;

        typedef HANDLE SOCKET;
        typedef SOCKET *LPSOCKET;
        typedef SOCKET *PSOCKET;

        typedef ulonglong ULONGLONG;
        typedef longlong LONGLONG;

        typedef ULONGLONG *LPULONGLONG;
        typedef ULONGLONG *PULONGLONG;

        typedef LONGLONG *LPLONGLONG;
        typedef LONGLONG *PLONGLONG;

        typedef unsigned char BYTE;
        typedef unsigned short WORD;
        typedef unsigned long DWORD;
        typedef unsigned int DWORD32;
        typedef ULONGLONG DWORD64;
        typedef ULONGLONG DWORDLONG;
        typedef int BOOL;
        typedef BYTE BOOLEAN;

        typedef BYTE *LPBYTE;
        typedef BYTE *PBYTE;
        typedef WORD *LPWORD;
        typedef WORD *PWORD;
        typedef DWORD *LPDWORD;
        typedef DWORD *PDWORD;
        typedef DWORD32 *LPDWORD32;
        typedef DWORD32 *PDWORD32;
        typedef DWORD64 *LPDWORD64;
        typedef DWORD64 *PDWORD64;
        typedef DWORDLONG *LPDWORDLONG;
        typedef DWORDLONG *PDWORDLONG;
        typedef BOOL *LPBOOL;
        typedef BOOL *PBOOL;
        typedef BOOLEAN *LPBOOLEAN;
        typedef BOOLEAN *PBOOLEAN;

        /* char/string definition. */
        typedef char CHAR;
        typedef unsigned char UCHAR;
        typedef wchar_t WCHAR;

        typedef CHAR* LPCHAR;
        typedef CHAR* PCHAR;

        typedef UCHAR* LPUCHAR;
        typedef UCHAR* PUCHAR;

        typedef WCHAR* LPWCHAR;
        typedef WCHAR* PWCHAR;

        #ifdef UNICODE
        typedef WCHAR TCHAR;
        #else
        typedef char TCHAR;
        #endif

        typedef TCHAR TBYTE;

        typedef TCHAR *LPTCHAR;
        typedef TCHAR *PTCHAR;

        typedef TBYTE *LPTBYTE;
        typedef TBYTE *PTBYTE;

        typedef CONST CHAR *LPCSTR;
        typedef CONST WCHAR *LPCWSTR;

        typedef CHAR *LPSTR;
        typedef WCHAR *LPWSTR;

        #ifdef UNICODE
        typedef LPCWSTR LPCTSTR;
        #else
        typedef LPCSTR LPCTSTR;
        #endif

        #ifdef UNICODE
        typedef LPWSTR LPTSTR;
        #else
        typedef LPSTR LPTSTR;
        #endif

        typedef CONST CHAR *PCSTR;
        typedef CONST WCHAR *PCWSTR;

        typedef CHAR *PSTR;
        typedef WCHAR *PWSTR;

        #ifdef UNICODE
        typedef PCWSTR PCTSTR;
        #else
        typedef PCSTR PCTSTR;
        #endif

        #ifdef UNICODE
        typedef PWSTR PTSTR;
        #else
        typedef PSTR PTSTR;
        #endif

        typedef DWORD COLORREF;
        typedef COLORREF *LPCOLORREF;
        typedef COLORREF *PCOLORREF;

        typedef WORD ATOM;

        typedef short SHORT;
        typedef unsigned short USHORT;

        typedef SHORT *LPSHORT;
        typedef SHORT *PSHORT;
        typedef USHORT *LPUSHORT;
        typedef USHORT *PUSHORT;

        /* int/int ptr */
        typedef int INT;
        typedef signed int INT32;
        typedef LONGLONG INT64;

        typedef INT *LPINT;
        typedef INT *PINT;
        typedef INT32 *LPINT32;
        typedef INT *PINT32;
        typedef INT64 *LPINT64;
        typedef INT *PINT64;

        typedef unsigned int UINT;
        typedef unsigned int UINT32;
        typedef ULONGLONG UINT64;

        typedef UINT *LPUINT;
        typedef UINT *PUINT;
        typedef UINT32 *LPUINT32;
        typedef UINT32 *PUINT32;
        typedef UINT64 *LPUINT64;
        typedef UINT64 *PUINT64;

        typedef long LONG;
        typedef int LONG32;
        typedef LONGLONG LONG64;

        typedef LONG *LPLONG;
        typedef LONG *PLONG;
        typedef LONG32 *LPLONG32;
        typedef LONG32 *PLONG32;
        typedef LONG64 *LPLONG64;
        typedef LONG64 *PLONG64;

        typedef unsigned long ULONG;
        typedef unsigned int ULONG32;
        typedef unsigned long long ULONG64;

        typedef ULONG *LPULONG;
        typedef ULONG *PULONG;
        typedef ULONG32 *LPULONG32;
        typedef ULONG32 *PULONG32;
        typedef ULONG64 *LPULONG64;
        typedef ULONG64 *PULONG64;

        #define POINTER_32

        #define POINTER_64

        typedef float FLOAT;

        typedef WORD LANGID;
        typedef DWORD LCID;
        typedef DWORD LCTYPE;
        typedef DWORD LGRPID;

        typedef LONGLONG USN;

        #ifdef __x86_64__
        typedef int HALF_PTR;
        #else
        typedef short HALF_PTR;
        #endif

        #ifdef __x86_64__
        typedef unsigned int UHALF_PTR;
        #else
        typedef unsigned short UHALF_PTR;
        #endif

        typedef HALF_PTR *LPHALF_PTR;
        typedef HALF_PTR *PHALF_PTR;
        typedef UHALF_PTR *LPUHALF_PTR;
        typedef UHALF_PTR *PUHALF_PTR;

        #ifdef __x86_64__
        typedef long long INT_PTR;
        #else
        typedef int INT_PTR;
        #endif

        typedef INT_PTR *LPINT_PTR;
        typedef INT_PTR *PINT_PTR;

        #ifdef __x86_64__
        typedef unsigned long long UINT_PTR;
        #else
        typedef unsigned int UINT_PTR;
        #endif

        typedef UINT_PTR *LPUINT_PTR;
        typedef UINT_PTR *PUINT_PTR;

        #ifdef __x86_64__
        typedef long long LONG_PTR;
        #else
        typedef long LONG_PTR;
        #endif

        typedef LONG_PTR *LPLONG_PTR;
        typedef LONG_PTR *PLONG_PTR;

        #ifdef __x86_64__
        typedef unsigned long long ULONG_PTR;
        #else
        typedef unsigned long ULONG_PTR;
        #endif

        typedef ULONG_PTR *LPULONG_PTR;
        typedef ULONG_PTR *PULONG_PTR;

        typedef ULONG_PTR DWORD_PTR;
        typedef ULONG_PTR *LPDWORD_PTR;
        typedef ULONG_PTR *PDWORD_PTR;

        typedef ULONG_PTR SIZE_T;
        typedef LONG_PTR SSIZE_T;

        typedef LONG_PTR LPARAM;
        typedef UINT_PTR WPARAM;
    #endif /* PAL_NO_WIN_TYPES */

#endif // #if defined(_WIN32) || defined(_WIN64) #else


#ifndef tmax
#define tmax(x,y) (((x) > (y))? (x):(y))
#endif

#ifndef tmin
#define tmin(x,y) (((x) > (y))? (y):(x))
#endif


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TTYPES_H */
