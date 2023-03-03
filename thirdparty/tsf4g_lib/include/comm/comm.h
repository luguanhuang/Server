#ifndef __TSF4G_COMM_H_
#define __TSF4G_COMM_H_

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
#pragma comment(lib, "libcomm_d.lib")
#else
#pragma comment(lib, "libcomm.lib")
#endif
#endif
#endif

#include "comm/shtable.h"
#include "comm/shtable_shm.h"
#include "comm/tmempool.h"
#include "comm/tmempool_shm.h"
#include "comm/tconvert.h"
#include "comm/tlist.h"

#endif

