/**
* @file:   tss_sdk_uic.h
* @brief:  This header file provides the interface of uic plugin.
* @copyright: 2011 Tencent. All Rights Reserved.
*/

#ifndef TSS_SDK_UIC_V2_H_
#define TSS_SDK_UIC_V2_H_

#include "tss_sdk.h"
#include "tss_sdk_uic.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

/* ʹ���첽�ӿ��ж�CHAT�����û�����Ĳ����ṹ�� */
/* Using an asynchronous interface to determine the CHAT type of user input parameters structure */
typedef struct
{
    TssSdkOpenid openid_;                   /* [in] �û�openid */
    unsigned char plat_id_;                 /* [in] plat_id, 0: IOS, 1: Android */
    unsigned int world_id_;                 /* [in] world id */
    UicMsgCategory msg_category_;           /* [in] ��Ϣ���ݵ�����ʼ�������� */
    unsigned int channel_id_;               /* [in] ���Ծ���Ƶ���� */
    unsigned int client_ip_;                /* [in] �ͻ���ip */
    TSS_UINT64 role_id_;                    /* [in] ��ɫ��� */
    unsigned int role_level_;               /* [in] ��ɫ�ȼ� */
    unsigned short role_name_len_;          /* [in] ��ɫ������ */
    unsigned char *role_name_;              /* [in] ��ɫ�� */
    unsigned int msg_len_;                  /* [in] ��Ϣ���ȣ��TSS_SDK_UIC_MAX_MESSAGE_LEN message length, maximum TSS_SDK_UIC_MAX_MESSAGE_LEN */
    unsigned char *msg_;                    /* [in] ��Ϣ���� message content */
    int door_level_;                        /* [in] �����ż����� */
    unsigned short callback_data_len_;      /* [in] �ش��ֶγ��� callback data length */
    unsigned char *callback_data_;          /* [in] �ش��ֶ����ݣ��TSS_SDK_UIC_MAX_CALLBACK_DATA_LEN callback data content, maximum TSS_SDK_UIC_MAX_CALLBACK_DATA_LEN */
    unsigned short game_data_len_;          /* [in] ��չ�ֶγ��� extension data length */
    unsigned char *game_data_;              /* [in] ��չ�ֶ����ݣ��TSS_SDK_UIC_MAX_GAME_DATA_LEN extension data content, max TSS_SDK_UIC_MAX_GAME_DATA_LEN */
} TssSdkUicChatUserInputInfoV2;

/* ʹ���첽�ӿ��ж�CHAT�����û�����󷵻صĻص����������ṹ�� */
/* Using an asynchronous interface to determine the callback function returned to the CHAT type of user input parameter structure */
typedef struct
{
    TssSdkOpenid openid_;                   /* [in] �û�openid */
    unsigned char plat_id_;                 /* [in] plat_id, 0: IOS, 1: Android */
    unsigned int world_id_;                 /* [in] world id */
    TSS_UINT64 role_id_;                    /* [in] role id */
    UicMsgResultFlag msg_result_flag_;      /* [in] �û����뾭��sdk���ж���������������ݽ�������Ƿ����� */
    /*      User input after the verdict of the sdk, developer to decide whether to shield in accordance with the results */
    int dirty_level_;                       /* [in] �����ִ������Ƽ��� */
    unsigned int msg_len_;                  /* [in] ��Ϣ���� message length */
    unsigned char *msg_;                    /* [in] ��Ϣ���ݣ��TSS_SDK_UIC_MAX_MESSAGE_LEN message content, max TSS_SDK_UIC_MAX_MESSAGE_LEN */
    unsigned short callback_data_len_;      /* [in] �ش��ֶγ��� callback data length */
    unsigned char *callback_data_;          /* [in] �ش��ֶ����ݣ��TSS_SDK_UIC_MAX_CALLBACK_DATA_LEN callback data content, maximum TSS_SDK_UIC_MAX_CALLBACK_DATA_LEN */
} TssSdkUicChatJudgeResultInfoV2;

/* ʹ��ͬ���ӿ��ж�NAME�����û�����Ĳ����ṹ�� */
/* Synchronous interface to determine the NAME type of user input parameters structure */
typedef struct
{
    unsigned int msg_len_;                  /* [in/out] ��Ϣ���� message length*/
    unsigned char *msg_;                    /* [in/out] ��Ϣ���ݣ��TSS_SDK_UIC_MAX_MESSAGE_LEN message content, max TSS_SDK_UIC_MAX_MESSAGE_LEN */
    int door_level_;                        /* [in] �����ż������������ע����1 */
    char if_replace_;                       /* [in] ����������дʣ��Ƿ��滻�ɡ�*��������ǣ���1���������0 */
    /*      If it contains sensitive words, whether to replace with the "*". If yes, fill in 1; if not, fill in 0*/
    UicMsgResultFlag msg_result_flag_;      /* [out] �û����뾭�����дʼ����ж���� User input after the verdict of the sensitive words check */
    int dirty_level_;                       /* [out] �����ִ������Ƽ��� */
} TssSdkUicNameUserInputInfoV2;

/* �첽��ʽ�ж�CHAT�����û�����ӿ� */
/* Asynchronous way to judge the type CHAT user input interface */
typedef TssSdkProcResult(*TssSdkUicJudgeUserInputChatV2)(const TssSdkUicChatUserInputInfoV2 *input_info);

/* �첽��ʽ�·���CHAT�����ж�����Ļص����� */
/* Return to the CHAT type to determine the results of a callback function in the asynchronous mode */
typedef TssSdkProcResult(*TssSdkUicChatOnJudgeResultV2)(const TssSdkUicChatJudgeResultInfoV2 *result_info);

/* ͬ����ʽ���NAME�����û�����ӿ� */
/* Synchronization check the NAME type of user input interface */
typedef TssSdkProcResult(*TssSdkUicJudgeUserInputNameV2)(TssSdkUicNameUserInputInfoV2 *input_info);

typedef struct
{

    TssSdkUicChatOnJudgeResultV2 on_chat_judge_result_;
} TssSdkUicInitInfoV2;

typedef struct
{
    TssSdkUicJudgeUserInputChatV2 judge_user_input_chat_;
    TssSdkUicJudgeUserInputNameV2 judge_user_input_name_;
} TssSdkUicInterfV2;


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
#define TSS_SDK_GET_UIC_INTERF_V2(init_data) \
    (const TssSdkUicInterfV2*)tss_sdk_get_busi_interf("tss_sdk_get_uic_interf_v2", (const TssSdkUicInitInfoV2 *)(init_data))

#pragma pack()

#ifdef __cplusplus
} /* end of extern "C" */
#endif

// ���ݾɰ汾�Ľӿ�
#include "tss_sdk_uic_v1.h"

#endif   /*TSS_SDK_PLUGIN_UIC_H_*/

