/*
**  @file $RCSfile: tlog_loc_info.h,v $
**  general description of this module
**  $Id: tlog_loc_info.h,v 1.2 2008-08-06 05:57:25 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008-08-06 05:57:25 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_LOC_INFO_H
#define TLOG_LOC_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    const char* loc_file;
    int loc_line;
    const char* loc_function;
    void* loc_data;
} tlog_loc_info_t;

typedef tlog_loc_info_t		TLOGLOCINFO;
typedef tlog_loc_info_t		*LPTLOGLOCINFO;

#ifdef __GNUC__
#   define TLOG_LOC_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, __FUNCTION__, user_data }
#elif defined(__FUNCTION__)
#   define TLOG_LOC_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, __FUNCTION__, user_data }
#else
#   define TLOG_LOC_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, "(nil)", user_data }
#endif

#define __tlog_str(n) #n

#ifdef __GNUC__
#   define __tlog_loc(n)	__FUNCTION__ "() at " __FILE__ ":" __tlog_str(n)
#elif defined(__FUNCTION__)
#   define __tlog_loc(n)	__FUNCTION__ "() at " __FILE__ ":" __tlog_str(n)
#else
#   define __tlog_loc(n)	__FILE__ ":" __tlog_str(n)
#endif

#define tlog_loc 		__tlog_loc(__LINE__)

#ifdef __cplusplus
}
#endif

#endif /* TLOG_LOC_INFO_H */


