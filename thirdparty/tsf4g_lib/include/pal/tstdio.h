/*
**  @file $RCSfile: tstdio.h,v $
**  general description of this module
**  $Id: tstdio.h,v 1.2 2009-01-23 09:34:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-23 09:34:20 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TSTDIO_H
#define TSTDIO_H

#include <fcntl.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


/* PS Method Start */

#if defined(_WIN32) || defined(_WIN64)

	#define	L_ctermid				9
	#define L_cuserid				9

#ifndef TMP_MAX
	#define TMP_MAX					238328
#endif

	/* vc can process below func
	#define fileno(fp)				_fileno(fp)
	#define fdopen(fd, mode)		_fdopen(fd, mode)
	#define getw(fp)				_getw(fp)
	#define mktemp(sz)				_mktemp(sz)
	#define putw(w, fp)				_putw(w, fp)
	#define tempnam(dir, prefix)	_tempnam(dir, prefix)
	*/
#if defined(_WIN32) || defined(_WIN64)
	#if  _MSC_VER < 1400
		#define snprintf				_snprintf
		#define vsnprintf				_vsnprintf
		#define snwprintf				_snwprintf
		#define vsnwprintf				_vsnwprintf
	#else
		#define snprintf(dest,count,fmt,...)				_snprintf_s(dest,count,_TRUNCATE ,fmt,__VA_ARGS__)
		#define vsnprintf(dest,count,fmt,valist)			_vsnprintf_s(dest,count,_TRUNCATE ,fmt,valist)
		#define snwprintf				_snwprintf
		#define vsnwprintf				_vsnwprintf
	#endif

#else
	#define snprintf				_snprintf
	#define vsnprintf				_vsnprintf
	#define snwprintf				_snwprintf
	#define vsnwprintf				_vsnwprintf
#endif
	//#define fpurge(fp)				fflush(fp)
	int fpurge(FILE *stream);

	//#define __fpurge(fp)			fflush(fp)
	void  __fpurge(FILE *stream);

	#define sys_errlist				_sys_errlist
	#define sys_nerr				_sys_nerr

	void setbuffer(FILE *a_fpStream, char *a_pszBuf, size_t a_iSize);
    void setlinebuf(FILE *a_fpStream);

#else
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* TSTDIO_H */
