/*
**  @file $RCSfile: texec.h,v $
**  general description of this module
**  $Id: texec.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TEXEC_H

#define TEXEC_H

#include <stdarg.h>

/* PA Method Start. */

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define TEXEC_ARG_MAX		4096
#else
	#define TEXEC_ARG_MAX		4096
#endif


/* the texecxx function family is used to create a process use the specified arguments.
   It combine fork + execxx in linux or other unix-like os.
   It just create a new process in windows-like os.
*/


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszPath.
*	@param[in]	a_pszPath the image file.
*	@param[in]	a_pszArg the argument list for the new process, the last argument must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texecl(IN const char *a_pszPath, IN const char *a_pszArg, ...);


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszFile, using path search.
*	@param[in]	a_pszFile the image file.
*	@param[in]	a_pszArg the argument list for the new process, the last argument must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texeclp(IN const char *a_pszFile, IN const char *a_pszArg, ...);


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszPath, using the specifed environment block.
*	@param[in]	a_pszPath the image file.
*	@param[in]	a_pszArg the argument list for the new process, the argument before last must be NULL.
*			the last argument is a string array pointer. the last element of the string array must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texecle(IN const char *a_pszPath,  IN const char *a_pszArg, ...);


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszPath.
*	@param[in]	a_pszPath the image file.
*	@param[in]	a_apszArgv the argument array for the new process, the last element of the array must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texecv(IN const char *a_pszPath, IN const char *a_apszArgv[]);


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszFile, using path search.
*	@param[in]	a_pszFile the image file.
*	@param[in]	a_apszArgv the argument array for the new process, the last element of the array must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texecvp(IN const char *a_pszFile, IN const char *a_apszArgv[]);


/** *******************************************************************
*	@brief  create a new process whose image file is a_pszPath, using the specifed environment block.
*	@param[in]	a_pszPath the image file.
*	@param[in]	a_apszArgv the argument array for the new process, the last element of the array must be NULL.
*	@param[in]	a_apszEnvp the environment string array for the new process, the last element of the array must be NULL.
*	@return whether the operation is succeeded.
*	@retval  -1 for failure, use errno to determine the reason.
*	@retval  >0 for the id of the new process.
*********************************************************************/
int texecve(IN const char *a_pszPath, IN const char *a_apszArgv[], IN const char *a_apszEnvp[]);


#ifdef __cplusplus
}
#endif

/* PA Method End. */

#endif /* TEXEC_H */


