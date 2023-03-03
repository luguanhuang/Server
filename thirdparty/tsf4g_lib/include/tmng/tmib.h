
#ifndef TMNG_MIB_H
#define TMNG_MIB_H

#include "pal/pal.h"
#include "pal/ttypes.h"
#include "tmng/tmetabase.h"


#define TMIB_VERSION        0x01

#define TMIB_IPC_TYPE       '1'
#define DR_NAME_LEN         32
#define DEFAULT_MIB_DATA_SIZE       1024*1024

struct tagMIBDataHead
{
    int32_t iPeriods;               /* report periods  (minutes) */
    time_t  tReportTime;            /* report timestamp */
    time_t  tUpdateTime;            /* update timestamp */
};

typedef struct tagMIBDataHead    TMIBHEAD;
typedef struct tagMIBDataHead    *LPTMIBHEAD;

#define     MIB_HEAD_SIZE       128


struct tagTMIBEntry
{
    char    szLib[DR_NAME_LEN];
    char    szMeta[DR_NAME_LEN];
    char    szDomain[DR_NAME_LEN];
    char    szName[DR_NAME_LEN];
    int32_t iProcID;
    int32_t iVersion;       
    int32_t iOff;                   /* iOff must be page aligned. */
    int32_t iSize;                  /* Use mprotect to do protect. */
    int32_t bExtern;                /* if  use external memory */
    HANDLE  iExShm;
    int32_t iExSize;
    int32_t bDelete;                /* this entry is deleted. */
    int32_t iPeriods;               /* report periods  (minutes) */
    time_t  tReportTime;            /* report timestamp */
    time_t  tUpdateTime;            /* update timestamp */
    int32_t bSystem;                /* this entry is added by system. */    
    int32_t iReserv1;
    int32_t iReserv2;
};

typedef struct tagTMIBEntry     TMIBENTRY;
typedef struct tagTMIBEntry    *LPTMIBENTRY;

struct tagTMIBData
{    
    char        szLib[DR_NAME_LEN];     /*! input/output. */
    char        szMeta[DR_NAME_LEN];    /*! input/output. */
    char        szDomain[DR_NAME_LEN];  /*! input. */
    char        szName[DR_NAME_LEN];    /*! input. */
    int32_t     iProcID;                /* input. */
    int32_t     iVersion;               
    int32_t     iSize;                  /* input/output. */
    int32_t     bExtern;                /* input/output. */
    int32_t     iPeriods;      
    LPTMIBHEAD  pstHead;
    char       *pszData;                /* output. */
    int32_t     iEntry;                 /* output, internal used. */
};

typedef struct tagTMIBData  TMIBDATA;
typedef struct tagTMIBData *LPTMIBDATA;

/*!
 * \brief 共享内存中信息数据库头信息
 * \struct TMIBINFO
 *
 */
struct tagTMIBInfo
{
    int32_t   iVersion;     /*!< 版本号 */
    int32_t   iSize;        /*!< 整个 Mib数据库在共享内存中最大占用的尺寸 */
    int32_t   iMaxDatas;    /*!< 最多可以容纳的 \struct TMIBENTRY 项数 */
    int32_t   iCurDatas;    /*!< 当前已经容纳的 \struct TMIBENTRY 项数 */
    int32_t   iExternSize;  /*!< 扩展尺寸 */
    int32_t   iHeadSize;    /*!< 头尺寸，也对齐的，应该对其写保护 */
    int32_t   iCheckSum;    /*!< 校验和 */
    TMIBENTRY entries[1];
};

typedef struct tagTMIBInfo  TMIBINFO;
typedef struct tagTMIBInfo *LPTMIBINFO;

/*!
 * \brief 用户程序访问信息数据库时的句柄结构
 * \struct TMIBDESC
 *
 */
struct tagTMIBDesc
{
    HANDLE    iLock;        /*!< 信息数据库访问锁 */
    HANDLE    iShm;         /*!< 信息数据库共享内存访问标识 */
    int32_t   bCreate;      /*!< 是否是信息数据库建立者 */
    TMIBINFO *pstInfo;      /*!< 信息数据库指针 */
};

typedef struct tagTMIBDesc  TMIBDESC;
typedef struct tagTMIBDesc *LPMIBDESC;
typedef struct tagTMIBDesc *HTMIB;

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 *  \fn TSF4G_API int32_t tmib_create(OUT HTMIB      *a_phMib,
 *                                    IN  const char *a_pszKey,
 *                                    IN  int32_t     a_iDatas,
 *                                    IN  int32_t     a_iSize);
 *  \brief              这个函数建立一个信息数据库访问句柄
 *                      如果数据库不存在，则先建立他
 *  \param a_phMib      不能为空，这个指针指向的位置会填写返回句柄
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \param a_iDatas     最多容纳的条目个数
 *  \param a_iSize      最大共享内存尺寸
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li \a a_iSize 设置过小，容纳不下 \a a_iDatas 个条目
 *      \li 内存分配请求失败
 *      \li 锁建立失败
 *      \li 取得共享内存标识符失败
 *      \li 已有的库版本不匹配
 */
TSF4G_API int32_t tmib_create(OUT HTMIB      *a_phMib,
                              IN  const char *a_pszKey,
                              IN  int32_t     a_iDatas,
                              IN  int32_t     a_iSize);

/*!
 *  \fn TSF4G_API int32_t tmib_destroy(IN const char *a_pszKey);
 *  \brief              这个函数释放一个已存在的信息数据库
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到 a_pszKey对应的信息数据库
 */
TSF4G_API int32_t tmib_destroy(IN const char *a_pszKey);

/*!
 *  \fn TSF4G_API int32_t tmib_open(OUT HTMIB      *a_phMib,
 *                                  IN  const char *a_pszKey);
 *  \brief              这个函数根据 a_pszKey 取得对应信息数据库访问句柄
 *  \param a_phMib      不能为空，这个指针指向的位置会填写返回句柄
 *  \param a_pszKey     句柄的标识字符串，内部用来区分锁和共享内存
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 内存分配请求失败
 *      \li 锁建立失败
 *      \li 取得共享内存标识符失败
 *      \li 已有的库版本不匹配
 */
TSF4G_API int32_t tmib_open(OUT HTMIB      *a_phMib,
                            IN  const char *a_pszKey);

/*!
 *  \fn TSF4G_API int32_t tmib_close(IN HTMIB *a_phMib);
 *  \brief              这个函数关闭信息数据库，并释放句柄内存
 *  \param a_phMib      不能为空，这个指针指向要关闭的信息数据库句柄
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmib_close(IN HTMIB *a_phMib);

/*!
 *  \fn TSF4G_API int32_t tmib_set_extern_size(INOUT HTMIB   a_hMib,
 *                                             IN    int32_t a_iExSize);
 *  \brief              这个函数设置对应信息数据库的 iExternSize变量
 *                      这个变量代表信息数据库扩展是的最大尺寸
 *                      设置过程中会锁定信息数据库句柄
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_iExSize    要设置的扩展尺寸，如果不大于零，则设置成缺省尺寸
 *                      这个尺寸是页对齐的
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmib_set_extern_size(INOUT HTMIB   a_hMib,
                                       IN    int32_t a_iExSize);

/*!
 *  \fn TSF4G_API int32_t tmib_set_updtime_unlock(INOUT HTMIB     a_hMib,
 *                                                INOUT TMIBDATA *a_pstData,
 *                                                IN    time_t    a_tUpdTime);
 *  \brief              这个函数在信息数据库中查找一个条目并设置更新时间
 *                      设置过程中没有锁定信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个是条目查询条件
 *  \param a_tUpdTime   待设置的时间
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到对应条目
 *      \li 对应条目索引越界
 */
TSF4G_API int32_t tmib_set_updtime_unlock(INOUT HTMIB     a_hMib,
                                          INOUT TMIBDATA *a_pstData,
                                          IN    time_t    a_tUpdTime);

/*!
 *  \fn TSF4G_API int32_t tmib_set_updtime_now(INOUT HTMIB     a_hMib,
 *                                             INOUT TMIBDATA *a_pstData);
 *  \brief              这个函数在信息数据库中查找一个条目并刷新更新时间
 *                      设置过程中锁定了信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个是条目查询条件
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 信息数据库锁定失败
 *      \li 没有找到对应条目
 *      \li 对应条目索引越界
 */
TSF4G_API int32_t tmib_set_updtime_now(INOUT HTMIB     a_hMib,
                                       INOUT TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_find_data_unlocked(
 *                                          IN    HTMIB     a_hMib,
 *                                          INOUT TMIBDATA *a_pstData,
 *                                          IN    int32_t   a_bIncludeDelete);
 *  \brief              在信息数据库中查找匹配 a_pstData的条目
 *                      查询过程中没有锁定信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构提供查询条件
 *                      如果成功返回，这个结构中会填写找到的条目
 *                      \n
 *                      查询条件为：
 *                      \li szName必须匹配
 *                      \li szLib, szMeta, szDomain为空时默认为匹配
 *                      \li iProcID 必须匹配
 *                      \li iVersion为，默认为匹配
 *                      \li iPeriods为，默认为匹配
 *                      查询成功时填写除 szLib, szMeta, szDomain之外的字段
 *  \param a_bIncludeDelete
 *                      非零表示查找范围不包含已删除的条目，否则包含
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 */
TSF4G_API int32_t tmib_find_data_unlocked(IN    HTMIB     a_hMib,
                                          INOUT TMIBDATA *a_pstData,
                                          IN    int32_t   a_bIncludeDelete);

/*!
 *  \fn TSF4G_API int32_t tmib_register_data_unlocked(
 *                                                  INOUT HTMIB     a_hMib,
 *                                                  INOUT TMIBDATA *a_pstData);
 *  \brief              在信息数据库中添加一个新条目
 *                      添加过程中没有锁定信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构待添加的条目
 *                      成功时补全所有的字段
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li a_pstData 中的 iSize不大于零
 *      \li 已经没有存储新条目的空间了
 */
TSF4G_API int32_t tmib_register_data_unlocked(INOUT HTMIB     a_hMib,
                                              INOUT TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_check_entry_unlocked(INOUT HTMIB     a_hMib,
 *                                                  IN    TMIBDATA *a_pstData);
 *  \brief              检查信息数据库中的条目，如果是删除的，清除删除标志
 *                      过程中没有锁定信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构是查询条件
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmib_check_entry_unlocked(INOUT HTMIB     a_hMib,
                                            IN    TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_register_data(INOUT HTMIB     a_hMib,
 *                                           INOUT TMIBDATA *a_pstData);
 *  \brief              在信息数据库中添加一个新条目
 *                      添加过程中锁定了信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构待添加的条目
 *                      成功时补全所有的字段
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 信息数据库锁定失败
 *      \li 调用 tmib_register_data_unlocked或 tmib_check_entry_unlocked失败
 */
TSF4G_API int32_t tmib_register_data(INOUT HTMIB     a_hMib,
                                     INOUT TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_register_data_by_mbhd(INOUT HTMIB     a_hMib,
 *                                                   IN    HTMBDESC  a_hDesc,
 *                                                   INOUT TMIBDATA *a_pstData);
 *  \brief              根据元数据库中的条目长度来设置 a_pstData的 iSize 
 *                      并在信息数据库中添加这个新条目
 *                      添加过程中锁定了信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_hDesc      不能为空，这个指针是元数据库句柄
 *  \param a_pstData    不能为空，这个结构待添加的条目
 *                      成功时补全所有的字段
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 元数据库中没有找到对应的条目
 *      \li 调用 tmib_register_data 失败
 */
TSF4G_API int32_t tmib_register_data_by_mbhd(INOUT HTMIB     a_hMib,
                                             IN    HTMBDESC  a_hDesc,
                                             INOUT TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_register_data_by_mbkey(
 *                                              INOUT HTMIB       a_hMib,
 *                                              IN    const char *a_pszBase,
 *                                              INOUT TMIBDATA   *a_pstData);
 *  \brief              使用串 a_pszBase查询元数据库中的条目长度
 *                      来设置 a_pstData的 iSize 
 *                      并在信息数据库中添加这个新条目
 *                      添加过程中锁定了信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pszBase    不能为空，这个串是元数据库标识
 *  \param a_pstData    不能为空，这个结构待添加的条目
 *                      成功时补全所有的字段
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 打开元数据库失败
 *      \li 调用 tmib_register_data_by_mbhd 失败
 */
TSF4G_API int32_t tmib_register_data_by_mbkey(INOUT HTMIB       a_hMib,
                                              IN    const char *a_pszBase,
                                              INOUT TMIBDATA   *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_protect_data(IN TMIBDATA *a_pstData,
 *                                          IN int32_t   a_bReadOnly);
 *  \brief              根据 a_bReadOnly设置 a_pstData的属性为只读或可读写
 *  \param a_pstData    不能为空，这个结构提供待设置读写属性的数据
 *  \param a_bReadOnly  非零表示查找到的条目为只读，否则可读可写
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li a_pstData 中 pszData为空
 *      \li a_pstData 中 iSize不大于零
 */
TSF4G_API int32_t tmib_protect_data(IN TMIBDATA *a_pstData,
                                    IN int32_t   a_bReadOnly);

/*!
 *  \fn TSF4G_API int32_t tmib_unregister_data_unlocked(
 *                                                  INOUT HTMIB     a_hMib,
 *                                                  IN    TMIBDATA *a_pstData);
 *  \brief              在信息数据库中查找匹配 a_pstData的条目
 *                      如果找到，则把对应条目设置为已删除
 *                      本函数执行过程中没有锁定信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构提供查询条件
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 */
TSF4G_API int32_t tmib_unregister_data_unlocked(INOUT HTMIB     a_hMib,
                                                IN    TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_unregister_data(INOUT HTMIB     a_hMib,
 *                                             IN    TMIBDATA *a_pstData);
 *  \brief              在信息数据库中查找匹配 a_pstData的条目
 *                      如果找到，则把对应条目设置为已删除
 *                      本函数执行过程中锁定了信息数据库
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构提供查询条件
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 */
TSF4G_API int32_t tmib_unregister_data(INOUT HTMIB     a_hMib,
                                       IN    TMIBDATA *a_pstData);

/*!
 *  \fn TSF4G_API int32_t tmib_unregister_domain_unlocked(
 *                                          INOUT HTMIB       a_hMib,
 *                                          IN    const char *a_pszDomain);
 *  \brief              在信息数据库中将匹配的条目标识为删除
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pszDomain  不能为空，这个串指示待删除的 szDomain
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmib_unregister_domain_unlocked(
                                                INOUT HTMIB       a_hMib,
                                                IN    const char *a_pszDomain);

/*!
 *  \fn TSF4G_API int32_t tmib_unregister_domain(INOUT HTMIB       a_hMib,
 *                                               IN    const char *a_pszDomain);
 *  \brief              在信息数据库中将匹配的条目标识为删除
 *                      和 tmib_unregister_domain_unlocked不同之处
 *                      是本函数删除过程中用锁来保护
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pszDomain  不能为空，这个串指示待删除的 szDomain
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li 参数检查失败
 */
TSF4G_API int32_t tmib_unregister_domain(INOUT HTMIB       a_hMib,
                                         IN    const char *a_pszDomain);

/*!
 *  \fn TSF4G_API int32_t tmib_get_data(IN    HTMIB     a_hMib,
 *                                      INOUT TMIBDATA *a_pstData,
 *                                      IN    int32_t   a_bReadOnly);
 *  \brief              在信息数据库中查找匹配 a_pstData的条目
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构提供查询条件
 *                      如果成功返回，这个结构中会填写找到的条目
 *                      \n
 *                      查询条件为：
 *                      \li szDomain和 szName必须匹配
 *                      \li iProcID 为零默认为匹配
 *                      \li iVersion为零或者待匹配的条目为零，默认为匹配
 *                      \li iPeriods为零或者待匹配的条目为零，默认为匹配
 *                      查询成功时填写除 iProcID和 bExtern之外的字段
 *  \param a_bReadOnly  非零表示查找到的条目为只读，否则可读可写
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 *      \li 找到的条目没有页对齐
 */
TSF4G_API int32_t tmib_get_data(IN    HTMIB     a_hMib,
                                INOUT TMIBDATA *a_pstData,
                                IN    int32_t   a_bReadOnly);

/*!
 *  \fn TSF4G_API int32_t tmib_validate_data(IN    HTMIB     a_hMib,
 *                                           INOUT TMIBDATA *a_pstData,
 *                                           IN    int32_t   a_bReadOnly);
 *  \brief              校验 a_pstData在信息数据库中是否存在
 *  \param a_hMib       不能为空，这个指针是信息数据库句柄
 *  \param a_pstData    不能为空，这个结构是待校验的数据
 *  \param a_bReadOnly  查询 a_hMib 时非零表示查找到的条目为只读，否则可读可写
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li a_pstData 不在索引越界
 *      \li a_pstData 标识为删除而且在信息数据库中找不到对应条目
 */
TSF4G_API int32_t tmib_validate_data(IN    HTMIB     a_hMib,
                                     INOUT TMIBDATA *a_pstData,
                                     IN    int32_t   a_bReadOnly);

/*!
 *  \fn TSF4G_API int32_t tmib_dump_head(IN HTMIB  a_hMib,
 *                                       IN FILE  *a_fp);
 *  \brief              这个函数根据信息数据库句柄，输出头数据到 a_fp 中
 *  \param a_hMib       不能为空，这个指针是待输出的信息数据库句柄
 *  \param a_fp         待输出的内容送到这个文件句柄里
 *                      如果 a_fp 是空，则输出到 stdout
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败时表示
 *      \li \a a_hMib 为空
 */
TSF4G_API int32_t tmib_dump_head(IN HTMIB  a_hMib,
                                 IN FILE  *a_fp);

/*!
 *  \fn TSF4G_API int32_t tmib_dump(IN HTMBDESC  a_hDesc,
 *                                  IN TMIBDATA *a_pstData,
 *                                  IN FILE     *a_fp);
 *  \brief              根据指定的 a_pstData中指定的 Lib和 Meta 查找元数据库
 *                      如果找到，则根据找到的元数据格式化 a_pstData
 *                      并把格式化后的结果输出到 a_fp 文件句柄中
 *  \param a_hDesc      不能为空，这个指针是元数据库句柄
 *  \param a_pstData    不能为空，这个结构中的 Lib和 Meta 是查询条件
 *  \param a_fp         待输出的内容送到这个文件句柄里
 *                      如果 a_fp 是空，则输出到 stdout
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 *      \li 函数 tdr_fprintf调用失败
 */
TSF4G_API int32_t tmib_dump(IN HTMBDESC  a_hDesc,
                            IN TMIBDATA *a_pstData,
                            IN FILE     *a_fp);

/*!
 *  \fn TSF4G_API int32_t tmib_dump_once(IN const char *a_pszBase,
 *                                       IN TMIBDATA   *a_pstData,
 *                                       IN FILE       *a_fp);
 *  \brief              根据指定的 a_pstData中指定的 Lib和 Meta 查找元数据库
 *                      元数据库由给定的标识串 a_pszBase来取得
 *                      如果找到，则根据找到的元数据格式化 a_pstData
 *                      并把格式化后的结果输出到 a_fp 文件句柄中
 *  \param a_pszBase    不能为空，句柄标识字符串，内部用来区分锁和共享内存
 *                      通过这个串找到对应的元数据库
 *  \param a_pstData    不能为空，这个结构中的 Lib和 Meta 是查询条件
 *  \param a_fp         待输出的内容送到这个文件句柄里
 *                      如果 a_fp 是空，则输出到 stdout
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到匹配的条目
 *      \li 函数 tdr_fprintf调用失败
 */
TSF4G_API int32_t tmib_dump_once(IN const char *a_pszBase,
                                 IN TMIBDATA   *a_pstData,
                                 IN FILE       *a_fp);

/*!
 *  \fn TSF4G_API int32_t tmib_dump_all(IN HTMIB       a_hMib,
 *                                      IN const char *a_pszBase,
 *                                      IN FILE       *a_fp);
 *  \brief              这个函数将信息数据库中的所有条目输出到 a_fp 中
 *                      元数据库通过 a_pszBase来取得
 *  \param a_hMib       不能为空，信息数据库句柄
 *  \param a_pszBase    不能为空，句柄标识字符串，内部用来区分锁和共享内存
 *                      通过这个串找到对应的元数据库
 *  \param a_fp         待输出的内容送到这个文件句柄里
 *                      如果 a_fp 是空，则输出到 stdout
 *  \return             成功返回 0; 失败返回小于零的整数;
 *  \n
 *      失败分为几种情况
 *      \li 参数检查失败
 *      \li 没有找到 a_pszBase对应的元数据库
 */
TSF4G_API int32_t tmib_dump_all(IN HTMIB       a_hMib,
                                IN const char *a_pszBase,
                                IN FILE       *a_fp);


#ifdef __cplusplus
}
#endif

#endif /* TMNG_MIB_H  */
