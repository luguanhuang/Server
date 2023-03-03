/*
**  @file $RCSfile: tresource.h,v $
**  general description of this module
**  $Id: tresource.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TRESOURCE_H
#define TRESOURCE_H

#include "pal/ttime.h"

#if defined(_WIN32) || defined(_WIN64)
#else
	#include <sys/resource.h>
#endif

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C"
{
#endif

#define RUSAGE_SELF     0               /* calling process */
#define RUSAGE_CHILDREN -1              /* terminated child processes */

struct rusage {
  struct timeval ru_utime;	/* user time used */
  struct timeval ru_stime;	/* system time used */
  long ru_maxrss;		/* integral max resident set size */
  long ru_ixrss;		/* integral shared text memory size */
  long ru_idrss;		/* integral unshared data size */
  long ru_isrss;		/* integral unshared stack size */
  long ru_minflt;		/* page reclaims */
  long ru_majflt;		/* page faults */
  long ru_nswap;		/* swaps */
  long ru_inblock;		/* block input operations */
  long ru_oublock;		/* block output operations */
  long ru_msgsnd;		/* messages sent */
  long ru_msgrcv;		/* messages received */
  long ru_nsignals;		/* signals received */
  long ru_nvcsw;		/* voluntary context switches */
  long ru_nivcsw;		/* involuntary context switches */
};

#define RLIMIT_CPU	0	/* cpu time in milliseconds */
#define RLIMIT_FSIZE	1	/* maximum file size */
#define RLIMIT_DATA	2	/* data size */
#define RLIMIT_STACK	3	/* stack size */
#define RLIMIT_CORE	4	/* core file size */
#define RLIMIT_RSS	5	/* resident set size */
#define RLIMIT_MEMLOCK	6	/* locked-in-memory address space */
#define RLIMIT_NPROC	7	/* number of processes */
#define RLIMIT_NOFILE	8	/* number of open files */

#define RLIM_NLIMITS	9	/* number of resource limits */
#define RLIM_INFINITY	((long) ((1UL << 31) - 1UL))

struct rlimit {
  long rlim_cur;		/* current (soft) limit */
  long rlim_max;		/* maximum value for rlim_cur */
};

int getrusage(int _who, struct rusage *_rusage);
int getrlimit(int _rltype, struct rlimit *_rlimit);
int setrlimit(int _rltype, const struct rlimit *_rlimit);

#ifdef __cplusplus
}
#endif

#endif

#endif /* TRESOURCE_H */
