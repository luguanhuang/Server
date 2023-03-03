/**
*
* @file     tdr_external.h
* @brief    输出API定义
*
* @author steve jackyai
* @version 1.0
* @date 2008-01-07
*
*
* Copyright (c)  2007-2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef  TDR_EXTERNAL_H
#define TDR_EXTERNAL_H


#ifdef TAPI_EXTERNAL_H
	#define TDR_API TSF4G_API
#else
	#if (defined(_WIN32) || defined(_WIN64)) && !defined(TSF4G_STATIC)
		#ifdef TSF4G_DLL_EXPORTS
			#define TDR_API __declspec(dllexport)
		#elif  defined(TSF4G_DLL_IMPORTS)
			#define TDR_API extern __declspec(dllimport)
		#else
			#define TDR_API
		#endif /*TSF4G_DLL_EXPORTS*/
	#else
		#define TDR_API
	#endif
#endif /*TDR_EXTERNAL_H*/


/* automatically include the correct library on windows */
#if defined(_WIN32) || defined(_WIN64)


#ifdef TDR_WITH_XML

#ifdef _DEBUG
#pragma comment( lib, "libscew_d.lib" )
#pragma comment(lib, "libexpat_d.lib")
#else
#pragma comment( lib, "libscew.lib" )
#pragma comment(lib, "libexpat.lib")
#endif

#endif /*TDR_WITH_XML*/


#endif



#endif /*TDR_EXTERNAL_H*/


