
#ifndef		TSF_G_MNG_H
#define		TSF_G_MNG_H

#include    "tmng/tmetabase.h"
#include    "tmng/tmib.h"
#include    "tmng/tsm_tdr.h"


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
	#pragma comment(lib, "libtmng_d.lib")
#else
	#pragma comment(lib, "libtmng.lib")
#endif

#endif
#endif


#endif

