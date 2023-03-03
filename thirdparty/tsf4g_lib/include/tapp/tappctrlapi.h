#ifndef __TAPP_CI_CONSOLE_INTERFACE__
#define __TAPP_CI_CONSOLE_INTERFACE__
#include <ctype.h>
#include "tlog/tlog.h"

#ifdef __cpluscplus
extern "C" {
#endif
    typedef struct CIPlugin
    {
        TLOGCATEGORYINST* pstLogCat;
        int (*tappctrl_send)(const char* pszMetaName, const char* pSendBuf, int iSendLen);
    } CIPlugin;

    int     InitPlugin(CIPlugin* pstCIPlugin);
    int     InputProcess(unsigned short argc, const char** argv);
    int     BinMsgProcess(const char* pszMetaName, const char* pBuf, int iBufLen);
    void    FreePlugin();

#ifdef __cpluscplus
extern "C" }
#endif

#endif
