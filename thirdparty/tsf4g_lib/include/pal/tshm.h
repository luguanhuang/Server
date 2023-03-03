/*
**  @file $RCSfile: tshm.h,v $
**  general description of this module
**  $Id: tshm.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSHM_H
#define TSHM_H

/*********************************************
#FUNCTION:	This head file definite the general shared memory operation.
#AUTHOR:	Niezhiming
#DATE:		12-11-2002
**********************************************/

#if defined(_WIN32) || defined(_WIN64)
#else
	#include <sys/ipc.h>
	#include <sys/shm.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* PS Method Start */

#if defined(_WIN32) || defined(_WIN64)

	/* Permission flag for shmget.  */
	#define SHM_R		0400		/* or S_IRUGO from <linux/stat.h> */
	#define SHM_W		0200		/* or S_IWUGO from <linux/stat.h> */

	/* Flags for `shmat'.  */
	#define SHM_RDONLY	010000		/* attach read-only else read-write */
	#define SHM_RND		020000		/* round attach address to SHMLBA */
	#define SHM_REMAP	040000		/* take-over region on attach */

	/* Commands for `shmctl'.  */
	#define SHM_LOCK	11		/* lock segment (root only) */
	#define SHM_UNLOCK	12		/* unlock segment (root only) */

	/* Segment low boundary address multiple.  */
	#define SHMLBA			(tos_getpagesize ())


	/* Type to count number of attaches.  */
	typedef unsigned long int shmatt_t;

	/* Data structure describing a set of semaphores.  */
	struct shmid_ds
  	{
		struct ipc_perm shm_perm;		/* operation permission struct */
		size_t shm_segsz;			    /* size of segment in bytes */
		time_t shm_atime;			    /* time of last shmat() */
		unsigned long int __unused1;
		time_t shm_dtime;			    /* time of last shmdt() */
		unsigned long int __unused2;
		time_t shm_ctime;			    /* time of last change by shmctl() */
		unsigned long int __unused3;
		pid_t shm_cpid;			        /* pid of creator */
		pid_t shm_lpid;			        /* pid of last shmop */
		shmatt_t shm_nattch;		    /* number of current attaches */
		unsigned long int __unused4;
		unsigned long int __unused5;
	};

	/* ipcs ctl commands */
	#define SHM_STAT 	13
	#define SHM_INFO 	14

	/* shm_mode upper byte flags */
	#define SHM_DEST	01000	/* segment will be destroyed on last detach */
	#define SHM_LOCKED	02000   /* segment will not be swapped */
	#define SHM_HUGETLB	04000	/* segment is mapped via hugetlb */

	struct	shminfo
  	{
		unsigned long int shmmax;
		unsigned long int shmmin;
		unsigned long int shmmni;
		unsigned long int shmseg;
		unsigned long int shmall;
		unsigned long int __unused1;
		unsigned long int __unused2;
		unsigned long int __unused3;
		unsigned long int __unused4;
	};

	struct shm_info
  	{
		int used_ids;
		unsigned long int shm_tot;	/* total allocated shm */
		unsigned long int shm_rss;	/* total resident shm */
		unsigned long int shm_swp;	/* total swapped shm */
		unsigned long int swap_attempts;
		unsigned long int swap_successes;
	};

    int shmctl(HANDLE shmid, int cmd, struct shmid_ds *buf);
    HANDLE shmget(key_t key, int size, int shmflg);
    void* shmat(HANDLE shmid, const void* shmaddr, int shmflg);
	int shmdt(const void* shmaddr);

#else
#endif

/* PS Method End. */

/* PA Method Start */

#define TSHM_MAX_PREFIX			128
#define TSHM_MAX_KEY			64

#define TSHM_DFT_ACCESS			0664
#define TSHM_MODE_MASK			0xffff

#define TSHMF_CREATE			0x10000
#define TSHMF_DELETE			TSHMF_CREATE
#define TSHMF_EXCL				0x20000
#define TSHMF_PRIVATE			0x40000


#define TSHMF_IDENT_CHAR		'z'

#define TSHMF_RDONLY			0x01

#define TSHM_PATTERN			"tshm:0x%0x:"

/** *******************************************************************
*	@brief delete a shm object.
*	@param[in] a_pszPath the path name of the shm object.
*	@param[in] a_iType the type of the shm object, it is just for compatible with ftok().
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int tshmdelete(IN const char* a_pszPath, IN int a_iType);	/* only useful to unix.*/

/** *******************************************************************
*	@brief open/create a shm object.
*	@param[in] a_pszPath the path name of the shm object.
*	@param[in] a_iSize the size of the shm object needed.
*	@param[in] a_iFlags some flags when open/create the shm object, the follow flags can be used bitwise combined: TSHMF_CREATE, TSHMF_EXCL, TSHMF_PRIVATE.
*	@param[in] a_iType the type of the shm object, it is just for compatible with ftok().
*	@return the descriptor of the shm object.
*	@retval -1 for failure.
*	@retval >=0 the descriptor of the returned shm object.
*********************************************************************/
HANDLE tshmopen(IN const char* a_pszPath, IN int a_iSize, IN int a_iFlags, IN int a_iType);

/** *******************************************************************
*	@brief close the shm object.
*	@param[in] a_iShm the descriptor of the shm object.
*	@param[in] a_iFlags only one flags can be specified: TSHMF_DELETE, delete the shm object after closing it. otherwise just close it.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int tshmclose(IN HANDLE a_iShm, IN int a_iFlags);

/** *******************************************************************
*	@brief attach the shm object into caller process's address space.
*	@param[in] a_iShm the descriptor of the shm object.
*	@param[in] a_iFlags specified the flags when attach, TSHMF_RDONLY if the address space only allow read.
*	@return the base address.
*	@retval -1 for failure.
*	@retval <>-1 the base address of the space
*********************************************************************/
void* tshmat(IN HANDLE a_iShm, IN int a_iFlags);

/** *******************************************************************
*	@brief detach the shm object, release the related address space.
*	@param[in] a_pvBase the base address which the shm object attach to.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int tshmdt(void* a_pvBase);

/* PA Method End. */

#ifdef __cplusplus
}
#endif


#endif /* TSHM_H */

