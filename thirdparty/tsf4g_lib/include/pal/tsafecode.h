/*
**  @file $RCSfile: tsafecode.h,v $
**  general description of this module
**  $Id: tsafecode.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSAFECODE_H

#define TSAFECODE_H

#ifndef TSF4G_SAFECODE
#else	/* TSF4G_SAFECODE */

/* unsafe ansi string function. */

#define strcpy
#define wcscpy

#define strcat
#define wcscat

#define strncat
#define wcsncat

#ifdef TSF4G_SAFECODE_STRNCPY
#else	/* TSF4G_SAFECODE_STRNCPY */
	#define strncpy
#endif	/* TSF4G_SAFECODE_STRNCPY */

#define sprintf
#define swprintf

#define vsprintf
#define vswprintf

#define gets

/* unsafe ms-specific function. */

#if defined(_WIN32) || defined(_WIN64)

#define lstrcpy
#define _tcscpy
#define _ftcscpy
#define _mbscpy

#define lstrcat
#define _tcscat
#define _ftcscat
#define _mbscat

#define wsprintf
#define wnsprintf
#define _stprintf

#define wvsprintf
#define wvnsprintf
#define _vstprintf

#endif

#endif 	/* TSF4G_SAFECODE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* TSAFECODE_H */
