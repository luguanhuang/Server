/*
**  @file $RCSfile: tsignal.h,v $
**  general description of this module
**  $Id: tsignal.h,v 1.3 2009-04-28 08:20:57 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-04-28 08:20:57 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSIGNAL_H

#define TSIGNAL_H

#include "pal/ttypes.h"
#include "pal/ttime.h"

#include "pal/trules.h"

#if defined(_WIN32) || defined(_WIN64)
#else
	#include <signal.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define TSIGHUP		 1
#define TSIGINT		 2
#define TSIGQUIT	 3
#define TSIGILL		 4
#define TSIGTRAP	 5
#define TSIGABRT	 6
#define TSIGIOT		 6
#define TSIGBUS		 7
#define TSIGFPE		 8
#define TSIGKILL	 9
#define TSIGUSR1	10
#define TSIGSEGV	11
#define TSIGUSR2	12
#define TSIGPIPE	13
#define TSIGALRM	14
#define TSIGTERM	15
#define TSIGSTKFLT	16
#define TSIGCHLD	17
#define TSIGCONT	18
#define TSIGSTOP	19
#define TSIGTSTP	20
#define TSIGTTIN	21
#define TSIGTTOU	22
#define TSIGURG		23
#define TSIGXCPU	24
#define TSIGXFSZ	25
#define TSIGVTALRM	26
#define TSIGPROF	27
#define TSIGWINCH	28
#define TSIGIO		29
#define TSIGPWR		30
#define	TSIGUNUSED	31

#if defined(_WIN32) || defined(_WIN64)

#if defined(_INC_SIGNAL) && !defined(TSIGNAL_BASIC)
	#error	"Do not include <signal.h>, include \"pal/pal.h\" instead. or you can add TSIGNAL_BASIC macro in preprocessor"
#endif

#ifndef _SIGNAL_H
#define _SIGNAL_H
#endif /* _SIGNAL_H */

#include "pal/siginfo.h"

#ifndef _INC_SIGNAL

/* for windows */
#define _NSIG		32

#define NSIG		_NSIG

#define SIGHUP		TSIGHUP
#define SIGINT		TSIGINT
#define SIGQUIT		TSIGQUIT
#define SIGILL		TSIGILL
#define SIGTRAP		TSIGTRAP
#define SIGABRT		TSIGABRT
#define SIGIOT		TSIGIOT
#define SIGBUS		TSIGBUS
#define SIGFPE		TSIGFPE
#define SIGKILL		TSIGKILL
#define SIGUSR1		TSIGUSR1
#define SIGSEGV		TSIGSEGV
#define SIGUSR2		TSIGUSR2
#define SIGPIPE		TSIGPIPE
#define SIGALRM		TSIGALRM
#define SIGTERM		TSIGTERM
#define SIGSTKFLT	TSIGSTKFL
#define SIGCHLD		TSIGCHLD
#define SIGCONT		TSIGCONT
#define SIGSTOP		TSIGSTOP
#define SIGTSTP		TSIGTSTP
#define SIGTTIN		TSIGTTIN
#define SIGTTOU		TSIGTTOU
#define SIGURG		TSIGURG
#define SIGXCPU		TSIGXCPU
#define SIGXFSZ		TSIGXFSZ
#define SIGVTALRM	TSIGVTALRM
#define SIGPROF		TSIGPROF
#define SIGWINCH	TSIGWINCH
#define SIGIO		TSIGIO
#define SIGPOLL		SIGIO
/*
#define SIGLOST		29
*/
#define SIGPWR		TSIGPWR
#define	SIGUNUSED	TSIGUNUSED
#define	SIGMAX		TSIGUNUSED

#define SIG_DFL ((void (*)(int))(0))
#define SIG_ERR	((void (*)(int))(-1))
#define SIG_IGN	((void (*)(int))(1))

#define SIG_BLOCK		1
#define SIG_SETMASK		2
#define SIG_UNBLOCK		3

#define SA_NOCLDSTOP	1
#define SA_NOCLDWAIT	2		/* Don't create zombie on child death.  */
#define SA_SIGINFO		4		/* Invoke signal-catching function with
                                    three arguments instead of one.  */

#define SA_ONSTACK		0x08000000 /* Use signal stack by using `sa_restorer'. */
#define SA_RESTART		0x10000000 /* Restart syscall on signal return.  */
#define SA_NODEFER		0x40000000 /* Don't automatically block the signal when
                                    its handler is being executed.  */
#define SA_RESETHAND	0x80000000 /* Reset to SIG_DFL on entry to handler.  */

#define SA_INTERRUPT	0x20000000 /* Historical no-op.  */

/* Some aliases for the SA_ constants.  */
#define SA_NOMASK		SA_NODEFER
#define SA_ONESHOT		SA_RESETHAND
#define SA_STACK		SA_ONSTACK

#endif /* INC_SIGNAL */

#ifndef _SIG_ATOMIC_T_DEFINED
typedef int sig_atomic_t;
#define _SIG_ATOMIC_T_DEFINED
#endif

#ifndef _SIG_T_DEFINED
typedef int sig_t;
#define _SIG_T_DEFINED
#endif

typedef struct
{
	unsigned long __bits[1]; /* max 32 signals */
} sigset_t;

typedef void (*sighandler_t)(int);

struct sigaction
{
	int sa_flags;

	union
	{
		/* Used if SA_SIGINFO is not set.  */
        sighandler_t sa_handler;
        /* Used if SA_SIGINFO is set.  */
        void (*sa_sigaction) (int, siginfo_t *, void *);
	} __sigaction_handler;
# define sa_handler     __sigaction_handler.sa_handler
# define sa_sigaction   __sigaction_handler.sa_sigaction

	sigset_t sa_mask;

	/* Restore handler.  */
	void (*sa_restorer) (void);
};

struct tagTSigDesc
{
	struct sigaction stAct;
	HANDLE hSem;
};

typedef struct tagTSigDesc		TSIGDESC;
typedef struct tagTSigDesc		*PTSIGDESC;
typedef struct tagTSigDesc		*LPTSIGDESC;

#if 0
sighandler_t signal(int signum, sighandler_t handler);
int raise(int sig);
#endif

int	kill(pid_t _pid, int _sig);
int killpg(pid_t pgrp, int sig);
int	sigaction(int _sig, const struct sigaction *_act, struct sigaction *_oact);
int	sigaddset(sigset_t *_set, int _signo);
int	sigdelset(sigset_t *_set, int _signo);
int	sigemptyset(sigset_t *_set);
int	sigfillset(sigset_t *_set);
int	sigismember(const sigset_t *_set, int _signo);
int	sigpending(sigset_t *_set);
int	sigprocmask(int _how, const sigset_t *_set, sigset_t *_oset);
int	sigsuspend(const sigset_t *_set);

int sigwaitinfo(const sigset_t *set, siginfo_t *info);
int sigtimedwait(const sigset_t *set,  siginfo_t *info, const struct timespec *timeout);

extern char *sys_siglist[];

void	psignal(int sig, const char *msg);

#ifndef TSIGNAL_BASIC
	#define signal(sig, h)		tsignal(sig, h)
	#define raise(sig)			traise(sig)
#endif /* TSIGNAL_BASIC */


#define _INC_SIGNAL

#endif

#if !defined(__USE_GNU) && !defined(_WIN32) && !defined(_WIN64)
typedef void (*sighandler_t)(int);
#endif

/* PA Method Start. */

/** *******************************************************************
*	@brief  set the signal handler function for specified signal.
*	@param[in] a_iSig the signal which need to be change handler.
*	@param[in] a_pfnHandler the customed handler function for signal a_iSig.
*			there are some predefined value can be used:
*			SIG_DFL  use default handler by system.
*			SIG_IGN	 this signal will be ignored.
*	@return	the previous handler for this signal.
*	@retval SIG_ERR if there are errors, use errno for detail.
*	@retval !SIG_ERR the previous handler function, it can be SIG_DFL or SIG_IGN.
*********************************************************************/
sighandler_t tsignal(IN int a_iSig, IN sighandler_t a_pfnHandler);

/** *******************************************************************
*	@brief raise a signal, and throw the previous-set hanlder processing.
*	@param[in] a_iSig the signal which need to be raised.
*	@return whether the process is succeeded.
*	@retval  0 if success.
*	@retval  -1 if failed, use errno for detail.
*********************************************************************/
int traise(IN int a_iSig);

/* PA Method End. */

#ifdef __cplusplus
}
#endif

#endif /* TSIGNAL_H */


