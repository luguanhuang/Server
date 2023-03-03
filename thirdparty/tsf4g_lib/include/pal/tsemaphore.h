/*
**  @file $RCSfile: tsemaphore.h,v $
**  general description of this module
**  $Id: tsemaphore.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSEMAPHORE_H
#define TSEMAPHORE_H

#if defined(_WIN32) || defined(_WIN64)

#include "pal/tos.h"

#define SEM_FAILED -1
#define SEM_VALUE_MAX 0x7fffffff

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	HANDLE sem;
	int lockedOrReferenced;
} sem_t;

int sem_init(sem_t *, int, unsigned);
int sem_close(sem_t *);
int sem_destroy(sem_t *);
int sem_getvalue(sem_t *, int *);
sem_t *sem_open(const char *, int, ...);
int sem_post(sem_t *);
int sem_timedwait(sem_t *, const struct timespec *);
int sem_trywait(sem_t *);
int sem_unlink(const char *);
int sem_wait(sem_t *);


#ifdef __cplusplus
}
#endif

#else
#include <semaphore.h>
#endif

#endif /* TSEMAPHORE_H */
