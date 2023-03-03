/*
**  @file $RCSfile: pal.h,v $
**  general description of this module
**  $Id: pal.h,v 1.3 2009-05-27 03:02:30 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-05-27 03:02:30 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef PAL_H
#define PAL_H

/**
 * 重要说明：由于windows平台部分pal接口未严格测试，强烈建议项目组
 * 慎用windows pal，如有问题，也可以联系TSF4G开发人员
 * */

/* automatically include the correct library on windows */
#if defined(_WIN32) || defined(_WIN64)

#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif


#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
	#pragma comment(lib, "libtsf4g_d.lib")
#else
	#pragma comment(lib, "libtsf4g.lib")
#endif
#else
#ifdef _DEBUG
	#pragma comment(lib, "libpal_d.lib")
#else
	#pragma comment(lib, "libpal.lib")
#endif
#endif

#endif

#if !defined(_WIN32) && !defined(_WIN64)
#ifdef _DEBUG
	#define dbgprintf(fmt,args...) printf(__FILE__":%d "fmt"\n",__LINE__,##args)
#else
	#define dbgprintf(fmt,args...)
#endif
#else
	#define dbgprintf(fmt)
#endif
#include "pal/tapi_external.h"
#include "pal/tos.h"
#include "pal/ttypes.h"
#include "pal/tstdio.h"
#include "pal/tstdlib.h"
#include "pal/tstring.h"
#include "pal/trules.h"
#include "pal/terrno.h"

#include <stddef.h>
#include <assert.h>
#include <stdarg.h>

#include "pal/tuio.h"
#include "pal/tdlfcn.h"
#include "pal/getopt.h"
#include "pal/ttime.h"
#include "pal/tfile.h"
#include "pal/tlock.h"
#include "pal/tmutex.h"
#include "pal/tipc.h"
#include "pal/tshm.h"
#include "pal/tsem.h"
#include "pal/tmsgque.h"
#include "pal/tmmap.h"
#include "pal/tsocket.h"
#include "pal/tinet.h"
#include "pal/tnet.h"
#include "pal/tpoll.h"
#include "pal/tfork.h"
#include "pal/tthread.h"
#include "pal/tdirent.h"
#include "pal/tsemaphore.h"
#include "pal/tsignal.h"
#include "pal/texec.h"
#include "pal/twait.h"
#include "pal/pcreposix.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include "limits.h"
#endif

#endif /* PAL_H */
