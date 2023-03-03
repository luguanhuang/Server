/*
**  @file $RCSfile: tsched.h,v $
**  general description of this module
**  $Id: tsched.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSCHED_H
#define TSCHED_H

#if defined(_WIN32) || defined(_WIN64)

/* Scheduling algorithms.  */
#define SCHED_OTHER	0
#define SCHED_FIFO	1
#define SCHED_RR	2

/* The official definition.  */
struct sched_param
{
	int __sched_priority;
};

#endif

#endif /* TSCHED_H */
