/*
**  @file $RCSfile: tmmap.h,v $
**  general description of this module
**  $Id: tmmap.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMMAP_H
#define TMMAP_H

/*********************************************
#FUNCTION:	This head file definite the general filemap operation.
#AUTHOR:	Niezhiming
#DATE:		15-5-2002
**********************************************/

#if defined(_WIN32) || defined(_WIN64)
#else
	#include <sys/mman.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif


/* PS Method Start. */

#if defined(_WIN32) || defined(_WIN64)


	#define PROT_NONE				PAGE_NONE
	#define PROT_EXEC				PAGE_EXECUTE
	#define PROT_READ				PAGE_READONLY
	#define PROT_WRITE				PAGE_READWRITE
	#define PROT_RDWR				PAGE_READWRITE


	/* Return value of `mmap' in case of an error.  */
	#define MAP_FAILED      ((void *) -1)

	/* Protections are chosen from these bits, OR'd together.  The
	implementation does not necessarily support PROT_EXEC or PROT_WRITE
	without PROT_READ.  The only guarantees are that no writing will be
	allowed without PROT_WRITE and no access will be allowed for PROT_NONE. */

#if 0
	#define PROT_READ       0x1             /* Page can be read.  */
	#define PROT_WRITE      0x2             /* Page can be written.  */
	#define PROT_EXEC       0x4             /* Page can be executed.  */
	#define PROT_NONE       0x0             /* Page can not be accessed.  */
	#define PROT_RDWR		(PROT_READ | PROT_WRITE)
#endif

	#define PROT_GROWSDOWN  0x01000000      /* Extend change to start of
	                                           growsdown vma (mprotect only).  */
	#define PROT_GROWSUP    0x02000000      /* Extend change to start of
	                                           growsup vma (mprotect only).  */

	/* Sharing types (must choose one and only one of these).  */
	#define MAP_SHARED      0x01            /* Share changes.  */
	#define MAP_PRIVATE     0x02            /* Changes are private.  */

	# define MAP_TYPE       0x0f            /* Mask for type of mapping.  */

	#define MAP_FILE       0
	#define MAP_ANONYMOUS  0x20            /* Don't use a file.  */
	#define MAP_ANON       MAP_ANONYMOUS

	/* These are Linux-specific.  */
	#define MAP_GROWSDOWN  0x00100         /* Stack-like segment.  */
	#define MAP_DENYWRITE  0x00800         /* ETXTBSY */
	#define MAP_EXECUTABLE 0x01000         /* Mark it as an executable.  */
	#define MAP_LOCKED     0x02000         /* Lock the mapping.  */
	#define MAP_NORESERVE  0x04000         /* Don't check for reservations.  */
	#define MAP_POPULATE   0x08000         /* Populate (prefault) pagetables.  */
	#define MAP_NONBLOCK   0x10000         /* Do not block on IO.  */

	/* Other flags.  */
	#define MAP_FIXED       0x10            /* Interpret addr exactly.  */

	/* Flags to `msync'.  */
	#define MS_ASYNC        1               /* Sync memory asynchronously.  */
	#define MS_SYNC         4               /* Synchronous memory sync.  */
	#define MS_INVALIDATE   2               /* Invalidate the caches.  */

	/* Flags for `mlockall'.  */
	#define MCL_CURRENT     1               /* Lock all currently mapped pages.  */
	#define MCL_FUTURE      2               /* Lock all additions to address
	                                           space.  */

	void* mmap(void *a_pvStart, size_t a_iLength, int a_iProt , int a_iFlags, HANDLE a_iFile, off_t a_iOffset);
	int munmap(void *a_pvStart, size_t a_iLength);
	int mprotect(const void *a_pvAddr, size_t a_iLen, int a_iProt);
	int mlock(const void *a_pvAddr, size_t a_iLen);
	int munlock(const void *addr, size_t a_iLen);
	int mlockall(int a_iFlags);
	int munlockall(void);
	int msync(void *a_pvStart, size_t a_iLength, int a_iFlags);

	int shm_open(const char *a_pszName, int a_iFlag, mode_t a_iMode);
	int shm_unlink(const char *a_pszName);

#else
	#ifndef PROT_RDWR
	#define PROT_RDWR				(PROT_READ | PROT_WRITE)
	#endif	/* PROT_RDWR */

#endif

/* PS Method End. */


#define TMMAPF_CREATE				0x10000
#define TMMAPF_DELETE				TMMAPF_CREATE
#define TMMAPF_EXCL					0x20000
#define TMMAPF_PRIVATE				0x40000		/* this flags only used in windows platform.*/

#define TMMAPF_READ					0x100000
#define TMMAPF_WRITE				0x200000

#define TMMAPF_ACCESS_MASK			0xf00000

#define TMMAPF_PROT_READ			0x1000000
#define TMMAPF_PROT_WRITE			0x2000000
#define TMMAPF_PROT_EXEC			0x4000000

HANDLE tmmapopen(const char* a_pszPath, int a_iSize, int a_iFlags);

int tmmapclose(HANDLE a_iMap);

void* tmmap(HANDLE a_iMap, int a_iStart, int a_iSize, int a_iFlags);

int tmunmap(void* a_pvBase, int a_iSize);

int tmprotect(const void* a_pvBase, size_t a_dwSize, int a_iProt);

int tmsync(void* a_pvAddr, int a_iSize);

int tmpsync(void* a_pvAddr, int a_iSize, int a_iPage);

int tmlock(const void *a_pvAddr, size_t a_iLen);

int tmunlock(const void *a_pvAddr, size_t a_iLen);

int tmlockall(int a_iFlags);

int tmunlockall(void);

#ifdef __cplusplus
}
#endif


#endif /* TMMAP_H */
