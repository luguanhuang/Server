/****************************************************************************
*
*	This file used to definition the error-code returned.
*
*	If you do not want to print the meaning of the error, then just
*
*	include this file. 
*	
*	If you do print the error, then you need the expat and scew lib.
*
*	Because the description of the error code use XML file.
*
*	steve, 2003-12-22, reserved.
*
*****************************************************************************/

#ifndef TERR_H
#define TERR_H

#include <stdio.h>

/* a errcode is consist by three part:
* error-level: 1byte.
* mod_id: 1byte.
* errno: 2byte. 0-1023 is used by system for general error.
* 0 means no error.
* <0 means error.
* >0 means success, but something happened.
*/

typedef int HERR;

#ifdef _REENTRANT
#define  terrno						*terr_get_location()
#else
extern HERR g_terrno;
#define  terrno						g_terrno
#endif

#define TERR_LEVEL_MIN				0x00
#define TERR_LEVEL_DEBUG			0x01
#define TERR_LEVEL_WARN				0x02
#define TERR_LEVEL_ERROR			0x03
#define TERR_LEVEL_SEVERE			0x04
#define TERR_LEVEL_MAX				TERR_LEVEL_SEVERE

#define TERR_NOERR					0
#define TERR_ERROR					-1

/* the minimum available user error number */
#define TERR_MAX_ERRNO				1024
#define TERR_SYS_ERRNO				TERR_MAX_ERRNO

#define TERR_MAX_STR				4096

#define TERR_MAKE(level, mod, errno)	(0x80000000 | (level<<24) | (mod<<16) | (errno)) 

#define TERR_MAKE_DEBUG(mod, errno)		TERR_MAKE(TERR_LEVEL_DEBUG, mod, errno)
#define TERR_MAKE_WARN(mod, errno)		TERR_MAKE(TERR_LEVEL_WARN, mod, errno)
#define TERR_MAKE_ERROR(mod, errno)		TERR_MAKE(TERR_LEVEL_ERROR, mod, errno)
#define TERR_MAKE_SEVERE(mod, errno)	TERR_MAKE(TERR_LEVEL_SEVERE, mod, errno)

#define TERR_MAKE_CODE(mod, code)		( (mod)<<16 | (code) )

#define TERR_GET_MOD(error)				( ( error & 0x00ff0000 ) >> 16 )
#define TERR_GET_CODE(error)			( error & 0xffff )
#define TERR_GET_ERRNO(error)			( error & 0xffff )
#define TERR_GET_LEVEL(error)			( (error & 0x0f000000) >> 24 )

#define TERR_IS_ERROR(error)			( error & 0x80000000 )


#define TERR_NOMEM					0x01
#define TERR_BADARG					0x02
#define TERR_BADHANDLE				0x03
#define TERR_BUFF					0x04
#define TERR_SEARCH					0x05 
#define TERR_REPEAT					0x06 
#define TERR_NONEXIST				0x07 


#ifdef __cplusplus
extern "C"
{
#endif

HERR terr_init(const char* pszPath, const char* pszLang, int iFlags);
void terr_release(void);

HERR terr_set_time_format(const char* pszFormat);
HERR terr_set_output_format(const char* pszFormat);

HERR terr_tostr(HERR hErr, char* pszErr, int* piLen);
HERR terr_error(int iFile, HERR hErr);
HERR terr_ferror(FILE* fp, HERR hErr);
void terr_perror(HERR hErr);

const char* terr_get(HERR hErr);

HERR* terr_get_location(void);

#ifdef __cplusplus
}
#endif

#endif /* TERR_H */
