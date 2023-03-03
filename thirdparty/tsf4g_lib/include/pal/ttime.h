/*
**  @file $RCSfile: ttime.h,v $
**  general description of this module
**  $Id: ttime.h,v 1.4 2009-10-23 03:56:12 flyma Exp $
**  @author $Author: flyma $
**  @date $Date: 2009-10-23 03:56:12 $
**  @version $Revision: 1.4 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TTIME_H
#define TTIME_H


#if defined(_WIN32) || defined(_WIN64)
	#define _POSIX_
		#include <time.h>
	#undef _POSIX_
    // in response to hardway's direct
	#pragma   comment(lib,"winmm.lib")

	#include <sys/timeb.h>
	#include <sys/utime.h>
#else
	#include <sys/time.h>
	#include <utime.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* PS Method Start */

#if defined(_WIN32) || defined(_WIN64)
	//#define	ftime(tp)			_ftime(tp)
	//#define	utime(path, buf)	_utime(path, buf)

/*
	#define tzset				_tzset
	#define tzname				_tzname
	#define timezone			_timezone
	#define daylight			_daylight
*/

	/* why use _timezone name? just for the above #define timezone _timezone */
	struct timezone
	{
		int  tz_minuteswest; /* minutes W of Greenwich */
		int  tz_dsttime;     /* type of dst correction */
	};

	struct timespec
	{
		time_t  tv_sec;    /*Seconds. */
		long    tv_nsec;   /*Nanoseconds. */
	};

	struct itimespec
	{
		struct timespec it_interval;  /*Timer period. */
		struct timespec it_value;     /*Timer expiration. */
	};

	#define ITIMER_REAL     0
	#define ITIMER_VIRTUAL	1
	#define ITIMER_PROF     2

	#define ITIMER_MAX_TIMER	3

	struct itimerval
	{
		struct  timeval it_interval;    /* timer interval */
		struct  timeval it_value;       /* current value */
	};

	int getitimer(int _which, struct itimerval *_value);
	int setitimer(int _which, struct itimerval *_value, struct itimerval *_ovalue);

	int nanosleep(const struct timespec *a_pstReq, struct timespec *a_pstRem);

	int gettimeofday(struct timeval *a_pstTv, struct timezone *a_pstTz);
	int settimeofday(const struct timeval *a_pstTv , const struct timezone *a_pstTz);
	int utimes(const char *a_pszFileName, const struct timeval a_astTv[2]);

	char* asctime_r(const struct tm *tm, char *buf);
	char* ctime_r(const time_t *timep, char *buf);
	struct tm* gmtime_r(const time_t *timep, struct tm *result);
	struct tm* localtime_r(const time_t *timep, struct tm *result);


	char *strptime(const char *a_pszStr, const char *a_pszFormat, struct tm *a_pstTm);

#else

#endif

/* PS Method End. */

/* PA Method Start  */

#define MS_TO_TV(tv, ms)		do									\
								{									\
									(tv).tv_sec = (ms)/1000; 		\
									(tv).tv_usec = ((ms) % 1000)*1000;\
								} while(0)

#define TV_TO_MS(ms, tv)		ms = (tv).tv_sec*1000 + (tv).tv_usec/1000

#define TV_DIFF(t, t1, t2)		do									\
								{									\
									if ((t1).tv_usec >= (t2).tv_usec)		\
									{								\
										(t).tv_sec =	(t1).tv_sec - (t2).tv_sec;\
										(t).tv_usec =	(t1).tv_usec - (t2).tv_usec;\
									}else										\
									{											\
										(t).tv_sec =	(t1).tv_sec - (t2).tv_sec -1;	\
										(t).tv_usec =	1000000 + (t1).tv_usec - (t2).tv_usec;\
									}											\
								} while(0)

#define TV_CLONE(dst, src)		do									\
								{									\
									(dst).tv_sec	=	(src).tv_sec; \
									(dst).tv_usec	=	(src).tv_usec; \
								} while(0)

/* PA Method End. */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TTIME_H */
