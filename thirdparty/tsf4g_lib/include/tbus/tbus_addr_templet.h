/**
*
* @file     tbus_addr_templet.h
* @brief    BUS通信地址模板分析的相关接口
* Bus系统进程通信地址有两种表示方法：
*	1.内部表示：32位无符号整数
*	2.外部显示或配置表示：十进制点分表示的字符串 xx.xx.xx.xxx
*
* @author jackyai
* @version 1.0
* @date 2008-09-10
*
*
* Copyright (c)  2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/
#ifndef TBUS_ADDR_TEMPLET_H
#define	TBUS_ADDR_TEMPLET_H

#include <stdio.h>
#include "tbus/tbus.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TBUS_ADDR_TEMPLET_MAX_BITS_NUM	32
#define TBUS_MAX_ADDR_SEGMENT_NUM	32	/*Tbus通信地址十进制表示法段数最大数目*/
#define TBUS_SPLIT_SEGMENT_CHARACTER	'.'	/*十进制点分法中分隔每一节的分隔字符*/
#define TBUS_SPLIT_SEGMENT_DESCRITION_CHARACTER	':'	/*十进制点分法中分隔每一节中描述串与数字的分隔字符*/



typedef struct tagTbusAddrSeg TBUSHANDLEEG;
typedef struct tagTbusAddrSeg *LPTBUSHANDLEEG;
typedef struct tagTbusAddrTemplet TBUSADDRTEMPLET;
typedef struct tagTbusAddrTemplet *LPTBUSADDRTEMPLET;

typedef struct tagTbusAddrSegRange TBUSADDRSEGRANGE;
typedef struct tagTbusAddrSegRange *LPTBUSADDRSEGRANGE;
typedef struct tagTbusAddrInstance TBUSADDRINSTANCE;
typedef struct tagTbusAddrInstance *LPTBUSADDRINSTANCE;


struct tagTbusAddrSeg
{
	unsigned char bStartBit;	/*这一节所占的bit从第几位开始，设定第31位为最高位，第0位为最低位*/
	unsigned char bBitNum;		/*这一节所占的bit数*/
	unsigned int dwMaxVal;		/*这一节能表示的最大值,即这一节数值为0～dwMaxVal，dwMaxVal=2^bBitNum - 1*/
};

/*通信地址十进制点分法模板数*/
struct tagTbusAddrTemplet
{
	int iSegNum;	/**/
	TBUSHANDLEEG astSeg[TBUS_MAX_ADDR_SEGMENT_NUM];
};


/*通信地址一段的表示范围*/
struct tagTbusAddrSegRange
{
    int iBegin;
    int iEnd;
};

/*通信地址*/
struct tagTbusAddrInstance
{
    int iSegNum;
    TBUSADDRSEGRANGE astSegRange[TBUS_MAX_ADDR_SEGMENT_NUM];
};


/**/



/**根据通信地址十进制点分模板串分析出通信地址模板内部结构
* 点分十进制表示串的模板为："segmentdesc:bitnum.segmendesc:binnum.....segmentdesc:bitnum",例如
* "world:8.zone:8.func:8.instance:8"
*@param[out] a_pstAddrTemplet 保存通信地址点分法表示模块信息的数据结构指针
*@param[in] a_pszAddTemplet 十进制点分法表示的通信地址模板
*@retval 0	处理成功
*@retval <0 处理失败
*/
int tbus_parse_addrtemplet(OUT LPTBUSADDRTEMPLET a_pstAddrTemplet, IN const char *a_pszAddTemplet);

void tbus_dump_addrtemplet(IN LPTBUSADDRTEMPLET a_pstAddrTemplet, IN FILE *a_fpDump);

/**根据地址模板设置将内部通信地址 转换成点分十进制表示的地址串
*@param[in] a_pstTemplet 通信地址模板结构的指针
*@param[in] a_iAddr	通信地址
*@return 成点分十进制表示的地址串
*@note 返回的成点分十进制表示的地址串保存在一个静态缓冲区中，后一次调用会覆盖前一次调用时获取的信息
*/
char *tbus_addr_nota_by_addrtemplet(IN LPTBUSADDRTEMPLET a_pstTemplet,IN TBUSADDR a_iAddr) ;

/**根据地址模板设置将内部通信地址 转换成点分十进制表示的地址串
*@param[in] a_pstTemplet 通信地址模板结构的指针
*@param[in] a_iAddr	通信地址
*@param[out] a_pszDstBuff 输出缓冲区
*@param[in] a_dwBuffSize 输入缓冲区长度
*@return 成点分十进制表示的地址串
*/
char *tbus_addr_notp_by_addrtemplet(LPTBUSADDRTEMPLET a_pstTemplet, TBUSADDR a_iAddr,
                                    char *a_pszDstBuff, unsigned int a_dwBuffSize);

/**根据通信地址模板将点分十进制表示的地址串转换成内部表示通信地址
*@param[in] a_pstTemplet 通信地址模板结构的指针
*@param[in] a_pszAddr 点分十进制表示的地址串缓冲去指针
*@param[out] a_piAddr 保存通信地址的指针
*@retval	0 成功
*@retval <0	失败
*/
int tbus_addr_aton_by_addrtemplet(IN LPTBUSADDRTEMPLET a_pstTemplet, IN const char *a_pszAddr, OUT LPTBUSADDR a_piAddr) ;


#ifdef __cplusplus
}
#endif
#endif /*TBUS_ADDR_TEMPLET_H*/
