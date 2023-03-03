#ifndef _TSM_TDR_H_
#define _TSM_TDR_H_
//============================================================================
// @Id:       $Id: tsm_tdr.h 46990 2013-04-09 08:00:13Z ryanczhang $
// @Author:   $Author: ryanczhang $
// @Date:     $Date:: 2013-04-09 16:00:13 +0800 #$
// @Revision: $Revision: 46990 $
// @HeadURL:  $HeadURL:: https://sz-svn4.tencent.com/ied/ied_tsf4g_rep/tsf4g#$
//----------------------------------------------------------------------------

/*!
 * \file tsm_tdr.h
 * \brief tsm for tdr api
 * \version 1.0
 * date 2010-09-13
 */

#include <sys/types.h>

#include "tdr/tdr_metalib_init.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 * TSM_URI 是一个描述工具，可以用来描述位置，查询，定义等
 * 对于 Tdr方式的共享内存管理来说，有必要定义 TsmUriTdr的格式和内部表示
 *
 * TSM URI for TDR 格式:
 *      tsm-data://<bussi-id>/<proc-name>:<proc-id>/<func-name>/<...>
 *          这是第一种格式，叫做 data 格式
 *
 * <1> bussi_id           业务id，例如: 大碗菜；QQ幻想；QQ飞车的内部标识
 * <2> process_name       进程名，例如: torm_server
 * <3> process_id         进程号，相同进程名的不同实例
 *                        如果不想区分进程，这个位置可以为空
 *                        另外需要注意的是：由于历史原因，process_id长度最大不能超过32字节
 * <4> function_name      功能名，例如: config, status
 * <5> ...                后面的字段可以自定义
 *
 *      tsm-meta://<bussi-id>/<lib>:<ver>:<build_ver>/<meta>:<ver>:<size>/<...>
 *          这是第二种格式，叫做 meta 格式
 *          其中
 * <1> bussi_id           业务id，例如: 大碗菜；QQ幻想；QQ飞车的内部标识
 * <2> lib                是 metalib名字
 * <3> ver                是 metalib版本号
 * <4> build_ver          是 metalib的 build版本号
 * <5> meta               是 meta 的名字
 * <6> ver                是 meta 的版本
 * <7> size               是 meta 的尺寸
 * <8> ...                后面的字段可以自定义
 *      如果只有 tsm-meta://<bussi-id>/<lib-name>/ 这代表一个 lib
 *
 * 两种格式中，任何<>括起来的字段都可以换成 *(表示不关心)
 *      一个查询 URI可以有 0个或多个 *字段
 *      一个查询 URI最后面的 *字段可以省略不写
 */

/*!
 * \brief   API 一共要提供六种功能，分别是
 *          1 错误处理
 *          2 将某个 data 和某个 meta 绑定，或者设定一个 data 的尺寸
 *          3 将 data 或者 meta 发布到共享内存中
 *          4 从共享内存中获得 data 指针
 *          5 将 data 从共享内存中删除
 *          6 取得共享内存中的 data 或者 meta 内容
 *      查询或遍历共享内存中已经存在的 data 或者 meta
 */

/*!
 * \brief 0 句柄相关 API
 *      为了使 API功能清晰，所有的 API都和一个句柄相关联
 *      这样 API可以用句柄来暂存数据
 *      每次调用 API之前，相关句柄中的错误信息将被清除
 */
typedef void *TsmTdrHandleP;

/*!
 * \fn      int tsm_tdr_base_set(int a_iIndexKey, int a_iLibCap, int a_iUriCap);
 * \brief   设置四个变量，以后用这些变量控制 index
 *          如果参数为0，则使用缺省值，缺省值在 tsm_base  中单件初始化函数中设置
 *          这个接口，必须在tsm_tdr_open之前调用，多次重复调用tsm_tdr_base_set会被忽略
 * \param
 *          \li a_iIndexKey 这个参数用来唯一标识一个 index
 *          \li a_iLibCap   每个 Index中最多可以容纳多少条 Lib记录，不能为负数，0为默认值（默认为3000）
 *          \li a_iUriCap   每个 Index中最多可以容纳多少条 Uri记录，不能为负数，0为默认值（默认为10000）
 * \return  非负 有效的设置个数（注意：非首次调用此函数都会返回0）
 *          \li -1          参数错误：a_iMetaLibCap或a_iDataCap为负数
 *          \li -2          初始化TsmBase单件实例失败
 * \n
 */
int tsm_tdr_base_set(int a_iIndexKey, int a_iMetaLibCap, int a_iDataCap);

/*!
 * \fn      int tsm_tdr_open(TsmTdrHandleP *a_ppHandle);
 * \brief   获得 TsmTdrHandleP 句柄
 *          tsm_tdr_open会根据tsm_tdr_base_set设置的全局变量来创建或重用共享内存索引区
 *          对于TsmIndexKey对应的共享内存：
 *          如果存在，tsm_tdr_open会修正单件实例TSM BASE中各变量值。如果不存在，则会创建TsmIndex共享内存
 *
 *          同时，由于tsm_tdr_open最终会根据设置的LibCap、UriCap等值来计算索引区大小IndexSize
 *          因此需要注意的是：一旦调用了tsm_tdr_open，再次调用tsm_tdr_base_set可能会引起后面不可预测的行为
 * \param   a_ppHandle   返回的句柄存储位置，如果是新创建句柄，必须为NULL
 * \return  成功返回0
 * \n
 *          其他情况返回
 *          \li -1  参数句柄指针为空
 *          \li -2  获取TsmIndex指针失败
 *          \li  1  句柄已被破坏
 */
int tsm_tdr_open(TsmTdrHandleP *a_ppHandle);

/*!
 * \fn      int tsm_tdr_close(TsmTdrHandleP *a_pHandle);
 * \brief   关闭一个 TsmTdrHandleP 句柄
 * \param   a_ppHandle   待关闭的句柄指针，必须为通过tsm_tdr_open调用返回的句柄指针
 * \return  成功返回 0
 * \n
 *          其他情况返回
 *          \li -1  参数句柄指针为空
 *          \li -2  参数句柄为空
 *          \li  1  句柄已被破坏
 */
int tsm_tdr_close(TsmTdrHandleP *a_ppHandle);

/*!
 * \brief 2 绑定相关 API
 *      一个句柄上可以绑定三种不同的变量
 *      1  data 格式的 URI
 *      2  meta 格式的 URI
 *      3  data 格式的 URI指定的结构体尺寸
 *  这三种变量单独存在，但是互相之间有一定的约束
 */

/*!
 * \fn      int tsm_tdr_bind_data(TsmTdrHandleP  a_pHandle,
 *                                const char    *a_pszUri);
 * \brief   将一个 data 格式的 URI绑定到句柄上，或者
 *          从句柄上清除一个 data 格式 (pszUri是 tsm-data://...)
 *          成功返回时，如果系统中已经存在这个 uri
 *          则这个 api会同时更新句柄的 meta 绑定和 size 绑定
 *          否则，清空 meta 绑定和清零 size 绑定
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pszUri    希望绑定到句柄上的 data URI
 * \return  成功返回 0，否则返回
 *          \li -1  参数句柄为空
 *          \li -2  参数句柄状态为关闭
 *          \li -3  URI 字符串格式错误
 *          \li -4  URI 类型错误，或者搜索过程出错
 *          \li -5  索引节点分配失败
 */
int tsm_tdr_bind_data(TsmTdrHandleP a_pHandle, const char *a_pszUri);

/*!
 * \fn      int tsm_tdr_bind_meta(TsmTdrHandleP  a_pHandle,
 *                                const char    *a_pszUri);
 * \brief   将一个 meta 格式的 URI绑定到句柄上
 *          调用这个 api之前，需要满足几个条件
 *          参数所指向的 meta 在系统中已经存在，或者可以自动载入
 *          句柄的 data 绑定不能为空
 *          句柄的 meta 绑定必须为空或者与参数相同
 *          句柄的 size 绑定为零，或者等于待绑定的 meta 中提取的尺寸
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pszUri    希望绑定到句柄上的 meta URI
 * \return  成功返回 0，否则返回
 *          \li -1  参数句柄为空
 *          \li -2  参数句柄状态为关闭
 *          \li -3  需要先 bind data
 *          \li -4  URI 字符串格式错误
 *          \li -5  URI 类型错误
 *          \li -6  句柄中 Meta 和待设置的 Meta 不匹配
 *          \li -7  待绑定的 meta 系统中不存在
 *          \li -8  索引节点分配失败
 *          \li -9  获得共享内存失败
 *          \li -10 共享内存尺寸为零
 *          \li -11 句柄中尺寸与待绑定的 Meta 不匹配
 */
int tsm_tdr_bind_meta(TsmTdrHandleP a_pHandle, const char *a_pszUri);

/*!
 * \fn      int tsm_tdr_bind_size(TsmTdrHandleP a_pHandle, size_t a_iSize);
 * \brief   将一个尺寸信息绑定到句柄上
 *          调用这个 api之前，需要满足几个条件
 *          句柄的 data 绑定不能为空
 *          句柄的 meta 绑定必须为空
 *          句柄的 size 绑定为零，或者与参数相同
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_iSize     希望绑定到句柄上的尺寸
 * \return  成功返回 0，否则返回
 *          \li -1  参数句柄为空
 *          \li -2  参数句柄状态为关闭
 *          \li -3  参数错误， a_iSize 为零
 *          \li -4  需要先 bind data
 *          \li -5  meta 绑定与 size 冲突
 *          \li -6  句柄中 Size 和待设置的 Size 不匹配
 */
int tsm_tdr_bind_size(TsmTdrHandleP a_pHandle, size_t a_iSize);

/*!
 * \brief 取得句柄上绑定过的 URI和结构体尺寸
 */

/*!
 * \fn      const char *tsm_tdr_bind_get_data(TsmTdrHandleP a_pHandle);
 * \brief   获得句柄上绑定的 data URI
 * \param   a_pHandle   tsm_tdr 句柄
 * \return  成功返回 data URI, 否则返回 NULL 并设置错误堆栈
 */
const char *tsm_tdr_bind_get_data(TsmTdrHandleP a_pHandle);

/*!
 * \fn      const char *tsm_tdr_bind_get_meta(TsmTdrHandleP a_pHandle);
 * \brief   获得句柄上绑定的 meta URI
 * \param   a_pHandle   tsm_tdr 句柄
 * \return  成功返回 meta URI, 否则返回 NULL 并设置错误堆栈
 */
const char *tsm_tdr_bind_get_meta(TsmTdrHandleP a_pHandle);

/*!
 * \fn      size_t tsm_tdr_bind_get_size(TsmTdrHandleP a_pHandle);
 * \brief   获得句柄上绑定的 data size
 * \param   a_pHandle   tsm_tdr 句柄
 * \return  成功返回 size, 否则返回 0 并设置错误堆栈
 */
size_t      tsm_tdr_bind_get_size(TsmTdrHandleP a_pHandle);


/*!
 * \brief 3 发布相关 API
 */

/*!
 * \fn      int tsm_tdr_load_data(TsmTdrHandleP  a_pHandle,
 *                                void          *a_pData,
 *                                int            a_iFlag);
 * \brief   发布 data 到系统中
 *          调用这个 api之前，需要满足几个条件
 *          句柄的 data 绑定不能为空
 *          句柄的 size 绑定不能为零
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pData     指向待发布的数据，例如: (struct tagUserData *)
 *                          数据长度为句柄的 size 绑定
 *          \li a_iFlag     标志，这个参数的含义是系统调用 open 的子集
 *                          包括 O_CREATE, O_EXCL, O_NONBLOCK or O_NDELAY
 * \return  成功返回 0，否则返回错误代码
 *          \li  1          a_iFlag 标志冲突 或 句柄为空
 *          \li -1          获取 size 失败 或 锁失败
 *          \li -2          获取共享内存指针失败
 *          \li -3          释放共享内存指针失败
 */
int tsm_tdr_load_data(TsmTdrHandleP a_pHandle, void *a_pData, int a_iFlag);


/*!
 * \fn      int tsm_tdr_load_meta(TsmTdrHandleP a_pHandle,
 *                                LPTDRMETALIB  a_pData,
 *                                int           a_iBid);
 * \brief   发布 metalib到系统中
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pData     LPTDRMETALIB 指向一个 metalib
 *          \li a_iBid      业务 ID
 * \return  成功返回 0，否则返回错误代码
 *          \li -1          tsm_tdr 句柄为空
 *          \li -2          tsm_tdr 句柄已关闭
 *          \li -3          a_pData 为空
 *          \li -4          a_iBid不能为-1（-1是TSM预留通配符）
 *          \li -5          内部生成 URI格式或类型错误
 *          \li -6          URI 查询出错
 *          \li -7          找到多条匹配记录
 *          \li -8          Meta Obj分配失败
 *          \li -9          内部建立索引时，索引越界
 *          \li -10         内部建立索引时，设置共享内存指针失败
 *          \li -11         内部建立索引时，设置 hash 结点失败
 */
int tsm_tdr_load_meta(TsmTdrHandleP a_pHandle,
                      LPTDRMETALIB  a_pData,
                      int           a_iBid);

/*!
 * \fn      int tsm_tdr_load_meta_ex(TsmTdrHandleP a_pHandle,
 *                                LPTDRMETALIB  a_pData,
 *                                int           a_iBid,
 *                                int           a_iRefreshMetaFlag);
 * \brief   发布 metalib到系统中
 * \param
 *          \li a_pHandle            tsm_tdr 句柄
 *          \li a_pData              LPTDRMETALIB 指向一个 metalib
 *          \li a_iBid               业务 ID
 *          \li a_iRefreshMetaFlag   当metalib不一致时，是否刷新meta的flag
 *                                   1:刷新，0:不刷新
 * \return  成功返回 0，否则返回错误代码
 *          \li  1          metalib存在，且与tsm不一致，进行了刷新
 *          \li  2          metalib存在，且与tsm不一致，没有进行刷新
 *          \li -1          tsm_tdr 句柄为空
 *          \li -2          tsm_tdr 句柄已关闭
 *          \li -3          a_pData 为空
 *          \li -4          a_iBid不能为-1（-1是TSM预留通配符）
 *          \li -5          内部生成 URI格式或类型错误
 *          \li -6          URI 查询出错
 *          \li -7          pstRow is null
 *          \li -8          tsm_tdr_check_metalib_i fail
 *          \li -9          找到多条匹配记录
 *          \li -10         tsm_tdr_create_meta_i fail
 */
int tsm_tdr_load_meta_ex(TsmTdrHandleP a_pHandle,
                      LPTDRMETALIB  a_pData,
                      int           a_iBid,
                      int           a_iRefreshMetaFlag);

/*!
 * \fn      int tsm_tdr_load_meta_file(TsmTdrHandleP  a_pHandle,
 *                                     const char    *a_pszPath,
 *                                     int            a_iBid);
 * \brief   将一个存在于文件中的 metalib发布到系统中
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pszPath   METALIB 文件路径
 *          \li a_iBid      业务 ID
 * \return  成功返回 0，否则返回错误代码
 *          \li -1          tsm_tdr 句柄为空
 *          \li -2          tsm_tdr 句柄已关闭
 *          \li -3          装入 metalib 失败
 *          \li -4          Lib 格式错误
 */
int tsm_tdr_load_meta_file(TsmTdrHandleP  a_pHandle,
                           const char    *a_pszPath,
                           int            a_iBid);


/*!
 * \brief 4 数据取得相关 API
 */

typedef void *TsmObjPtrP;

/*!
 * \fn      TsmObjPtrP tsm_tdr_ptr(TsmTdrHandleP a_pHandle, int a_iFlag);
 * \brief   获得句柄中绑定的 data 格式 URI所指向的，在共享内存中的指针结构
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_iFlag     标志，这个参数的含义是系统调用 open 的子集
 *                          包括 O_CREAT, O_EXCL, O_NONBLOCK or O_NDELAY
 * \return
 *          \li NULL        出错，并将错误压入错误栈
 *          \li -1          a_iFlag 的条件不满足
 *          \li 其他情况    成功
 */
TsmObjPtrP tsm_tdr_ptr(TsmTdrHandleP a_pHandle, int a_iFlag);

/*!
 * \fn      void *tsm_tdr_at(TsmObjPtrP a_pPtr, int a_iReadOnly);
 * \brief   返回 PTR所指向的，在共享内存中的结构
 * \param   a_iReadOnly     表示如何打开操作取得的空间
 * \return  NULL 代表失败，否则是指向结构的指针
 */
void *tsm_tdr_at(TsmObjPtrP a_pPtr, int a_iReadOnly);

/*!
 * \fn      int tsm_tdr_dt(void *a_pAt);
 * \brief   释放 tsm_tdr_at 返回的指针
 *          这个函数调用之后，不再保证那个指针指向正确的结构体
 *          内部实现主要是对共享内存数据解锁的过程
 *          用户程序应该成对地使用 at, dt
 *          并且在 at 之后尽快地使用 dt (即使不加锁)
 * \param   a_pAt           通过 tsm_tdr_ptr调用获得的指针
 * \return  成功返回 0，否则返回错误代码
 *          \li -1          内存指针为空
 *          \li -2          内存指针为 -1
 *          \li -3          内存指针无效
 *          \li -4          Index 不匹配
 */
int tsm_tdr_dt(void *a_pAt);


/*!
 * \brief 5 查询相关 API，这部分 API是模仿 MySQL
 */

typedef void *TsmTdrResultP;
typedef void *TsmTdrRowP;

/*!
 * \fn      int tsm_tdr_query(TsmTdrHandleP a_pHandle, const char *a_pszUri);
 * \brief   执行一个查询
 *          由于内部实现限制，tsm_tdr_query每次最多只能查询到1000个结果，如果
 *          有更多的数据的话，无法通过tsm_query找到（目前一般认为1000是足够的）
 * \param
 *          \li a_pHandle   tsm_tdr 句柄
 *          \li a_pszUri    待查询的 URI
 * \return  返回值不小于零，表示查询到的结果集里包含的结果数目
 *          否则，返回错误代码
 *          \li -1          tsm_tdr 句柄为空
 *          \li -2          URI 格式错误
 *          \li -3          查询出错
 *          \li -4          内存分配失败
 */
int tsm_tdr_query(TsmTdrHandleP a_pHandle, const char *a_pszUri);

/*!
 * \fn      TsmTdrResultP tsm_tdr_store_result(TsmTdrHandleP a_pHandle);
 * \brief   返回上一个 query查询的结果集
 *          一次查询之后，如果重复调用此函数，则除第一次外，后面都返回 NULL
 * \param   a_pHandle       tsm_tdr 句柄
 * \return  如果成功，返回结果集指针，否则返回 NULL
 */
TsmTdrResultP tsm_tdr_store_result(TsmTdrHandleP a_pHandle);

/*!
 * \fn      int tsm_tdr_num_rows(TsmTdrResultP a_pResult);
 * \brief   返回结果集中记录的行数
 * \param   a_pResult       结果集指针
 * \return
 *          \li -1          结果集指针为空
 *          \li 其他情况    结果集中记录数
 */
int tsm_tdr_num_rows(TsmTdrResultP a_pResult);

/*!
 * \fn      int tsm_tdr_data_seek(TsmTdrResultP a_pResult, int a_iIndex);
 * \brief   在结果集中定位一条记录
 * \param
 *          \li a_pResult   结果集指针
 *          \li a_iIndex    结果集中记录的索引，范围为 [0, num_rows)
 * \return  成功返回 0，否则返回错误代码
 *          \li -1          结果集指针为空
 *          \li -2          索引越界
 */
int tsm_tdr_data_seek(TsmTdrResultP a_pResult, int a_iIndex);

/*!
 * \fn      int tsm_tdr_free_result(TsmTdrResultP a_pResult);
 * \brief   释放一个结果集
 * \param   待释放的结果集
 * \return  返回 0
 */
int tsm_tdr_free_result(TsmTdrResultP a_pResult);


/*!
 * \fn      TsmTdrRowP tsm_tdr_fetch_row(TsmTdrResultP a_pResult);
 * \brief   从结果集中返回一行，如果已经到达结尾，返回 NULL
 * \param   a_pResult       结果集指针
 * \return  当 a_pResult不是空的时候，返回一条记录或 NULL(已经到达结果集结尾)
 */
TsmTdrRowP tsm_tdr_fetch_row(TsmTdrResultP a_pResult);

/*!
 * \fn      int tsm_tdr_row_unlink(TsmTdrRowP a_pRow, TsmTdrHandleP a_pHandle);
 * \brief   删除一行记录
 * \param
 *          \li a_pRow      记录指针
 *          \li a_pHandle   tsm_tdr 句柄
 * \return  成功返回 0，否则返回错误代码
 *          \li -1          tsm_tdr 句柄为空
 *          \li -2          tsm_tdr 句柄为关闭状态
 *          \li -3          记录指针为空或删除 data  hash 节点失败
 *          \li -5          删除 meta  hash 结点失败
 */
int         tsm_tdr_row_unlink(TsmTdrRowP a_pRow, TsmTdrHandleP a_pHandle);


/*!
 * \fn      int tsm_tdr_row_type(TsmTdrRowP a_pRow);
 * \brief   从记录中提取行类型
 * \param   a_pRow          记录指针
 * \return  记录指针为空时返回 -1 ，否则返回行类型
 */
int         tsm_tdr_row_type    (TsmTdrRowP a_pRow);

/*!
 * \fn      int tsm_tdr_row_is_data(TsmTdrRowP a_pRow);
 * \brief   判断记录是否为 data 类型
 * \param   a_pRow          记录指针
 * \return  是返回 1，否则返回 0
 */
int         tsm_tdr_row_is_data (TsmTdrRowP a_pRow);

/*!
 * \fn      int tsm_tdr_row_is_meta(TsmTdrRowP a_pRow);
 * \brief   判断记录是否为 meta 类型
 * \param   a_pRow          记录指针
 * \return  是返回 1，否则返回 0
 */
int         tsm_tdr_row_is_meta (TsmTdrRowP a_pRow);

/*!
 * \fn      const char *tsm_tdr_row_meta_uri(TsmTdrRowP a_pRow);
 * \brief   获得记录中 data URI 的 meta URI 绑定
 * \param   a_pRow          记录指针
 * \return  如果记录是 data URI 并且 meta URI 绑定过，则返回这个 meta URI
 *          否则，返回 NULL
 */
const char *tsm_tdr_row_meta_uri(TsmTdrRowP a_pRow);

/*!
 * \fn      const char *tsm_tdr_row_uri(TsmTdrRowP a_pRow);
 * \brief   获得记录中 URI
 * \param   a_pRow          记录指针
 * \return  如果记录类型错误，返回 NULL ；否则返回记录 URI
 */
const char *tsm_tdr_row_uri     (TsmTdrRowP a_pRow);

/*!
 * \fn      TsmObjPtrP tsm_tdr_row_meta_ptr(TsmTdrRowP a_pRow);
 * \brief   获得 meta 共享内存指针结构
 *          此接口用于得到data uri中元数据库(metalib)的TSM对象指针
 *          请进一步参考tsm_tdr_row_data_ptr的说明
 * \param   a_pRow          记录指针
 * \return  记录为空或共享内存尺寸为零，返回 NULL ；否则返回结构指针
 */
TsmObjPtrP  tsm_tdr_row_meta_ptr(TsmTdrRowP a_pRow);

/*!
 * \fn      TsmObjPtrP tsm_tdr_row_data_ptr(TsmTdrRowP a_pRow);
 * \brief   获得 data 共享内存指针结构
 *          如果TdrRow内容为meta uri，此接口可以得到meta uri对应的metalib的TSM对象指针
 *          如果TdrRow内容为data uri，此接口可以得到data uri对应的data的TSM对象指针
 *
 *          P.S. 这里再说明下data的涵义：
 *          对于TdrRow内容为meta uri的情况，data指的就是metalib这部分数据
 *          对于TdrRow内容为data uri的情况，data指的就是这部分数据data
 * \param   a_pRow          记录指针
 * \return  记录为空或共享内存尺寸为零，返回 NULL ；否则返回结构指针
 */
TsmObjPtrP  tsm_tdr_row_data_ptr(TsmTdrRowP a_pRow);

/*!
 * \fn      LPTDRMETA tsm_tdr_row_meta(TsmTdrRowP a_pRow);
 * \brief   获得记录中的 meta 指针
 *          此接口用于得到data uri中元数据结构的指针
 *          data涵义请参考tsm_tdr_row_data_ptr的说明
 * \param   a_pRow          记录指针
 * \return  如果记录不为空，且记录中存在 meta 指针，返回这个指针；否则返回 NULL
 */
LPTDRMETA   tsm_tdr_row_meta    (TsmTdrRowP a_pRow);

/*!
 * \fn      LPTDRMETA tsm_tdr_row_data(TsmTdrRowP a_pRow, int a_iReadOnly);
 * \brief   获得记录中的 data 指针
 *          如果TdrRow内容为meta uri，此接口可以得到meta uri对应的metalib缓冲区指针
 *          如果TdrRow内容为data uri，此接口可以得到data uri对应的data缓冲区指针
 *
 *          data涵义请参考tsm_tdr_row_data_ptr的说明
 * \param
 *          \li a_pRow          记录指针
 *          \li a_iReadOnly     返回的指针所指向的数据是否允许修改
 * \return  如果记录不为空，且记录中存在 data 指针，返回这个指针；否则返回 NULL
 */
void       *tsm_tdr_row_data    (TsmTdrRowP a_pRow, int a_iReadOnly);

/*!
 * \fn      size_t tsm_tdr_row_size(TsmTdrRowP a_pRow);
 * \brief   获得记录的 size
 * \param   a_pRow          记录指针
 * \return  如果记录指针为空或类型错误，返回 0；否则返回记录的 size
 */
size_t      tsm_tdr_row_size    (TsmTdrRowP a_pRow);

/*!
 * \fn      TsmTdrRowP tsm_tdr_row_only(TsmTdrHandleP a_pHandle);
 * \brief   有时，开发者明确地知道查询的返回只有一行
 *          这时可以用简化的 api返回这一行
 *          下一个查询之前，返回的行总是有效的
 * \param   a_pHandle       tsm_tdr 句柄
 * \return  如果句柄结果集中只有唯一的一行，返回这一行；否则返回 NULL
 */
TsmTdrRowP tsm_tdr_row_only(TsmTdrHandleP a_pHandle);

/*!
 * \fn      TsmTdrRowP tsm_tdr_row_first(TsmTdrHandleP a_pHandle);
 * \brief   有时，开发者只想处理结果集中的首行
 *          这时可以用简化的 api返回这一行
 *          下一个查询之前，返回的行总是有效的
 * \param   a_pHandle       tsm_tdr 句柄
 * \return  如果句柄结果集非空，返回第一行；否则返回 NULL
 */
TsmTdrRowP tsm_tdr_row_first(TsmTdrHandleP a_pHandle);


/*!
 * \brief 一些说明
 * TSM TDR API 的目的是在不同进程中共享数据，可以共享的数据有两种
 *
 *      1 元数据 meta
 *           一个 meta 存在于一个 metalib中
 *           一般 metalib作为一个整体发布到系统中
 *           系统通过 libname，metaname，version 区分不同的元数据
 *           元数据一旦发布，就不能删除
 *
 *      2 结构体，这是具体的数据
 *           一个结构体发布到系统中，会附带三个属性datauri，metauri和datasize
 *           如果 metauri和 datasize 同时存在，这两个属性不能冲突
 *           metauri 存在的时候，datasize可以省略，因为从 meta 中可以提取size
 *           datasize单独存在时，系统之外的的进程可以看到这个结构体
 *           但是只知道结构体的尺寸不知道这个结构体的格式，这叫做匿名发布
 */


#ifdef __cplusplus
}
#endif


//----------------------------------------------------------------------------
// THE END
//============================================================================
#endif
