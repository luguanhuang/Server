/*! 
 *  \file tmetabase.h
 *  \brief 元数据库操作 API
 *  \version 1.0
 *  \date 2010-01-20
 */

#ifndef TMETABASE_H
#define TMETABASE_H

#include "pal/pal.h"
#include "pal/ttypes.h"
#include "tdr/tdr.h"

#define TMB_VERSION  0x01

#define TMB_IPC_TYPE '2'

#define TMB_DFT_KEY  30000

/*!
 * \brief 共享内存中元数据库的头信息
 * \struct TMETABASE
 */
struct tagTMetaBase
{
    int32_t iVersion;   /*!< 版本号 */
    int32_t iSize;      /*!< 最大可用共享内存尺寸 */
    int32_t iMaxLibs;   /*!< 最大能够容纳的 Lib个数 */
    int32_t iCurLibs;   /*!< 当前已经容纳的 Lib个数 */
    int32_t offs[1];    /*!< 偏移量数组 */
};

typedef struct tagTMetaBase  TMETABASE;
typedef struct tagTMetaBase *LPTMETABASE;

/*!
 * \brief 用户访问元数据库的句柄
 * \struct TMBDESC
 */
struct tagTMBDesc
{
    HANDLE       iLock;     /*!< 元数据库访问锁 */
    HANDLE       iShm;      /*!< 元数据库共享内存访问标识符 */
    LPTMETABASE  pstBase;   /*!< 元数据库指针 */
    LPTDRMETALIB pstLib;    /*!< 当前访问的 Lib指针 */
};

typedef struct tagTMBDesc  TMBDESC;
typedef struct tagTMBDesc *LPTMBDESC;
typedef struct tagTMBDesc *HTMBDESC;

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 *  \fn TSF4G_API int32_t tmb_create(OUT HTMBDESC   *a_phDesc,
 *                                   IN  const char *a_pszKey,
 *                                   IN  int32_t     a_iMaxLibs,
 *                                   IN  int32_t     a_iSize);
 *  \brief              这个函数建立一个元数据库访问句柄
 *                      如果数据库不存在，则先建立他
 *  \param a_phDesc     不能为空，调用成功时，这个指针指向的位置会填写返回句柄
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \param a_iMaxLibs   最多容纳的 Lib个数
 *  \param a_iSize      最大共享内存尺寸
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li \a a_iSize 设置过小，容纳不下 \a a_iMaxLibs 个 Lib
 *      \li 内存分配请求失败
 *      \li 锁建立失败
 *      \li 取得共享内存标识符失败
 *      \li 已有的库版本不匹配
 */
TSF4G_API int32_t tmb_create(OUT HTMBDESC  *a_phDesc,
                             IN const char *a_pszKey,
                             IN int32_t     a_iMaxLibs,
                             IN int32_t     a_iSize);

/*!
 *  \fn TSF4G_API int32_t tmb_destroy(IN const char *a_pszKey);
 *  \brief              这个函数释放 tmb_create 产生的元数据库锁与共享内存
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \return             成功返回 0; 失败返回小于零的整数;
 */
TSF4G_API int32_t tmb_destroy(IN const char *a_pszKey);

/*!
 *  \fn TSF4G_API int32_t tmb_open(OUT HTMBDESC   *a_phDesc,
 *                                 IN  const char *a_pszKey,
 *                                 IN  int32_t     a_fReadOnly);
 *  \brief              这个函数打开一个元数据库访问句柄
 *  \param a_phDesc     不能为空，调用成功时，这个指针指向的位置会填写返回句柄
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \param a_fReadOnly  如果非零，则只读模式打开，否则可写
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 内存分配请求失败
 *      \li 取得共享内存标识符失败
 *      \li 取得共享内存指针失败
 *      \li 已有的库版本不匹配
 */
TSF4G_API int32_t tmb_open(OUT HTMBDESC   *a_phDesc,
                           IN  const char *a_pszKey,
                           IN  int32_t     a_fReadOnly);

/*!
 *  \fn TSF4G_API int32_t tmb_close(INOUT HTMBDESC *a_phDesc);
 *  \brief              这个函数关闭一个已打开的元数据库访问句柄
 *  \param a_phDesc     不能为空，调用成功是这个指针指向的位置会填写 NULL
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmb_close(INOUT HTMBDESC *a_phDesc);

/*!
 *  \fn TSF4G_API int32_t tmb_lock(IN HTMBDESC a_hDesc);
 *  \brief              这个函数对一个元数据库访问句柄做锁定操作
 *  \param a_hDesc      不能为空，这个指针是元数据库的访问句柄
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 锁本身不可访问
 *      \li 锁定失败
 */
TSF4G_API int32_t tmb_lock(IN HTMBDESC a_hDesc);

/*!
 *  \fn TSF4G_API int32_t tmb_unlock(IN HTMBDESC a_hDesc);
 *  \brief              这个函数对一个元数据库访问句柄做解锁操作
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 锁本身不可访问
 *      \li 解锁失败
 */
TSF4G_API int32_t tmb_unlock(IN HTMBDESC a_hDesc);

/*!
 *  \fn TSF4G_API int32_t tmb_dump(IN HTMBDESC  a_hDesc,
 *                                 IN int32_t   a_fDetail,
 *                                 IN FILE     *a_fp);
 *  \brief              这个函数输出元数据库到文件句柄中
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_fDetail    如果这个变量非零，输出元数据库详细信息
 *                      否则只输出头信息
 *  \param a_fp         用来输出的文件指针
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmb_dump(IN HTMBDESC  a_hDesc,
                           IN int32_t   a_fDetail,
                           IN FILE     *a_fp);

/*!
 *  \fn TSF4G_API int32_t tmb_dump_files(IN HTMBDESC    a_hDesc,
 *                                       IN int32_t     a_fDetail,
 *                                       IN const char *a_szStyle,
 *                                       IN const char *a_szPath);
 *  \brief              这个函数可以把元数据库输出到各种格式到文件中
 *  \param a_hDesc      不能为空，这个指针是要访问的元数据库句柄
 *  \param a_fDetail    如果非零，产生文件并输出详细信息，否则只输出头信息
 *  \param a_szStyle    输出格式，支持 "xml", "tdr", 否则默认为 "txt"
 *  \param a_szPath     用来保存输出文件的目录名，为空的话，则输出到标准输出
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmb_dump_files(IN HTMBDESC    a_hDesc,
                                 IN int32_t     a_fDetail,
                                 IN const char *a_szStyle,
                                 IN const char *a_szPath);

/*!
 *  \fn TSF4G_API int32_t tmb_append_unlocked(INOUT HTMBDESC     a_hDesc,
 *                                            IN    LPTDRMETALIB a_pszLib);
 *  \brief              这个函数在元数据库中追加一个 tdr元数据
 *                      过程中没有执行元数据库锁定
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_pszLib     不能为空，这个指针是要追加的条目
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 元数据库已满
 *      \li 锁建立失败
 *      \li 元数据库中存在这个元数据，但是 build版本不同或者尺寸不同
 *      \li 这个 tdr文件太大，如果导入则超出了共享内存建立时设置的最大尺寸
 */
TSF4G_API int32_t tmb_append_unlocked(INOUT HTMBDESC     a_hDesc,
                                      IN    LPTDRMETALIB a_pszLib);

/*!
 *  \fn TSF4G_API int32_t tmb_append(INOUT HTMBDESC    a_hDesc,
 *                                   IN    const char *a_pszPath);
 *  \brief              这个函数在元数据库中追加一个 tdr元数据
 *                      过程锁定了元数据库
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_pszPath    tdr 文件路径
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 元数据库已满
 *      \li 锁建立失败
 *      \li 元数据库中存在这个元数据，但是 build版本不同或者尺寸不同
 *      \li 这个 tdr文件太大，如果导入则超出了共享内存建立时设置的最大尺寸
 */
TSF4G_API int32_t tmb_append(INOUT HTMBDESC    a_hDesc,
                             IN    const char *a_pszPath);

/*!
 *  \fn TSF4G_API int32_t tmb_open_metalib(INOUT HTMBDESC    a_hDesc,
 *                                         IN    const char *a_pszLib,
 *                                         IN    int32_t     a_iVersion);
 *  \brief              这个函数根据名字和版本在元数据库中查找 Lib
 *                      如果找到，则设置元数据库句柄中的 Lib变量
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_pszLib     要查找的 Lib名字
 *  \param a_iVersion   满足条件的最大的版本号
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到符合条件的 Lib
 */
TSF4G_API int32_t tmb_open_metalib(INOUT HTMBDESC    a_hDesc,
                                   IN    const char *a_pszLib,
                                   IN    int32_t     a_iVersion);

/*!
 *  \fn TSF4G_API int32_t tmb_meta_by_name(IN    HTMBDESC    a_hDesc,
 *                                         IN    const char *a_pszName,
 *                                         INOUT LPTDRMETA  *a_ppstMeta);
 *  \brief              这个函数用元数据库句柄中指定的 Lib查找符合名字的条目
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_pszName    句柄指定的 Lib中找名字符合这个参数的条目
 *  \param a_ppstMeta   不能为空，如果找到了，这个指针指向的条目填写条目地址
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 元数据库中没有指定 Lib
 *      \li 指定的 Lib中没有找到符合的条目
 */
TSF4G_API int32_t tmb_meta_by_name(IN    HTMBDESC    a_hDesc,
                                   IN    const char *a_pszName,
                                   INOUT LPTDRMETA  *a_ppstMeta);

/*!
 *  \fn TSF4G_API int32_t tmb_meta_by_id(IN    HTMBDESC   a_hDesc,
 *                                       IN    int32_t    a_iID,
 *                                       INOUT LPTDRMETA *a_ppstMeta);
 *  \brief              这个函数用元数据库句柄中指定的 Lib查找符合 id 的条目
 *  \param a_hDesc      不能为空，这个指针是元数据库访问句柄
 *  \param a_iID        句柄指定的 Lib中找符合这个 id 的条目
 *  \param a_ppstMeta   不能为空，如果找到了，这个指针指向的条目填写条目地址
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 元数据库中没有指定 Lib
 *      \li 指定的 Lib中没有找到符合的条目
 */
TSF4G_API int32_t tmb_meta_by_id(IN    HTMBDESC   a_hDesc,
                                 IN    int32_t    a_iID,
                                 INOUT LPTDRMETA *a_ppstMeta);

#ifdef __cplusplus
}
#endif

#endif /* TMETABASE_H */
