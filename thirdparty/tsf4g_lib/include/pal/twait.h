/*
**  @file $RCSfile: twait.h,v $
**  general description of this module
**  $Id: twait.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TWAIT_H

#define TWAIT_H

#include "pal/tos.h"
#include "pal/ttypes.h"

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C"
{
#endif

/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* Bits in the fourth argument to `waitid'.  */
#define WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
#define WEXITED		4	/* Report dead child.  */
#define WCONTINUED	8	/* Report continued child.  */
#define WNOWAIT		0x01000000 /* Don't reap, just poll status.  */


/* Macros for constructing status values.  */
#define	W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	W_STOPCODE(sig)			((sig) << 8 | 0x7f)
#define W_CONTINUED				0xffff
#define	WCOREFLAG				0x80


/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define	WEXITSTATUS(status)	(((status) & 0xff00) >> 8)

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	WTERMSIG(status)	((status) & 0x7f)

/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define	WSTOPSIG(status)	WEXITSTATUS(status)

/* Nonzero if STATUS indicates normal termination.  */
#define	WIFEXITED(status)	(WTERMSIG(status) == 0)

/* Nonzero if STATUS indicates termination by a signal.  */
#define WIFSIGNALED(status) \
  (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)

/* Nonzero if STATUS indicates the child is stopped.  */
#define	WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)

/* Nonzero if STATUS indicates the child continued after a stop.  We only
   define this if <bits/waitflags.h> provides the WCONTINUED flag bit.  */
#ifdef WCONTINUED
# define WIFCONTINUED(status)	((status) == W_CONTINUED)
#endif

/* Nonzero if STATUS indicates the child dumped core.  */
#define	WCOREDUMP(status)	((status) & WCOREFLAG)

union wait
{
    int w_status;
    struct
	{
		unsigned int __w_termsig:7; /* Terminating signal.  */
		unsigned int __w_coredump:1; /* Set if dumped core.  */
		unsigned int __w_retcode:8; /* Return code if exited normally.  */
		unsigned int:16;
	} __wait_terminated;

	struct
	{
		unsigned int __w_stopval:8; /* W_STOPPED if stopped.  */
		unsigned int __w_stopsig:8; /* Stopping signal.  */
		unsigned int:16;
	} __wait_stopped;
};

# define w_termsig	__wait_terminated.__w_termsig
# define w_coredump	__wait_terminated.__w_coredump
# define w_retcode	__wait_terminated.__w_retcode
# define w_stopsig	__wait_stopped.__w_stopsig
# define w_stopval	__wait_stopped.__w_stopval

pid_t wait(int *a_piStatus);
pid_t waitpid(pid_t a_iPid, int *a_piStatus, int a_iOptions);

#ifdef __cplusplus
}
#endif

#endif

#endif /* TWAIT_H */

