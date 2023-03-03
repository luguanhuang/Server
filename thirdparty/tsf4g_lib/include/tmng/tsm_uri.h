#ifndef _TSM_URI_H_
#define _TSM_URI_H_
//============================================================================
// @Id:       $Id: tsm_uri.h 45743 2013-03-07 07:27:25Z laynehu $
// @Author:   $Author: laynehu $
// @Date:     $Date:: 2013-03-07 15:27:25 +0800 #$
// @Revision: $Revision: 45743 $
// @HeadURL:  $HeadURL:: https://sz-svn4.tencent.com/ied/ied_tsf4g_rep/tsf4g#$
//----------------------------------------------------------------------------

/*!
 * \file tsm_uri.h
 * \brief tsm 内部 URI操纵 API
 * \version 1.0
 * \date 2010-10-11
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/types.h>

/*!
 * \fn                  void *tsm_uri_alloc(const char *a_pszUri);
 * \brief               从字符串的 uri，建立一个 uri结构
 * \param   a_pszUri    待创建的 URI字符串表示
 * \return              返回一个 URI结构，但是对用户透明，所以是 void *
 * \n
 *          失败时返回 NULL ，并且设置错误堆栈
 */
void *tsm_uri_alloc(const char *a_pszUri);

/*!
 * \fn                  const char *tsm_uri_str(void *a_pstUri);
 * \brief               从 uri结构，转换到字符串的 uri
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              返回 URI的字符串表示
 * \n
 *          a_pstUri为 NULL 时，返回 NULL
 *          其他情况，不会出错
 */
const char *tsm_uri_str(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_set_bid(void *a_pstUri,int a_iBid);
 * \brief               设置 uri结构中的 bissiness id
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 *          a_iBid      希望设置到结构中的 bissiness id
 * \return              成功返回 0
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_set_bid(void *a_pstUri,int a_iBid);

/*!
 * \fn                  int tsm_uri_std(void *a_pstUri);
 * \brief               清除 uri结构中与 meta 相关的属性，生成标准化的 URI
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功返回 0
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -3      a_pstUri中表示类型的字段非法
 */
int tsm_uri_std(void *a_pstUri);

/*!
 * \fn                  void tsm_uri_free(void *a_pstUri);
 * \brief               释放 uri结构
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              无
 */
void tsm_uri_free(void *a_pstUri);


//----------------------------------------------------------------------------
//  GET functions
//----------------------------------------------------------------------------

/*!
 * \fn                  int tsm_uri_get_type(void *a_pstUri);
 * \brief               获得 URI的类型
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功返回类型值
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_type(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_get_bid(void *a_pstUri);
 * \brief               获得 URI的 bissiness id
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 bissiness id
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_bid(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_get_size(void *a_pstUri);
 * \brief               获得 URI的尺寸
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 URI的尺寸
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_size(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_libname(void *a_pstUri);
 * \brief               获得 META URI 的库名
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回库名
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_libname(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_get_lib_ver(void *a_pstUri);
 * \brief               获得 META URI 的库版本号
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回库版本号
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_lib_ver(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_get_lib_build_ver(void *a_pstUri);
 * \brief               获得 META URI 的库构建版本号
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回库构建版本号
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_lib_build_ver(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_metaname(void *a_pstUri);
 * \brief               获得 META URI 的 META 名
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 META 名
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_metaname(void *a_pstUri);

/*!
 * \fn                  int tsm_uri_get_meta_ver(void *a_pstUri);
 * \brief               获得 META URI 的 META 版本号
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 META 版本号
 * \n
 *          失败时返回
 *          \li -1      a_pstUri为空
 *          \li -2      a_pstUri中表示类型的字段非法
 */
int tsm_uri_get_meta_ver(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_other(void *a_pstUri);
 * \brief               获得 META URI 的附加字段
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回附加字段
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_other(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_prog(void *a_pstUri);
 * \brief               获得 DATA URI 的进程名
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回进程名
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_prog(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_id(void *a_pstUri);
 * \brief               获得 DATA URI 的进程 ID
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回进程 ID
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_id(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_func(void *a_pstUri);
 * \brief               获得 DATA URI 的功能名
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回功能名
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_func(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_value(void *a_pstUri);
 * \brief               获得 DATA URI 的 value字段
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 value字段
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_value(void *a_pstUri);

/*!
 * \fn                  const char *tsm_uri_get_path(void *a_pstUri);
 * \brief               获得 DATA URI 的 path 字段
 * \param   a_pstUri    由 tsm_uri_alloc 返回的，表示 URI结构的指针
 * \return              成功时返回 path 字段
 * \n
 *          失败时返回
 *          \li NULL    a_pstUri为空 或 a_pstUri中表示类型的字段非法
 */
const char *tsm_uri_get_path(void *a_pstUri);


#ifdef __cplusplus
}
#endif


//----------------------------------------------------------------------------
// THE END
//============================================================================
#endif
