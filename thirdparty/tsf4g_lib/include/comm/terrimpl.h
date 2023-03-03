/*****************************************************************************
*
*	This file is the implementation of the error description.
*
*	"ov_err.h" use this APIs to get the description of error.
*
*	steve, 2003-12-22, reserved.
*
*****************************************************************************/

/* the format args sequence: pid, timestr, modstr, errstr. */

#ifndef TERRIMPL_H
#define TERRIMPL_H

#include <time.h>
#include "scew/scew.h"

#define TERRIMPL_MAX_SETS			256
#define TERRIMPL_MAX_FORMAT			512
#define TERRIMPL_MAX_NAME			32

#define TERRIMPL_FLAGS_USE_MSECS	0x01

#define TERRIMPL_MSECS_FORMAT		"%08x"
#define TERRIMPL_DEF_TIME_FORMAT	"%02d-%02d-%02d %02d:%02d:%02d"
#define TERRIMPL_DEF_OUTPUT_FORMAT	"[%d][%s]:%s:\t%s"

#define TERRIMPL_STR_UNKNOWN		"unknown"

#define TERRIMPL_MAJOR				0x01

#define TERRIMPL_MOD				0x00

#define TERRIMPLE_NOERR				TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_NOERR) 
#define TERRIMPLE_NOMEM				TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_NOMEM) 
#define TERRIMPLE_BADARG			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_BADARG) 
#define TERRIMPLE_BUFF				TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_BUFF) 
#define TERRIMPLE_REPEAT			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_REPEAT) 
#define TERRIMPLE_SEARCH			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_SEARCH) 
#define TERRIMPLE_NONEXIST			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_NONEXIST) 
#define TERRIMPLE_BADROOT			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+1) 
#define TERRIMPLE_BADVER			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+2) 
#define TERRIMPLE_BADMOD			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+3) 
#define TERRIMPLE_PARSER			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+4) 
#define TERRIMPLE_LOADXML			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+5) 
#define TERRIMPLE_NEEDID			TERR_MAKE_ERROR(TERRIMPL_MOD, TERR_MAX_ERRNO+6) 

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTErrDesc
{
	int iId;
	char* pszDesc;
};

typedef struct tagTErrDesc		TERRDESC;

struct tagTErrSet
{
	char* pszName;
	int iSet;
	int iMaxErrs;
	int iErrs;
	TERRDESC* pstErrors;
};

typedef struct tagTErrSet		TERRSET;

struct tagTErrLib
{
	int fInited;
	int iSets;
	int iFlags;
	int iPid;

	scew_tree* pstTree;			/* pointer to scew XML tree. */
	int aiRes[3];

	char szOutput[TERRIMPL_MAX_FORMAT];	/* output format string. */
	char szTime[TERRIMPL_MAX_FORMAT];	/* time format string. */
	
	TERRSET sets[TERRIMPL_MAX_SETS];
};

typedef struct tagTErrLib		TERRLIB;

struct tagTErrInfo
{
	TERRLIB* pstLib;
	TERRSET* pstSet;
	TERRDESC* pstDesc;
	int iRes;			/* reserve. */
};

typedef struct tagTErrInfo		TERRINFO;


/* alloc/free the ERRLIB structure. */
TERRLIB* terrimpl_alloc(void);
void terrimpl_free(TERRLIB* pstLib);

/* init the ERRLIB structure. */
void terrimpl_init(TERRLIB* pstLib);

/* set the formatting string/flags for printing. */
HERR terrimpl_set_output_format(TERRLIB* pstLib, const char* pszFormat);
HERR terrimpl_set_time_format(TERRLIB* pstLib, const char* pszFormat);
int terrimpl_set_flags(TERRLIB* pstLib, int iFlags);

/* get the formatting string/flags for printing. */
HERR terrimpl_get_output_format(TERRLIB* pstLib, char* pszFormat, int* piLen);
HERR terrimpl_get_time_format(TERRLIB* pstLib, char* pszFormat, int* piLen);
int terrimpl_get_flags(TERRLIB* pstLib);

/* load(use xml file/stream/buffer)/unload, make sure the pstLib is valid. */
HERR terrimpl_load_buffer(TERRLIB* pstLib, const char* pszBuff, int iBuff, const char* pszLang);
HERR terrimpl_load_fp(TERRLIB* pstLib, FILE* fp, const char* pszLang);
HERR terrimpl_load_file(TERRLIB* pstLib, const char* pszPath, const char* pszLang);
void terrimpl_unload(TERRLIB* pstLib);

/* get the error information according the error id. */
HERR terrimpl_get_info(TERRLIB* pstLib, HERR hErr, TERRINFO* pstInfo);

/* search the error description. */
const char* terrimpl_err2str(TERRLIB* pstLib, HERR hErr);

/* translate the error id to string. */
HERR terrimpl_translate(TERRLIB* pstLib, HERR hErr, char* pszBuff, int* piBuff);

/* dump the context. */
void terrimpl_dump(FILE* fp, TERRLIB* pstLib);

#ifdef __cplusplus
}
#endif

#endif /* TERRIMPL_H */
