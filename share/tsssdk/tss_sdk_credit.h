/**
* @file:   tss_sdk_credit.h
* @brief:  This header file provides the interface of credit plugin.
* @copyright: 2017 Tencent. All Rights Reserved.
*/

#ifndef TSS_SDK_CREDIT_H_
#define TSS_SDK_CREDIT_H_

#include "tss_sdk.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

/* ���öȲ�ѯ������ */
typedef enum
{
    CREDIT_PROC_SUCC = 0,       /* ���öȲ�ѯ�ɹ� */
    CREDIT_PROC_FAIL = 1,       /* ���öȲ�ѯʧ�� */
    CREDIT_PROC_TIMEOUT = 2,    /* ���öȲ�ѯ��ʱ */
} TssSdkCreditResultType;

/* ʹ���첽�ӿڻ�ȡ��ˢ����û�����Ĳ����ṹ�� */
/* Using an asynchronous interface to get anti-brush result of user query parameters structure */
typedef struct
{
    unsigned int plat_id_;            /* [in] ƽ̨ platid,0-IOS,1-Android,255-all */
    unsigned char openid[64];         /* [in] �û�openid */
    unsigned int id_;                 /* [in] ����˻ش�id response id */
} TssSdkCreditQueryInfo;

/* ʹ���첽�ӿڻ�ȡ��ˢ����û�����󷵻صĻص����������ṹ�� */
/* Using an asynchronous interface to determine the callback function returned
   to the anti-brush result of user input parameter structure */
typedef struct
{
    unsigned int version_;            /* [out] �汾�� version */
    int result_;                      /* [out] ��ѯ��� result */
    unsigned int data_time_;          /* [out] ����ʱ�� update time(UNIX format) */
    unsigned char openid[64];         /* [out] �û�openid */
    unsigned int plat_id_;            /* [out] ƽ̨ platid,0-IOS,1-Android,255-all */
    unsigned int payment_;            /* [out] ���� payment */
    unsigned int score_;              /* [out] ���� score(0-600) */
    unsigned int rank_;               /* [out] ���� rank(0-100) */
    unsigned int stars_;              /* [out] �Ǽ� stars(1-7) */
    unsigned int id_;                 /* [out] ����˻ش�id response id */
} TssSdkCreditJudgeResultInfo;

typedef struct
{
    unsigned int plat_id_;            /* [in] platid,0-IOS,1-Android,255-all */
    unsigned char openid[64];         /* [in] openid */
    unsigned int seq_;                /* [in] seq */
} TssSdkPluginCreditQuery;


/* �첽��ʽ��ѯ���öȽӿ� */
/* Asynchronous way to query user credit status input interface */
typedef TssSdkProcResult(*TssSdkCreditJudgeQuery)(const TssSdkCreditQueryInfo *input_info);

/* �첽��ʽ�·������ö��ж�����Ļص����� */
/* Return to the credit to determine the results of a callback function in the asynchronous mode */
typedef TssSdkProcResult(*TssSdkCreditOnJudgeResult)(const TssSdkCreditJudgeResultInfo *result_info);

typedef struct
{
    TssSdkCreditJudgeQuery credit_judge_query_;
} TssSdkCreditInterf;

typedef struct
{
    TssSdkCreditOnJudgeResult on_credit_judge_result_;
} TssSdkCreditInitInfo;

/* ��ȡ���öȲ�ѯ�ӿ�

����˵��
- init_data�����öȲ�ѯ�ӿڳ�ʼ����Ϣ

����ֵ���ɹ�-���öȲ�ѯ�ӿ���ָ�룬ʧ��-NULL
*/
/* Get credit query interface

Parameter Description
- init_data��Credit query interface initialization information

return value��success-credit interface pointer, failure-NULL
*/
#define TSS_SDK_GET_CREDIT_INTERF(init_info) \
    (const TssSdkCreditInterf*)tss_sdk_get_busi_interf("tss_sdk_get_credit_interf", (const TssSdkCreditInitInfo *)(init_info))

#pragma pack()

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif   /*TSS_SDK_CREDIT_H_*/

