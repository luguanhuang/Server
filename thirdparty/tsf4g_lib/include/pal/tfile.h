/*
**  @file $RCSfile: tfile.h,v $
**  general description of this module
**  $Id: tfile.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TFILE_H /* general file operation. */
#define TFILE_H

/*********************************************
#FUNCTION:	This head file definite the general file operation.
#AUTHOR:	Niezhiming
#DATE:		15-5-2002
**********************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pal/trules.h>

#if defined(_WIN32) || defined(_WIN64)
	#include <io.h>
#else
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)

	/* Operations for the `flock' call.  */
	#define	LOCK_SH	1	/* Shared lock.  */
	#define	LOCK_EX	2 	/* Exclusive lock.  */
	#define	LOCK_UN	8	/* Unlock.  */

	/* Can be OR'd in to one of the above.  */
	#define	LOCK_NB	4	/* Don't block when locking.  */

	int flock(int fd, int operation);

#else
#endif

#define TF_MODE_READ			0x01
#define TF_MODE_WRITE			0x02

#define TF_MODE_CREATE			0x10
#define TF_MODE_TRUNCATE		0x20
#define TF_MODE_EXCL			0x40

#define TF_MAX_NAME				4096
#define TF_MAX_PATH				4096

#if defined(_WIN32) || defined(_WIN64)
typedef struct _stat			TFSTAT;
typedef struct _stat			*PTFSTAT;
#else
typedef struct stat				TFSTAT;
typedef struct stat				*PTFSTAT;
#endif


/** ******************************************************************
  @brief  open a file, return a os level file handle.
  @param[in] a_pszPath the path of the file to open.
  @param[in] a_iMode the flags used to open the file:
    TF_MODE_READ: open the file for read.
    TF_MODE_WRITE: open the file for write.
    TF_MODE_CREATE: if the specified file do not exist, create it.
    TF_MODE_TRUNCATE: truncate the file to zero size when opened.
    TF_MODE_EXCL: if specify TF_MODE_CREATE and the file already exist, this function will fail.

  @return the handle behave for the file.
  @retval -1 for failure;
  @retval >=0 for success;
*/
HANDLE tfopen(IN const char* a_pszPath, IN int a_iMode);



/** *******************************************************************
*	@brief  close a opened file handle.
*	@param[in] a_iFile the file handle to be closed.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detailed reason.
*	@retval 0 for success.
*********************************************************************/
int tfclose(IN HANDLE a_iFile);



/** *******************************************************************
*	@brief delete a file from the filesystem.
*	@param[in] a_pszPath the path of the file to be deleted.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success
*********************************************************************/
int tfdelete(IN const char* a_pszPath);



/** *******************************************************************
*	@brief write data to specified file.
*	@param[in] a_iFile  the file to be writren.
*	@param[in] a_pszBuff the buffer for holding data.
*	@param[in] a_iSize the length of the data in buffer.
*	@return the bytes written.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 for success, the bytes written.
*********************************************************************/
int tfwrite(IN HANDLE a_iFile, IN const char* a_pszBuff, IN int a_iSize);



/** *******************************************************************
*	@brief read data from the specified file.
*	@param[in] a_iFile the file handle for read.
*	@param[out] a_pszBuff the buffer used to hold data read.
*	@param[in] a_iSize the size of the buffer.
*	@return the bytes read.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 for success
*********************************************************************/
int tfread(IN HANDLE a_iFile, OUT char* a_pszBuff, IN int a_iSize);



/** *******************************************************************
*	@brief load a file's content to a buffer, the buffer is allocated by this function using "malloc", caller should call "free" to free this buffer is no used.
*	@param[in] a_iFile the file handle.
*	@param[in,out] a_ppszBuff  the variable for hold the address of the buffer.
*	@param[in,out] a_piSize the size of the allocated buffer.
*	@return whether this operation is succeeded.
*	@retval  -1 for failure.
*	@retval  0 for success.
*********************************************************************/
int tfload(IN HANDLE a_iFile, INOUT char** a_ppszBuff, INOUT int* a_piSize);



/** *******************************************************************
*	@brief same to tfload, but use path name to identify the file.
*	@param[in] a_pszPath the path of the file.
*	@param[in,out] a_ppszBuff  the variable for hold the address of the buffer.
*	@param[in,out] a_piSize the size of the allocated buffer.
*	@return whether this operation is succeeded.
*	@retval  -1 for failure.
*	@retval  0 for success.
*********************************************************************/
int tflload(IN const char* a_pszPath, INOUT char** a_ppszBuff, INOUT int* a_piSize);



/** *******************************************************************
*	@brief synchronize file content, flush it to disk.
*	@param[in] a_iFile handle of the file to synchronize.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tfsync(IN HANDLE a_iFile);




/** *******************************************************************
*	@brief lock the file in range
*	@param[in] a_iFile handle of the file to lock.
*	@param[in] a_iOff the start position of the range to lock.
*	@param[in] a_iLen the length from the start position to lock.
*	@param[in] a_iIsBlocking whether this operation should block if can not be finished immediately.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tflock(IN HANDLE a_iFile, IN int a_iOff, IN int a_iLen, IN int a_iIsBlocking);



/** *******************************************************************
*	@brief unlock the file in range.
*	@param[in] a_iFile handle of the file to unlock.
*	@param[in] a_iOff the start position of the range to unlock.
*	@param[in] a_iLen the length from the start position to unlock.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tfunlock(IN HANDLE a_iFile, IN int a_iOff, IN int a_iLen);



/** *******************************************************************
*	@brief  reposition the file pointer.
*	@param[in] a_iFile the handle of the file to reposition.
*	@param[in] a_iOff the offset which is relatively to the value of a_iOrigin.
*	@param[in] a_iOrigin  the method to reposition:
*			SEEK_SET: use a_iOff as absolute position.
*			SEEK_CUR: use a_iOff as relative position from currrent.
*			SEEK_END: use a_iOff as relative position from the file end.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval > for success,returns the offset.
*********************************************************************/
int tfseek(IN HANDLE a_iFile, IN int a_iOff, IN int a_iOrigin);



/** *******************************************************************
*	@brief  get the size of the file.
*	@param[in] a_iFile handle of the file.
*	@return the size of the file.
*	@retval -1 for failure.
*	@retval >=0 for success, the size of the file.
*********************************************************************/
int tfsize(IN HANDLE a_iFile);



/** *******************************************************************
*	@brief truncate file.
*	@param[in] a_iFile the handle of the file.
*	@param[in] a_iSize the size of the file after beging truncated.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tftruncate(IN HANDLE a_iFile, IN int a_iSize);



/** *******************************************************************
*	@brief check whether the file named a_pszPath exist.
*	@param[in] a_pszPath the name of the file to check.
*	@return whether the file exist.
*	@retval -1 non-existence.
*	@retval 0 for existence.
*********************************************************************/
int tfexist(IN const char* a_pszPath);



/** *******************************************************************
*	@brief get the size information of the file.
*	@param[in] a_pszPath the path of the file to operate.
*	@return the size of the file.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 for success, the size of the file.
*********************************************************************/
int tflsize(IN const char* a_pszPath);



/** *******************************************************************
*	@brief create directory.
*	@param[in] a_pszPath the path of the directory to create.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tfmkdir(IN const char* a_pszPath);



/** *******************************************************************
*	@brief copy file.
*	@param[in] a_pszDst path of the target file which is copying to.
*	@param[in] a_pszSrc path of the source file to be copied.
*	@param[in] a_iIsReplaceExist  whether replace the target file if the target file already exist.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tfcopy(IN const char* a_pszDst, IN const char* a_pszSrc, IN int a_iIsReplaceExist);



/** *******************************************************************
*	@brief retrieve information of the file.
*	@param[in] a_pszPath the path of the file to retieve information.
*	@param[out] a_pstStat the buffer to hold the information.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tflstat(IN const char* a_pszPath, OUT TFSTAT* a_pstStat);



/** *******************************************************************
*	@brief same as tflstat, but use handle to identify the file.
*	@param[in] a_iFile the handle of the file to retieve information.
*	@param[out] a_pstStat the buffer to hold the information.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
int tfstat(IN HANDLE a_iFile, OUT TFSTAT* a_pstStat);


#ifdef __cplusplus
}
#endif

#endif	/* TFILE_H */


