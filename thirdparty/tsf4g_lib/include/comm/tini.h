/*
**  @file $RCSfile: tini.h,v $
**  general description of this module
**  $Id: tini.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TINI_H
#define TINI_H

#define TINI_LINELENGTH		4096
#define TINI_CHARDIVIDE		'#'
#define TINI_CHARESCAPE		'\\'
#define TINI_CHARASSIGN		'='
#define TINI_LEFTSQUOTE		'['
#define TINI_RIGHTQUOTE		']'

#define TINI_STRING			(int)1
#define TINI_INT			(int)2
#define TINI_LONG			(int)3
#define TINI_DOUBLE			(int)4
#define TINI_LINE			(int)5
#define TINI_IP				(int)6
#define TINI_ADDR			(int)7

#define TINI_SECTION		(int)50

#define TINI_INT_ARRAY		(int)102
#define TINI_IP_ARRAY		(int)106
#define TINI_ADDR_ARRAY		(int)107
#define TINI_STRING_ARRAY	(int)108
#define TINI_SECTION_ARRAY	(int)150

#define TINI_STR_LEN		(int)4096

#define TINI_MAX_ID_LEN  	32
#define TINI_MAX_SECTION_MEMBER  100

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTiniTypeDesc
{
	char szID[TINI_MAX_ID_LEN];
	int iType;
	int iOffset;
	char szInitVal[256];
	int iArrayMax;
	char chSeparator;
};

typedef struct tagTiniTypeDesc		TINITYPEDESC;
typedef struct tagTiniTypeDesc		*LPTINITYPEDESC;
typedef struct tagTiniTypeDesc		*PTINITYPEDESC;


struct tagTiniSectionDesc
{
	char szID[TINI_MAX_ID_LEN];
	int iType;
	int iOffset;
	TINITYPEDESC astMember[TINI_MAX_SECTION_MEMBER];
	int iArrayNumOffset;
	int iArrayMemberSize;
	int iArrayMax;
};

typedef struct tagTiniSectionDesc	TINISECTIONDESC;
typedef struct tagTiniSectionDesc	*LPTINISECTIONDESC;
typedef struct tagTiniSectionDesc	*PTINISECTIONDESC;

int tini_get_type(IN char* a_pszFile,  OUT void* a_pvType, IN TINITYPEDESC *a_pstTypeDesc);

int tini_get_ini(IN char* a_pszFile, OUT void* pvIni,	IN TINISECTIONDESC *a_pstSecDesc);

int tini_read_sec(IN FILE* a_fp, OUT void* a_pvSec, IN TINISECTIONDESC *a_pstSecDesc, IN  char* a_pszSecID, IN char* a_pszNext);


int tini_read(	IN  char    *a_pszFileName,
				IN  char    *a_pszGroupName,
				IN  char    *a_pszIdentName,
                OUT char    *a_pszResult,
                IN  int     a_iBufSize );

#ifdef __cplusplus
}
#endif

#endif /* TINI_H */
