/*
**  @file $RCSfile: tos.h,v $
**  general description of this module
**  $Id: tos.h,v 1.7 2009-01-23 09:34:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-23 09:34:20 $
**  @version $Revision: 1.7 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TOS_H
#define TOS_H

#if defined(__GNUC__)
#define TOS_UNUSED_ARG(a) ((void)a)//do{}while(NULL == &a)
#else
#define TOS_UNUSED_ARG(a) a
#endif

#if defined(_WIN32) || defined(_WIN64)
	#if(_WIN32_WINNT >= 0x0400)
		#include <winsock2.h>
		#include <windows.h>
	#else /* _WIN32_WINNT */
		#include <winsock2.h>
		#include <windows.h>
	#endif /* _WIN32_WINNT */
	#include <io.h>
	#include <process.h>
	#include "pal/getopt.h"
#else
	#include <unistd.h>
	#include <endian.h>
	#include <libgen.h>
#endif

#include <stdlib.h>

#include "pal/ttypes.h"

#include "pal/trules.h"

#if defined(_WIN32) || defined(_WIN64)
	#if defined (LITTLEENDIAN) && LITTLEENDIAN >0
		#define TOS_LITTLEENDIAN

		#if defined (TOS_BIGENDIAN)
			#undef TOS_BIGENDIAN
		#endif
	#else
		#define TOS_BIGENDIAN
		#if defined (TOS_LITTLEENDIAN)
			#undef TOS_LITTLEENDIAN
		#endif
	#endif

#else

	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define TOS_LITTLEENDIAN

		#if defined (TOS_BIGENDIAN)
			#undef TOS_BIGENDIAN
		#endif
	#else
		#define TOS_BIGENDIAN
		#if defined (TOS_LITTLEENDIAN)
			#undef TOS_LITTLEENDIAN
		#endif
	#endif

#endif

#define WAIT_INFINITE				-1

#ifndef UNIX_PATH_MAX
	#define UNIX_PATH_MAX			108
#endif

#define TOS_DIRDOT					'.'
#define	TOS_DIRBAR					'_'

#if defined(_WIN32) || defined(_WIN64)
	#define	TOS_DIRSEP				'\\'
#else
	#define	TOS_DIRSEP				'/'
#endif


#ifdef __cplusplus
extern "C"
{
#endif

/* PS Method Start. */

#if defined(_WIN32) || defined(_WIN64)
	#include <direct.h>

	#define TOS_TMP_DIR				"C:\\Temp\\"
	#define TOS_LOG_DIR				"C:\\Log\\"

	#define STDIN_FILENO			0
	#define STDOUT_FILENO			1
	#define STDERR_FILENO			2

	#define _SC_ARG_MAX				1
	#define _SC_CHILD_MAX			2
	#define _SC_CLK_TCK				3
	#define _SC_JOB_CONTROL			4
	#define _SC_NGROUPS_MAX			5
	#define _SC_OPEN_MAX			6
	#define _SC_SAVED_IDS			7
	#define _SC_STREAM_MAX			8
	#define _SC_TZNAME_MAX			9
	#define _SC_VERSION				10

	#define _POSIX_CHOWN_RESTRICTED	0
	#undef  _POSIX_JOB_CONTROL
	#define _POSIX_NO_TRUNC			0
	#undef  _POSIX_SAVED_IDS
	#define _POSIX_VDISABLE			-1
	#define _POSIX_VERSION			199009L

	#define _PC_CHOWN_RESTRICTED	1
	#define _PC_LINK_MAX			2
	#define _PC_MAX_CANON			3
	#define _PC_MAX_INPUT			4
	#define _PC_NAME_MAX			5
	#define _PC_NO_TRUNC			6
	#define _PC_PATH_MAX			7
	#define _PC_PIPE_BUF			8
	#define _PC_VDISABLE			9

	#define _POSIX_STREAM_MAX		20	/* can be bigger in DOS */

	#define NGROUPS_MAX				0

	#define MAX_CHILDREN			64

	/* #define ARG_MAX				4096 -- depends on tb size; use sysconf */
	#define CHILD_MAX				MAX_CHILDREN
	/* #define OPEN_MAX				20 - DOS can change this */
	/* #define STREAM_MAX			20 - DOS can change this */
	#define TZNAME_MAX				3

	#define LINK_MAX				1
	#define MAX_CANON				126
	#define MAX_INPUT				126
	#define NAME_MAX				12	/* 8.3 */

	#define PIPE_BUF				512	/* but there aren't any pipes */

	#define PATH_MAX				MAX_PATH

	long sysconf(int name);
	long fpathconf(int filedes, int name);
	long pathconf(const char *path, int name);

	char* ttyname(int fd);
	int ttyname_r(int fd, char* buf, size_t buflen);

	#define mkdir(path, mode)			_mkdir(path)
	#define getpid()				(pid_t) GetCurrentProcessId()


	char *basename (const char *fname);
	char *dirname (const char *fname);

	int getpagesize(void);

	//#define usleep(us)				SleepEx(us/1000, TRUE)
	void usleep(unsigned long usec);

	//#define sleep(s)				SleepEx(s*1000, TRUE)
	unsigned int sleep(unsigned int seconds);

	pid_t getppid(void);

	uid_t getuid(void);
	uid_t geteuid(void);

	int setuid(uid_t uid);
	int seteuid(uid_t euid);

	gid_t getgid(void);
	gid_t getegid(void);

	int setgid(gid_t gid);
	int setegid(gid_t egid);

	int setreuid(uid_t ruid, uid_t euid);
	int setregid(gid_t rgid, gid_t egid);

	pid_t getpgrp(void);
	int setpgrp(void);

	pid_t getpgid(pid_t pid);
	int setpgid(pid_t pid, pid_t pgid);

	int getgroups(int size, gid_t a_list[]);
	int setgroups(size_t size, const gid_t *a_list);

	char *getlogin(void);
	int getlogin_r(char *buf, size_t bufsize);
	char *cuserid(char *string);

    pid_t tcgetpgrp(int fd);
    int tcsetpgrp(int fd, pid_t pgrp);

	pid_t setsid(void);

	unsigned int alarm(unsigned int seconds);

	#if _MSC_VER < 1300

	HANDLE WINAPI OpenThread(
							 DWORD dwDesiredAccess,
							 BOOL bInheritHandle,
							 DWORD dwThreadId
							 );

	DWORD WINAPI GetProcessId(
							  HANDLE Process
							 );
	#endif /* _MSC_VER < 1300 */

	#define TOS_MAX_CHILD	MAXIMUM_WAIT_OBJECTS

	int tos_get_pidlist_i(int a_iMax, pid_t* a_piPidList, int a_iPid, int a_iPpid);
	int tos_open_proclist_i(int a_iCount, pid_t* piPidList, HANDLE* phProcList);
	void tos_close_proclist_i(int a_iCount, HANDLE* phProcList);

	FARPROC tos_get_ntdll_proc_i(const char* pszName);
	FARPROC tos_get_kernel_proc_i(const char* pszName);
	int tos_get_process_id_i(HANDLE hProc);
	HANDLE tos_open_thread_i(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);

#ifdef WINNT
	void* tos_get_sd(void);
	void tos_free_sd(void* pvSd);
#endif

#else

	#define TOS_TMP_DIR				"/tmp/"
	#define TOS_LOG_DIR				"/var/log/"
	#define MAX_PATH				PATH_MAX

#endif

#if defined(_WIN32) || defined(_WIN64)

#define O_APPEND					_O_APPEND
#define O_BINARY					_O_BINARY
#define O_CREAT						_O_CREAT
#define O_EXCL						_O_EXCL
#define O_TRUNC						_O_TRUNC
#define O_RDONLY					_O_RDONLY
#define O_WRONLY					_O_WRONLY
#define O_RDWR						_O_RDWR

#define O_NOATIME					0
#define O_NOCTTY					0
#define O_NONBLOCK					0
#define O_NDELAY					O_NONBLOCK
#define O_DIRECT					0

#define O_SYNC						0
#define O_ASYNC						0

#ifndef S_IREAD
	#define S_IREAD					_S_IREAD
#endif /* S_IREAD */

#ifndef S_IWRITE
	#define S_IWRITE 				_S_IWRITE
#endif /* S_IWRITE */

#ifndef S_IEXEC
	#define S_IEXEC  				_S_IEXEC
#endif /* S_IEXEC */

#define S_IRUSR						S_IREAD
#define S_IWUSR						S_IWRITE
#define S_IXUSR						S_IEXEC
#define S_IRWXU						(S_IRUSR | S_IWUSR | S_IXUSR)

#define S_IRGRP						0
#define S_IWGRP						0
#define S_IXGRP						0
#define S_IRWXG						(S_IRGRP | S_IWGRP | S_IXGRP)

#define S_IROTH						0
#define S_IWOTH						0
#define S_IXOTH						0
#define S_IRWXO						(S_IROTH | S_IWOTH | S_IXOTH)

//#define alloca(size)					_alloca(size)
void *alloca(size_t size);


//#define lseek64(fd, offset, origin)	_lseek64(fd, offset, origin)
off64_t lseek64(int fd, off64_t offset, int whence);


int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);


/*
#define stat						_stat
*/

//#define pipe(fds)					_pipe(fds)
int pipe(int filedes[2]);


//#define popen(cmd, mode)			_popen(cmd, mode)
FILE *popen(const char *command, const char *type);

//#define pclose(fp)					_pclose(fp)
int pclose(FILE *stream);


/* Values for the second argument to access.
   These may be OR'd together.  */
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */

BOOL shortcut_create(const char *pszSrc, const char *pszDest);
BOOL shortcut_derefer(char *pszShortcut);
int win_deref(char *path);

int readlink(const char *path, char *buf, size_t bufsiz);
int link(const char *oldpath, const char *newpath);
int symlink(const char *oldpath, const char *newpath);
char *realpath(const char *path, char *resolved_path);

#define environ						_environ

#endif

/* PS Method End. */


/* PA Method Start. */

#define tos_swap64(x) \
     ((((x) & 0xff00000000000000ull) >> 56)                                   \
      | (((x) & 0x00ff000000000000ull) >> 40)                                 \
      | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
      | (((x) & 0x000000ff00000000ull) >> 8)                                  \
      | (((x) & 0x00000000ff000000ull) << 8)                                  \
      | (((x) & 0x0000000000ff0000ull) << 24)                                 \
      | (((x) & 0x000000000000ff00ull) << 40)                                 \
      | (((x) & 0x00000000000000ffull) << 56))

#ifdef TOS_LITTLEENDIAN
#define ntohq(x)                    tos_swap64(x)
#define htonq(x)                    tos_swap64(x)
#else
#define ntohq(x)                    (x)
#define htonq(x)                    (x)
#endif

#define NTOHNS(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)					      \
	{								      		  \
		*(unsigned short*)d	=	ntohs(*(unsigned short*)s);  \
		d	+=	sizeof(unsigned short);			  \
		s	+=	sizeof(unsigned short);			  \
	}								      		  \
}

#define HTONNS(d, s, size)			{			  \
	int i;								          \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(unsigned short*)d	=	htons(*(unsigned short*)s); \
		d	+=	sizeof(unsigned short);			  \
		s	+=	sizeof(unsigned short);		 	  \
	}								      		  \
}

#define NTOHNL(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								              \
		*(unsigned long*)d	=	ntohl(*(unsigned long*)s);  \
		d	+=	sizeof(unsigned long);			  \
		s	+=	sizeof(unsigned long);			  \
	}								      		  \
}

#define HTONNL(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(unsigned long*)d	=	htonl(*(unsigned long*)s); \
		d	+=	sizeof(unsigned long);			  \
		s	+=	sizeof(unsigned long);			  \
	}								      		  \
}

#define NTOHNQ(d, s, size)			{			  \
	int i;								      	  \
	for(i=0; i<size; i++)						  \
	{								      		  \
		*(ulonglong*)d	=	ntohq(*(ulonglong*)s);	 \
		d	+=	sizeof(ulonglong);			      \
		s	+=	sizeof(ulonglong);			      \
	}								      		  \
}

#define HTONNQ(d, s, size)			{			   \
	int i;								           \
	for(i=0; i<size; i++)						   \
	{								      		   \
		*(ulonglong*)d	=	htonq(*(ulonglong*)s); \
		d	+=	sizeof(ulonglong);			       \
		s	+=	sizeof(ulonglong);			       \
	}								      	       \
}

#define TOS_LINUX			0x01
#define TOS_WIN9X			0x100
#define TOS_WINNT			0x200

/** *******************************************************************
*	@brief  this function is used to get the type of the operation system.
*	@return	the type of the operation system.
*	@retval TOS_LINUX, It's linux OS.
*	@retval TOS_WIN9X, It's WIN9X OS.
*	@retval TOS_WINNT, It's WINNT OS.
*********************************************************************/
int tos_get_ostype(void);


/** *******************************************************************
*	@brief  this function is used to check whether the operation system
*			is linux.
*	@return whether the operation system is linux.
*	@retval 0 if it is not linux
*	@retval 1 if it is  linux
*********************************************************************/
int tos_is_linux(void);


/** *******************************************************************
*	@brief  this function is used to check whether the operation system
*			is winnt series.
*	@return whether the operation system is winnt.
*	@retval  0 if it is not windows nt/nt2000/xp/20003 etc.
*	@retval  1 if it is windows nt/nt2000/xp/2003 etc.
*********************************************************************/
int tos_is_winnt(void);



/** *******************************************************************
*	@brief  use to sleep micro seconds.
*	@param[in]  a_iUs  specify the number of micro-seconds to sleep;
*	@return whether this function failed or success.
*	@retval 0 tos_usleep finished success.
*	@retval -EINTR tos_usleep is interrupted.
*	@retval -INVAL  a_iUs use a value >=1000000(In some systems).
*********************************************************************/
int tos_usleep(IN int a_iUs);


/** *******************************************************************
*	@brief make the caller process become a daemon process.
*	@param[in] a_pszRoot the root filesystem for this process.
*	@return no return.
*********************************************************************/
void tos_make_daemon(IN const char* a_pszRoot);


/** *******************************************************************
*	@brief  set the signal handler, ignore the SIGPIPE signal.
*	@return no return.
*********************************************************************/
void tos_ignore_pipe(void);


/** *******************************************************************
*	@brief  this function is used to convert a file-name to a macro.
*			the rules is like that: abc.h->ABC_H
*	@param[out] a_pszMacro the buffer used to hold the macro, null
*			ternimated.
*	@param[in] a_iMacro the buffer size of a_pszMacro.
*	@param[in] a_pszFile the file-name which need to be converted,
*			null ternimated.
*	@return no return, if the buffer is not enough, the macro is truncated.
*********************************************************************/
void tos_file_to_macro(INOUT char* a_pszMacro, IN int a_iMacro, IN const char* a_pszFile);


/** *******************************************************************
*	@brief get the page size for allocate granity.
*	@return the size of one page.
*********************************************************************/
int tos_get_pagesize(void);


/** *******************************************************************
*	@brief  get current time, use string format.
*	@param[out] a_pszBuff the buffer for hold the time string,
*			null terminated.
*	@param[in] a_iBuff the buffer size.
*	@return the length of the time string.
*	@retval -1 if there are errors.
*	@retval >0 the length of the time string.
*********************************************************************/
int tos_time(INOUT char* a_pszBuff, IN int a_iBuff);

/** *******************************************************************
*	@brief  do some initialization job for some os.
*	@param[in]  a_pvArg not used now, must be zero.
*	@return	 whether this operation is success.
*	@retval 0 the initialization is success.
*	@retval -1 the initialization is failed.
*********************************************************************/
int tos_init(IN void* a_pvArg);


/** *******************************************************************
*	@brief  do some finitialization or clean job for some os.
*	@return whether this operation is success.
*	@retval 0 the finitialization is success.
*	@retval -1 the finitialization is failed.
*********************************************************************/
int tos_fini(void);


/** *******************************************************************
*	@brief  write the pid of the current process to the file specified.
*	@param[in]  a_pszPidFile  the path of the file to write pid.
*	@return	 whether this operation is success.
*	@retval 0 the operation is success.
*	@retval -1 the operation is failed.
*********************************************************************/
int tos_write_pid(const char* a_pszPidFile);


/** *******************************************************************
*	@brief  read the pid from the file specified.
*	@param[in]  a_pszPidFile  the path of the file to read pid.
*	@return	 whether this operation is success.
*	@retval >=0 the operation is success, the return value is a pid.
*	@retval -1 the operation is failed.
*********************************************************************/
pid_t tos_read_pid(const char* a_pszPidFile);


/** *******************************************************************
*	@brief  get the full name of the execute file  by pid.
*	@param[in]  a_pid the pid used to retrieve the execute file.
*	@param[out]  a_pszPath the buffer for storing the path.
*	@param[in]  a_iPath the length of the buffer.
*	@return	 whether this operation is success.
*	@retval 0 the operation is success.
*	@retval -1 the operation is failed.
*********************************************************************/
int tos_get_exe(pid_t a_pid, char* a_pszPath, int a_iPath);


/** *******************************************************************
*	@brief  Get current working directory.
*      This function copies an absolute pathname of the current working directory to the array pointed to by buf, which is of length size.
       If  the current absolute pathname would require a buffer longer than size elements, NULL is returned.
       If buf is NULL, NULL is returned.
*	@param[in]  a_pszBuf  Storage location for the path
*	@param[in]  a_size the Maximum length of the path in characters
*	@return	 NULL on failure with errno set accordingly, and buf on success. The contents of the array pointed to by buf is undefined on error
*********************************************************************/
char *tos_getcwd(char *a_pszBuf, size_t  a_size);


/** *******************************************************************
*	@brief  Changes the current working directory.
*	@param[in]  a_pszPath  Path of new working directory.
*	@return	  Upon successful completion, 0 shall be returned. Otherwise, -1 shall be returned, the current working directory shall remain unchanged, and  errno  shall  be  set  to
       indicate the error.

*********************************************************************/
int tos_chdir(const char *a_pszPath);


/** *******************************************************************
*	@brief  check whether the process identified by pid is a process created by pszExeFile.
*	@param[in]  a_pid the pid of the process.
*	@param[in]  a_pszExeFile the execute file.
*	@return	 whether the process identified by pid is a process created by pszExeFile.
*	@retval 0 it is.
*	@retval -1 it not.
*********************************************************************/
int tos_check_pid(pid_t a_pid, const char* a_pszExeFile);


/** *******************************************************************
*	@brief  wait the process identified by a_pid to exit.
*	@param[in]  a_pid the pid of the process.
*	@param[in]  a_iTimeout max seconds to wait.
*	@return	 whether the process identified by pid exit.
*	@retval 0 it is.
*	@retval -1 it not.
*********************************************************************/
int tos_timed_wait(pid_t a_pid, int a_iTimeout);


/** *******************************************************************
*	@brief  send a signal to the process which is created by a_pszExeFile.
*	@param[in]  a_pszPidFile the path of the file for storing pid information.
*	@param[in]  a_pszExeFile the execute file.
*	@param[in]  a_iSig the signal to be sent.
*	@param[out]  a_ppid the buffer to hold the pid of the process.
*	@return	 whether the signal is sent to the process identified by pid and created by pszExeFile .
*	@retval 0 it is.
*	@retval -1 it not.
*********************************************************************/
int tos_send_signal(const char* a_pszPidFile, const char* a_pszExeFile, int a_iSig, pid_t* a_ppid);


/** *******************************************************************
*	@brief  kill the previous instance which is created by a_pszExeFile.
*	@param[in]  a_pszPidFile the path of the file for storing pid information.
*	@param[in]  a_pszExeFile the execute file.
*	@param[in]  a_iTimeout the max seconds to wait.
*	@return	 whether the process identified by pid and created by pszExeFile is killed.
*	@retval 0 it is killed.
*	@retval -1 it not.
*********************************************************************/
int tos_kill_prev(const char* a_pszPidFile, const char* a_pszExeFile, int a_iTimeout);


/** *******************************************************************
*	@brief  make multiple level of directories.
*	@param[in]  a_pszDir the path of the directory to create, the buffer must be writable.
*	if unwritable wanted, you could use tos_mkdir
*	@return	 whether the operation is succeeded.
*	@retval 0 it is killed.
*	@retval -1 it not.
*
*	@see tos_mkdir
*********************************************************************/
int tos_mkdir_fast(char* a_pszDir);


/** *******************************************************************
*	@brief  make multiple level of directories.
*	@param[in]  a_pszDir the path of the directory to create.
*	@return	 whether the operation is succeeded.
*	@retval 0 it is killed.
*	@retval -1 it not.
*********************************************************************/
int tos_mkdir(const char* a_pszDir);


/** *******************************************************************
*	@brief  make multiple level of directories from a fullpath.
*	@param[in]  a_pszPath the fullpath which's directory need to be create, the buffer must be writable.
*	@return	 whether the operation is succeeded.
*	@retval 0 it is killed.
*	@retval -1 it not.
*********************************************************************/
int tos_mkdir_fast_by_path(char* a_pszPath);


/** *******************************************************************
*	@brief  make multiple level of directories from a fullpath.
*	@param[in]  a_pszPath the fullpath which's directory should to be create.
*	@return	 whether the operation is succeeded.
*	@retval 0 it is killed.
*	@retval -1 it not.
*********************************************************************/
int tos_mkdir_by_path(const char* a_pszPath);


/* PA Method End. */

#ifdef __cplusplus
}
#endif


#endif /* TOS_H */

