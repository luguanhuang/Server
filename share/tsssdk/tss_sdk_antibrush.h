/**
* @file:   tss_sdk_antibrush.h
* @brief:  This header file provides the interface of anti-brush plugin.
* @copyright: 2011 Tencent. All Rights Reserved.
*/

#ifndef TSS_SDK_ANTIBRUSH_H_
#define TSS_SDK_ANTIBRUSH_H_

#include "tss_sdk.h"

#ifdef __cplusplus
extern "C"
{
#endif

static const unsigned int TSS_SDK_ANTIBRUSH_MAX_ACCOUNT_ID_LEN = 64; /* ���account_id���� Maximum account_id length */
static const unsigned int TSS_SDK_ANTIBRUSH_MAX_RUID_LEN = 64;       /* ���ruid���� Maximum ruid length */
static const unsigned int TSS_SDK_ANTIBRUSH_MAX_DEVICE_ID_LEN = 64;  /* ���device_id���� Maximum device_id length */

#pragma pack(1)

/* �û��ʺ����� */
/* User Account Type */
typedef enum
{
    ANTIRUSG_ACCOUNT_OTHER   = 0, /* ���� other account */
    ANTIRUSG_ACCOUNT_QQ_OPEN = 1, /* QQ�����ʺ� qq openplat account(openid) */
    ANTIRUSG_ACCOUNT_WECHAT  = 2, /* ΢�ſ����ʺ� wechat openplat account(openid) */
    ANTIRUSG_ACCOUNT_QQ      = 3, /* QQ�� qq account */
    ANTIRUSG_ACCOUNT_PHONE   = 4, /* �ֻ��� phone number */
    ANTIRUSG_ACCOUNT_SELF    = 5, /* �����ʺ� self account */
} AntiBrushAccountType;

/* ��ˢ������ */
typedef enum
{
    ANTIBRUSH_PROC_SUCC = 0, /*��ˢ�����ѯ�ɹ� */
    ANTIBRUSH_PROC_FAIL = 1, /* ��ˢ�����ѯʧ�� */
    ANTIBRUSH_PROC_TIMEOUT = 2, /* ��ˢ��ѯ��ʱ */
} TssSdkAntiBrushResultType;

/* ��һ�����Ϣ User Base Info */
typedef struct
{
    AntiBrushAccountType account_type_;     /* [in] �ʺ����� Account Type */
    unsigned char account_id_[TSS_SDK_ANTIBRUSH_MAX_ACCOUNT_ID_LEN]; /* [in] �������openid��΢������ʺţ�QQ�ţ��ֻ��� User Openid or QQ Account */
    unsigned int gameid_;                   /* [in] ��Ʒid����ȫ����䣩Game Product ID */
    unsigned int appid_;                    /* [in] ��ȫ����id����ȫ����䣩Application ID */
    unsigned int event_id_;                 /* [in] �id Event ID */
    unsigned int plat_;                     /* [in] �豸ƽ̨��0-IOS��1-Android */
    unsigned int clinet_ip_;                /* [in] �û�IP User Device IP */
    unsigned int area_;                     /* [in] �û�ƽ̨��1-΢�ţ�2-��Q wechat=1, mobile-qq=2*/
    unsigned int oidb_gameid_;              /* [in] oidb��Ϸid��ʹ����Qƽ̨��openidʱʹ�ã�������ϵ��ȫ���ṩ��Global Gameid */
} TssSdkAntiBrushUserInfo;

/* ���ҵ����Ϣ User Business Info */
typedef struct
{
    unsigned int cost_;                     /* [in] 30���ڸ��Ѽ�¼ */
    unsigned int fight_;                    /* [in] ս���� Fight */
    unsigned char ruid_[TSS_SDK_ANTIBRUSH_MAX_RUID_LEN]; /* [in] ��ɫ�� Role ID */
    unsigned int level_;                    /* [in] �ȼ� Level */
    unsigned int register_ts_;              /* [in] ע��ʱ��� Register Timestamp */
    unsigned int online_time_;              /* [in] ����ʱ�� Online Duration */
    unsigned int friend_count_;             /* [in] ������ Friends Count */
    unsigned char device_id_[TSS_SDK_ANTIBRUSH_MAX_DEVICE_ID_LEN]; /* [in] �豸ID Device ID */
} TssSdkAntiBrushBusiInfo;

/* ʹ���첽�ӿڻ�ȡ��ˢ����û�����Ĳ����ṹ�� */
/* Using an asynchronous interface to get anti-brush result of user query parameters structure */
typedef struct
{
    TssSdkAntiBrushUserInfo user_info_;     /* [in] ��һ�����Ϣ User Base Info */
    TssSdkAntiBrushBusiInfo busi_info_;     /* [in] ���ҵ����Ϣ User Business Info */
    unsigned int id_;                       /* [in] ����˻ش�id response id */
} TssSdkAntiBrushQueryInfo;

/* ʹ���첽�ӿڻ�ȡ��ˢ����û�����󷵻صĻص����������ṹ�� */
/* Using an asynchronous interface to determine the callback function returned
   to the anti-brush result of user input parameter structure */
typedef struct
{
    unsigned char account_id_[TSS_SDK_ANTIBRUSH_MAX_ACCOUNT_ID_LEN];
    /* [in] �������openid��΢������ʺţ�QQ�ţ��ֻ��� User Openid or QQ Account */
    unsigned int result_;                   /* [in] ����˴����� anti-brush result */
    unsigned int level_;                   /* [in] ��ˢ��� anti-brush result */
    unsigned int id_;                       /* [in] ����˻ش�id response id */
} TssSdkAntiBrushJudgeResultInfo;

/* �첽��ʽ��ѯ��ˢ�ӿ� */
/* Asynchronous way to query user anti-brush status input interface */
typedef TssSdkProcResult(*TssSdkAntiBrushJudgeQuery)(const TssSdkAntiBrushQueryInfo *input_info);

/* �첽��ʽ�·��ط�ˢ�ж�����Ļص����� */
/* Return to the anti-brush to determine the results of a callback function in the asynchronous mode */
typedef TssSdkProcResult(*TssSdkAntiBrushOnJudgeResult)(const TssSdkAntiBrushJudgeResultInfo *result_info);

typedef struct
{
    TssSdkAntiBrushJudgeQuery antibrush_judge_query_;
} TssSdkAntiBrushInterf;

typedef struct
{
    TssSdkAntiBrushOnJudgeResult on_antibrush_judge_result_;
} TssSdkAntiBrushInitInfo;

/* ��ȡ�û�������ƽӿ�

����˵��
- init_data���û�������ƽӿڳ�ʼ����Ϣ

����ֵ���ɹ�-�û�������ƽӿ���ָ�룬ʧ��-NULL
*/
/* Get user input control interface

Parameter Description
- init_data��User input control interface initialization information

return value��success-user input control interface pointer, failure-NULL
*/
#define TSS_SDK_GET_ANTIBRUSH_INTERF(init_info) \
    (const TssSdkAntiBrushInterf*)tss_sdk_get_busi_interf("tss_sdk_get_antibrush_interf", (const TssSdkAntiBrushInitInfo *)(init_info))

#pragma pack()

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif   /*TSS_SDK_ANTIBRUSH_H_*/

