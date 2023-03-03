/*
**  @file $RCSfile: tmutex.h,v $
**  general description of this module
**  $Id: tmutex.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMUTEX_H
#define TMUTEX_H

#include "pal/tos.h"

/*********************************************
#FUNCTION:	This head file definite the mutex operation.
#AUTHOR:	Niezhiming
#DATE:		12-11-2002
**********************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/* PA Method Start */

#if defined(_WIN32) || defined(_WIN64)
	#define TMUTEX							CRITICAL_SECTION
	#define TMUTEX_INIT(pmutex)				(InitializeCriticalSection(pmutex), 0)
	#define TMUTEX_RELEASE(pmutex)			(DeleteCriticalSection(pmutex), 0)
	#define TMUTEX_LOCK(pmutex)				(EnterCriticalSection(pmutex),0)
	#define TMUTEX_TRYLOCK(pmutex)			(TryEnterCriticalSection(pmutex)?0:-1)
	#define TMUTEX_UNLOCK(pmutex)			(LeaveCriticalSection(pmutex),0)
#else
	#define TMUTEX							pthread_mutex_t
	#define TMUTEX_INIT(pmutex)				pthread_mutex_init(pmutex, NULL)
	#define TMUTEX_RELEASE(pmutex)			pthread_mutex_destroy(pmutex)
	#define TMUTEX_LOCK(pmutex)				pthread_mutex_lock(pmutex)
	#define TMUTEX_TRYLOCK(pmutex)			pthread_mutex_trylock(pmutex)
	#define TMUTEX_UNLOCK(pmutex)			pthread_mutex_unlock(pmutex)
#endif

/* PA Method End */

/* PS Method Start */

#if defined(_WIN32) || defined(_WIN64)
	#define PTHREAD_MUTEX_INITIALIZER				{0}
	#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP	{0}
	#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP	{0}

	#define pthread_mutex_t						CRITICAL_SECTION
	#define pthread_mutex_init(pmutex, init)	(InitializeCriticalSection(pmutex),0)
	#define pthread_mutex_destroy(pmutex)		(DeleteCriticalSection(pmutex), 0)
	#define pthread_mutex_lock(pmutex)			(EnterCriticalSection(pmutex), 0)
	#define pthread_mutex_trylock(pmutex)		(TryEnterCriticalSection(pmutex)?0:-1)
	#define pthread_mutex_unlock(pmutex)		(LeaveCriticalSection(pmutex), 0)
#else
	#define CRITICAL_SECTION					pthread_mutex_t
	#define InitializeCriticalSection(pmutex)	pthread_mutex_init(pmutex, NULL)
	#define DeleteCriticalSection(pmutex)		pthread_mutex_destroy(pmutex)
	#define EnterCriticalSection(pmutex)		pthread_mutex_lock(pmutex)
	#define TryEnterCriticalSection(pmutex)		((0==pthread_mutex_trylock(pmutex))?TRUE:FALSE)
	#define LeaveCriticalSection(pmutex)		pthread_mutex_unlock(pmutex)
#endif

/* PS Method End */

#ifdef __cplusplus
}
#endif


#endif /* TMUTEX_H */

