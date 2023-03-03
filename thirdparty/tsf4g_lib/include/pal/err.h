/*
**  @file $RCSfile: err.h,v $
**  general description of this module
**  $Id: err.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef ERR_H
#define ERR_H

#include <sys/cdefs.h>
#include <stdarg.h>

#ifdef __cpluscplus
extern "C"
{
#endif

void warn (const char *fmt, ...);
void warnx (const char *fmt, ...);

void errx (int eval, const char *fmt, ...);

void vwarn (const char *fmt, va_list ap);
void vwarnx (const char *fmt, va_list ap);

void verr (int eval, const char *fmt, va_list ap);
void verrx (int eval, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif /* ERR_H */
