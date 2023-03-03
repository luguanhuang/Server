/*
**  @file $RCSfile: tfcntl.h,v $
**  general description of this module
**  $Id: tfcntl.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TFCNTL_H
#define TFCNTL_H

#include "pal/ttypes.h"
#include <fcntl.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)

/* Values for the second argument to `fcntl'.  */
#define F_DUPFD		0	/* Duplicate file descriptor.  */
#define F_GETFD		1	/* Get file descriptor flags.  */
#define F_SETFD		2	/* Set file descriptor flags.  */
#define F_GETFL		3	/* Get file status flags.  */
#define F_SETFL		4	/* Set file status flags.  */
#ifndef __USE_FILE_OFFSET64
# define F_GETLK	5	/* Get record locking info.  */
# define F_SETLK	6	/* Set record locking info (non-blocking).  */
# define F_SETLKW	7	/* Set record locking info (blocking).	*/
#else
# define F_GETLK	F_GETLK64  /* Get record locking info.	*/
# define F_SETLK	F_SETLK64  /* Set record locking info (non-blocking).*/
# define F_SETLKW	F_SETLKW64 /* Set record locking info (blocking).  */
#endif
#define F_GETLK64	12	/* Get record locking info.  */
#define F_SETLK64	13	/* Set record locking info (non-blocking).  */
#define F_SETLKW64	14	/* Set record locking info (blocking).	*/

# define F_SETOWN	8	/* Get owner of socket (receiver of SIGIO).  */
# define F_GETOWN	9	/* Set owner of socket (receiver of SIGIO).  */

# define F_SETSIG	10	/* Set number of signal to be sent.  */
# define F_GETSIG	11	/* Get number of signal to be sent.  */

# define F_SETLEASE	1024	/* Set a lease.	 */
# define F_GETLEASE	1025	/* Enquire what lease is active.  */
# define F_NOTIFY	1026	/* Request notfications on a directory.	 */

/* For F_[GET|SET]FL.  */
#define FD_CLOEXEC	1	/* actually anything with low bit set goes */

/* For posix fcntl() and `l_type' field of a `struct flock' for lockf().  */
#define F_RDLCK		0	/* Read lock.  */
#define F_WRLCK		1	/* Write lock.	*/
#define F_UNLCK		2	/* Remove lock.	 */

/* For old implementation of bsd flock().  */
#define F_EXLCK		4	/* or 3 */
#define F_SHLCK		8	/* or 4 */

/* Operations for bsd flock(), also used by the kernel implementation.	*/
# define LOCK_SH	1	/* shared lock */
# define LOCK_EX	2	/* exclusive lock */
# define LOCK_NB	4	/* or'd with one of the above to prevent
				   blocking */
# define LOCK_UN	8	/* remove lock */

# define LOCK_MAND	32	/* This is a mandatory flock:	*/
# define LOCK_READ	64	/* ... which allows concurrent read operations.	 */
# define LOCK_WRITE	128	/* ... which allows concurrent write operations.  */
# define LOCK_RW	192	/* ... Which allows concurrent read & write operations.	 */

/* Types of directory notifications that may be requested with F_NOTIFY.  */
# define DN_ACCESS	0x00000001	/* File accessed.  */
# define DN_MODIFY	0x00000002	/* File modified.  */
# define DN_CREATE	0x00000004	/* File created.  */
# define DN_DELETE	0x00000008	/* File removed.  */
# define DN_RENAME	0x00000010	/* File renamed.  */
# define DN_ATTRIB	0x00000020	/* File changed attibutes.  */
# define DN_MULTISHOT	0x80000000	/* Don't remove notifier.  */

struct flock
  {
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
#ifndef __USE_FILE_OFFSET64
    off_t l_start;	/* Offset where the lock begins.  */
    off_t l_len;	/* Size of the locked area; zero means until EOF.  */
#else
    off64_t l_start;	/* Offset where the lock begins.  */
    off64_t l_len;	/* Size of the locked area; zero means until EOF.  */
#endif
    pid_t l_pid;	/* Process holding the lock.  */
  };

#ifdef __USE_LARGEFILE64
struct flock64
  {
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    off64_t l_start;	/* Offset where the lock begins.  */
    off64_t l_len;	/* Size of the locked area; zero means until EOF.  */
    pid_t l_pid;	/* Process holding the lock.  */
  };
#endif

#else

#endif

#ifdef __cplusplus
}
#endif

#endif /* TFCNTL_H */
