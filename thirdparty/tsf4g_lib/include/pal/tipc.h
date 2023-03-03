/*
**  @file $RCSfile: tipc.h,v $
**  general description of this module
**  $Id: tipc.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TIPC_H
#define TIPC_H

#include "pal/ttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)

/* Mode bits for `msgget', `semget', and `shmget'.  */
#define IPC_CREAT	01000		/* Create key if key does not exist. */
#define IPC_EXCL	02000		/* Fail if key exists.  */
#define IPC_NOWAIT	04000		/* Return error on wait.  */

/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#define IPC_RMID	0		/* Remove identifier.  */
#define IPC_SET		1		/* Set `ipc_perm' options.  */
#define IPC_STAT	2		/* Get `ipc_perm' options.  */
#ifdef __USE_GNU
# define IPC_INFO	3		/* See ipcs.  */
#endif

/* Special key values.  */
#define IPC_PRIVATE	((key_t) 0)	/* Private key.  */


/* Data structure used to pass permission information to IPC operations.  */
struct ipc_perm
{
	key_t key;  	           /* Key supplied to semget() */
	uid_t uid;                 /* Effective UID of owner */
	gid_t gid;     	           /* Effective GID of owner */
	uid_t cuid;                /* Effective UID of creator */
	gid_t cgid;                /* Effective GID of creator */
	unsigned short mode;       /* Permissions */
	unsigned short seq;        /* Sequence number */
};

key_t ftok(const char *pathname, int proj_id);

#endif


#define TIPC_MAP_CHAR_FROM		'\\'
#define TIPC_MAP_CHAR_TO		'_'

#define TIPC_MAX_NAME			4096
#define TIPC_MAX_PATH			4096

#define TIPC_PATTERN			"ipc_key_0x%0x"

/** *******************************************************************
*	@brief create a unique name by combine path and prefix.
*	@param[out] a_pszBuff the buffer for hold the name.
*	@param[in] a_iBuff the size of the buffer a_pszBuff.
*	@param[in] a_pszPath the path, which is a string.
*	@param[in] a_pszPrefix the prefix, which is a string.
*	@return the length of the name returned in a_pszBuff.
*	@retval -1 for failure, the buffer is not enough.
*	@retval >=0 the length of the name returned.
*********************************************************************/
int tipc_path2name(char* a_pszBuff, int a_iBuff, const char* a_pszPath, const char* a_pszPrefix);

/** *******************************************************************
*	@brief  create a unique name by a key.
*	@param[out] a_pszBuff the buffer for hold the name.
*	@param[in] a_iBuff the size of the buffer a_pszBuff.
*	@param[in] a_iKey the key, it is a integer.
*	@return the length of the name returned in a_pszBuff.
*	@retval -1 for failure, the buffer is not enough.
*	@retval >=0 the length of the name returned.
*********************************************************************/
int tipc_key2name(char* a_pszBuff, int a_iBuff, key_t a_iKey);

#ifdef __cplusplus
}
#endif

#endif /* TIPC_H */

