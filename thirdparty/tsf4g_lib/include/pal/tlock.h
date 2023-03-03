/*
**  @file $RCSfile: tlock.h,v $
**  general description of this module
**  $Id: tlock.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOCK_H
#define TLOCK_H

#define TLOCK_MAX_PREFIX		128
#define TLOCK_MAX_KEY			64

#define TLOCK_MAX_TRIES			100

#define TLOCK_DFT_ACCESS		0664
#define TLOCK_MODE_MASK			0xffff

#define TLOCKF_CREATE			0x10000
#define TLOCKF_DELETE			TLOCKF_CREATE
#define TLOCKF_EXCL				0x20000

#include "pal/trules.h"


#ifdef __cplusplus
extern "C"
{
#endif

/** *******************************************************************
*	@brief  open or create a lock
*	@param a_pszPath Input, the lock's pathname
*	@param a_iFlags Input, some flags. a_iFlags is consist by two part. 
*		   one is the creation flags, which can be a combination of bitwise ||:
*				TLOGF_CREATE for lock creation.
*				TLOGF_EXCL for lock exclusion, if the lock already exist, the function will fail.
*		   the other is access bits masked by TLOCK_MODE_MASK
*	@param a_iType Input, the type of the lock, it can be specifed as you willing.
*	@return whether the operation is succeeded.
*	@retval 0  if sucess.
*	@retval -1  if failed, use errno to determine the reason.
*********************************************************************/
HANDLE tlockopen(IN const char* a_pszPath, IN int a_iFlags, IN int a_iType);


/** *******************************************************************
*	@brief close the lock descriptor.
*	@param a_iLock Input, the descriptor of the lock.
*	@param a_iFlags Input, some flags:
*		  		TLOGF_DELETE for lock deletion.
*	@return whether the operation is succeeded.
*	@retval 0  if sucess.
*	@retval -1  if failed, use errno to determine the reason.
*********************************************************************/
int tlockclose(IN HANDLE a_iLock, IN int a_iFlags);


/** *******************************************************************
*	@brief  delete the lock from the system.
*	@param a_pszPath Input, the lock's pathname.
*	@param a_iType Input, the type of the lock, it can be specifed as you willing.
*	@return whether the operation is succeeded.
*	@retval 0  if sucess.
*	@retval -1  if failed, use errno to determine the reason.
*********************************************************************/
int tlockdelete(IN const char* a_pszPath, IN int a_iType);


/** *******************************************************************
*	@brief lock operations.
*	@param a_iLock Input, the descriptor of the lock.
*	@param a_iIsLocking Input, TRUE for locking operation, FALSE for unlocking operation.
*	@return whether the operation is succeeded.
*	@retval 0  if sucess.
*	@retval -1  if failed, use errno to determine the reason.
*********************************************************************/
int tlockop(IN HANDLE a_iLock, IN int a_iIsLocking);

#ifdef __cplusplus
}
#endif

#endif /* TLOCK_H */


