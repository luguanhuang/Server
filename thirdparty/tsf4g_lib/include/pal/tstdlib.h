/*
**  @file $RCSfile: tstdlib.h,v $
**  general description of this module
**  $Id: tstdlib.h,v 1.2 2009-01-23 09:34:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-23 09:34:20 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSTDLIB_H
#define TSTDLIB_H

#include <stdlib.h>
#include "pal/ttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)

	#if _INTEGRAL_MAX_BITS >= 64

		#if _MSVCVER >= 1300
		#define strtoll(nptr, endptr, base)		_strtoi64(nptr, endptr, base)
		#define strtoull(nptr, endptr, base)	_strtoui64(nptr, endptr, base)
		#else
		int64_t strtoll(const char *a_pszStr, char **a_ppszEndPtr, int a_iBase);
		uint64_t strtoull(const char *a_pszStr, char **a_ppszEndPtr, int a_iBase);
		#endif
	#else /* _INTEGRAL_MAX_BITS */

	long long strtoll(const char *a_pszStr, char **a_ppszEndPtr, int a_iBase);
	unsigned long long strtoull(const char *a_pszStr, char **a_ppszEndPtr, int a_iBase);

	#endif /* _INTEGRAL_MAX_BITS */

	//#define atoll(nptr)			strtoll(nptr, (char**)0, 10)
	longlong atoll(const char *nptr);

	//#define atoq(nptr)			strtoll(nptr, (char**)0, 10)
	longlong atoq(const char *nptr);

	/* vc can process below func
	#define getenv(name)		_getenv(name)
	#define putenv(str)			_putenv(str)
	*/

	double drand48(void);

	double erand48(unsigned short a_aushSub[3]);

	long int lrand48(void);

	long int nrand48(unsigned short a_aushSub[3]);

	long int mrand48(void);

	long int jrand48(unsigned short a_aushSub[3]);

	void srand48(long int a_lSeed);

	unsigned short *seed48(unsigned short a_aushSeed[3]);

	void lcong48(unsigned short a_aushParam[7]);

#else

	#define _strtoi64(nptr, endptr, base)		strtoll(nptr, endptr, base)
	#define _strtoui64(nptr, endptr, base)		strtoull(nptr, endptr, base)

#endif

#ifdef __cplusplus
}
#endif

#endif /* TSTDLIB_H */
