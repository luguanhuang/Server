/**
*
* @file     tdr.h
* @brief    TDR主头文件
*
* @author steve jackyai
* @version 1.0
* @date 2007-03-26
*
*
* Copyright (c)  2007-2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TDR_H
#define TDR_H

#if defined(_WIN32) || defined(_WIN64)

#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif

/*缺省使用libtdr_xml库*/
#ifdef TDR_WITHOUT_XML
    #undef TDR_WITH_XML
#else
    #ifndef TDR_WITH_XML
        #define TDR_WITH_XML
    #endif
#endif

#ifdef _TSF4G_LARGE_LIB_

#ifdef _DEBUG
	#pragma comment(lib, "libtsf4g_d.lib")
#else
	#pragma comment(lib, "libtsf4g.lib")
#endif

#else

#ifdef _DEBUG
    #ifdef TDR_WITH_DBMS
        #pragma comment(lib, "libtdr_dbms_xml_d.lib")
    #elif defined(TDR_WITH_XML)
        #pragma comment(lib, "libtdr_xml_d.lib")
    #else
        #pragma comment(lib, "libtdr_d.lib")
    #endif
#else
    #ifdef TDR_WITH_DBMS
        #pragma comment(lib, "libtdr_dbms_xml.lib")
    #elif defined(TDR_WITH_XML)
        #pragma comment(lib, "libtdr_xml.lib")
    #else
        #pragma comment(lib, "libtdr.lib")
    #endif
#endif

#endif

#endif


#include "tdr/tdr_external.h"
#include "tdr/tdr_types.h"
#include "tdr/tdr_ctypes_info.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_error.h"
#include "tdr/tdr_metalib_init.h"
#include "tdr/tdr_XMLtags.h"
#include "tdr/tdr_XMLMetaLib.h"
#include "tdr/tdr_metalib_to_hpp.h"
#include "tdr/tdr_data_io.h"
#include "tdr/tdr_data_sort.h"
#include "tdr/tdr_net.h"
#include "tdr/tdr_operate_data.h"
#include "tdr/tdr_metalib_manage.h"
#include "tdr/tdr_sql.h"
#include "tdr/tdr_meta_entries_index.h"
#include "tdr/tdr_net_tlv.h"

#endif /* TDR_H */
