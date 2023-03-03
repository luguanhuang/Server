/*
**  @file $RCSfile: tthread.h,v $
**  general description of this module
**  $Id: tthread.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TTHREAD_H
#define TTHREAD_H

/* PS Method Start */

#include "pal/tos.h"
#include "pal/tsched.h"
#include "pal/tsignal.h"
#include "pal/tmutex.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined(_WIN32) || defined(_WIN64)
#else
	#include <pthread.h>
#endif

#if defined(_WIN32) || defined(_WIN64)

	/* Thread identifiers */
/*	typedef HANDLE						pthread_t; */

#define TSF4G_LOCKNUM 20   /* 总共能支持使用的读写锁数目,目前读写锁仅用作tbus服务，其他服务使用前需要自行测试可靠性*/
	enum
	{
	  PTHREAD_CREATE_JOINABLE,
	#define PTHREAD_CREATE_JOINABLE	PTHREAD_CREATE_JOINABLE
	  PTHREAD_CREATE_DETACHED
	#define PTHREAD_CREATE_DETACHED	PTHREAD_CREATE_DETACHED
	};

	enum
	{
	  PTHREAD_INHERIT_SCHED,
	#define PTHREAD_INHERIT_SCHED	PTHREAD_INHERIT_SCHED
	  PTHREAD_EXPLICIT_SCHED
	#define PTHREAD_EXPLICIT_SCHED	PTHREAD_EXPLICIT_SCHED
	};

	enum
	{
	  PTHREAD_SCOPE_SYSTEM,
	#define PTHREAD_SCOPE_SYSTEM	PTHREAD_SCOPE_SYSTEM
	  PTHREAD_SCOPE_PROCESS
	#define PTHREAD_SCOPE_PROCESS	PTHREAD_SCOPE_PROCESS
	};

	enum
	{
	  PTHREAD_MUTEX_TIMED_NP,
	  PTHREAD_MUTEX_RECURSIVE_NP,
	  PTHREAD_MUTEX_ERRORCHECK_NP,
	  PTHREAD_MUTEX_ADAPTIVE_NP
	#ifdef __USE_UNIX98
	  ,
	  PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_TIMED_NP,
	  PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
	  PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
	  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
	#endif
	#ifdef __USE_GNU
	  /* For compatibility.  */
	  , PTHREAD_MUTEX_FAST_NP = PTHREAD_MUTEX_ADAPTIVE_NP
	#endif
	};

	enum
	{
	  PTHREAD_PROCESS_PRIVATE,
	#define PTHREAD_PROCESS_PRIVATE	PTHREAD_PROCESS_PRIVATE
	  PTHREAD_PROCESS_SHARED
	#define PTHREAD_PROCESS_SHARED	PTHREAD_PROCESS_SHARED
	};

	#if defined __USE_UNIX98 || defined __USE_XOPEN2K
	enum
	{
	  PTHREAD_RWLOCK_PREFER_READER_NP,
	  PTHREAD_RWLOCK_PREFER_WRITER_NP,
	  PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
	  PTHREAD_RWLOCK_DEFAULT_NP = PTHREAD_RWLOCK_PREFER_WRITER_NP
	};
	#endif	/* Unix98 */

	typedef long	pthread_once_t;
	typedef DWORD	pthread_key_t;

	#define PTHREAD_ONCE_INIT 0

	/* Special constants */

	#ifdef __USE_XOPEN2K
	/* -1 is distinct from 0 and all errno constants */
	# define PTHREAD_BARRIER_SERIAL_THREAD -1
	#endif

	/* Cleanup buffers */

	struct _pthread_cleanup_buffer
	{
	  void (*__routine) (void *);		  /* Function to call.  */
	  void *__arg;				  /* Its argument.  */
	  int __canceltype;			  /* Saved cancellation type. */
	  struct _pthread_cleanup_buffer *__prev; /* Chaining of cleanup functions.  */
	};

	/* Cancellation */

	enum
	{
	  PTHREAD_CANCEL_ENABLE,
	#define PTHREAD_CANCEL_ENABLE	PTHREAD_CANCEL_ENABLE
	  PTHREAD_CANCEL_DISABLE
	#define PTHREAD_CANCEL_DISABLE	PTHREAD_CANCEL_DISABLE
	};
	enum
	{
	  PTHREAD_CANCEL_DEFERRED,
	#define PTHREAD_CANCEL_DEFERRED	PTHREAD_CANCEL_DEFERRED
	  PTHREAD_CANCEL_ASYNCHRONOUS
	#define PTHREAD_CANCEL_ASYNCHRONOUS	PTHREAD_CANCEL_ASYNCHRONOUS
	};

	#define PTHREAD_CANCELED ((void *) -1)

	typedef struct
	{
		LPSECURITY_ATTRIBUTES psa;
		size_t stacksize;
		void * stackaddr;
		DWORD creationflags;
		int detachstate;
		int scope;
		int schedpolicy; /*supported values: SCHED_FIFO, SCHED_RR, and SCHED_OTHER*/
		struct sched_param schedparam;
		int inheritsched;
		int detach;
		size_t guardsize;
	} pthread_attr_t;

	typedef struct
	{
		HANDLE cond;
		int num_waiting;
	} pthread_cond_t;

	typedef struct
	{
		int pshared;
	} pthread_condattr_t;


	typedef struct
	{
		int protocol;
		int pshared;
		int prioceiling;
		int type;
	} pthread_mutexattr_t;


	typedef struct pthread_rwlock_t
	{
        /* 读线程读完后设置read_event,写进程等待read_event被设置 */
		HANDLE read_event;
        /* 互斥锁，写进程的上锁机制，读进程在修改readers变量时也会短暂加锁，随即释放 */
		HANDLE write_mutex;
        /* 标识当前读者数 */
		long readers;
	} pthread_rwlock_t;

	typedef struct pthread_rwlockattr_t
	{
		int lock;
		int pshared;
	} pthread_rwlockattr_t;

	typedef struct
	{
		HANDLE handle;
		unsigned int tid;
	} pthread_t;


	int   pthread_attr_destroy(pthread_attr_t *);
	int   pthread_attr_getdetachstate(const pthread_attr_t *, int *);
	int   pthread_attr_getguardsize(const pthread_attr_t *, size_t *);
	int   pthread_attr_getinheritsched(const pthread_attr_t *, int *);
	int   pthread_attr_getschedparam(const pthread_attr_t *, struct sched_param *);
	int   pthread_attr_getschedpolicy(const pthread_attr_t *, int *);
	int   pthread_attr_getscope(const pthread_attr_t *, int *);
	int   pthread_attr_getstackaddr(const pthread_attr_t *, void **);
	int   pthread_attr_getstacksize(const pthread_attr_t *, size_t *);
	int   pthread_attr_init(pthread_attr_t *);
	int   pthread_attr_setdetachstate(pthread_attr_t *, int);
	int   pthread_attr_setguardsize(pthread_attr_t *, size_t);
	int   pthread_attr_setinheritsched(pthread_attr_t *, int);
	int   pthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *);
	int   pthread_attr_setschedpolicy(pthread_attr_t *, int);
	int   pthread_attr_setscope(pthread_attr_t *, int);
	int   pthread_attr_setstackaddr(pthread_attr_t *, void *);
	int   pthread_attr_setstacksize(pthread_attr_t *, size_t);
	int   pthread_cancel(pthread_t);
	void  pthread_cleanup_push(void*);
	void  pthread_cleanup_pop(int);
	int   pthread_cond_broadcast(pthread_cond_t *);
	int   pthread_cond_destroy(pthread_cond_t *);
	int   pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
	int   pthread_cond_signal(pthread_cond_t *);
	int   pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);
	int   pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
	int   pthread_condattr_destroy(pthread_condattr_t *);
	int   pthread_condattr_getpshared(const pthread_condattr_t *, int *);
	int   pthread_condattr_init(pthread_condattr_t *);
	int   pthread_condattr_setpshared(pthread_condattr_t *, int);
	int   pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
	int   pthread_detach(pthread_t);
	int   pthread_equal(pthread_t, pthread_t);
	void  pthread_exit(void *);
	int   pthread_getconcurrency(void);
	int   pthread_getschedparam(pthread_t, int *, struct sched_param *);
	int   pthread_join(pthread_t, void **);
	int   pthread_mutexattr_destroy(pthread_mutexattr_t *);
	int   pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *, int *);
	int   pthread_mutexattr_getprotocol(const pthread_mutexattr_t *, int *);
	int   pthread_mutexattr_getpshared(const pthread_mutexattr_t *, int *);
	int   pthread_mutexattr_gettype(const pthread_mutexattr_t *, int *);
	int   pthread_mutexattr_init(pthread_mutexattr_t *);
	int   pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
	int   pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
	int   pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
	int   pthread_mutexattr_settype(pthread_mutexattr_t *, int);
    /** 以下函数目前仅用作tbus服务，其他服务使用前需要自行测试可靠性*/
	int   pthread_rwlock_destroy(pthread_rwlock_t *);
	int   pthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *);
	int   pthread_rwlock_rdlock(pthread_rwlock_t *);
	int   pthread_rwlock_tryrdlock(pthread_rwlock_t *);
	int   pthread_rwlock_trywrlock(pthread_rwlock_t *);
	int   pthread_rwlock_unlock(pthread_rwlock_t *);
	int   pthread_rwlock_wrlock(pthread_rwlock_t *);

	int   pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
	int   pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *, int *);
	int   pthread_rwlockattr_init(pthread_rwlockattr_t *);
	int   pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);
	pthread_t pthread_self(void);
	int	  pthread_kill(pthread_t thread, int sig);
	int   pthread_setcancelstate(int, int *);
	int   pthread_setcanceltype(int, int *);
	int   pthread_setconcurrency(int);
	int   pthread_setschedparam(pthread_t, int , const struct sched_param *);

	void  pthread_testcancel(void);

	int pthread_sigmask(int how, const sigset_t *newmask, sigset_t *oldmask);
	int sigwait(const sigset_t *set, int*sig);

    #define pthread_once(once, func) {          \
            if(1==InterlockedIncrement(once))   \
                func();                         \
        }

    #define pthread_key_create(pkey, func)  (((*(pkey) = TlsAlloc()) != TLS_OUT_OF_INDEXES) ? 0 : -1)
    #define pthread_key_delete(key)         (TlsFree(key) ? 0 : -1)
    #define pthread_setspecific(key, arg)   (TlsSetValue((key), (arg)) ? 0 : -1)
    #define pthread_getspecific(key)        TlsGetValue(key)

#else

	#define GetCurrentProcessId()		(DWORD) getpid()

	#define TlsFree(key)				( 0==pthread_key_delete(key) ? TRUE : FALSE )

	#define TlsSetValue(key, arg)		pthread_setspecific(key, arg)

	#define TlsGetValue(key)			( 0==pthread_getspecific(key) ? TRUE : FALSE )

#endif

/* PS Method End */

/* PA Method Start */

/* PA Method End */

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif /* TTHREAD_H */

