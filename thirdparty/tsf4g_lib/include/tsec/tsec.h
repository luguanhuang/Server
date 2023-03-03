/** @file $RCSfile: tsec.h,v $
  header file for tsf4g-security module
  $Id: tsec.h,v 1.3 2009-10-13 08:11:46 flyma Exp $
@author $Author: flyma $
@date $Date: 2009-10-13 08:11:46 $
@version 1.0
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/

#ifndef _TSEC_H
#define _TSEC_H
#include "md5.h"
#include "oi_tea.h"
#include "taes.h"

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
	#pragma comment(lib, "libtsec_d.lib")
#else
	#pragma comment(lib, "libtsec.lib")
#endif
#endif

#endif

#endif /**< _TSEC_H */

