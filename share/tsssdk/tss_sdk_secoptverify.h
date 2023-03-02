/*
 @copyright  2004-2015  Apache License, Version 2.0
 @filename   tss_sdk_secoptverify.h
 @author     yunfeiyang
 @version
 @date       2016/03/16 11:17
 @brief
 @details    2016/03/16 yunfeiyang create
*/
#ifndef TSS_SDK_SENSITIVE_H_
#define TSS_SDK_SENSITIVE_H_

#include "tss_sdk.h"

#ifdef __cplusplus
extern "C"
{
#endif /* end of __cplusplus */

#define TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN  1024        /* �����Ϣ���� Maximum message length */

#pragma pack(1)

/* ���в�������̬�û�������Ϣ */
typedef struct
{
    /* �û�openid */
    unsigned char openid_[64];
    /* �û�����ϵͳ���ͣ�0-IOS��1-Android */
    unsigned int plat_id_;
    /* �û������ţ�û����0 */
    unsigned int world_id_;
} TssSdkSensitiveUserInfoBase;

/* ��ȫ�������� */
typedef enum
{
    TYPE_SAFE_MODE_CONTROL = 1,         // ����̬����
    TYPE_OTHER = 99,                    // ��������
} TssSdkSecurityType;


/* ����̬���ƽ�� */
typedef enum
{
    SECURITY_PASSED = 0,                   // ����ֱ̬�ӷŹ�
    SECURITY_NEED_SET_SAFE_MODE = 1,       // ���ñ���̬
    SECURITY_IN_LIMITED_TIME = 2,          // �����ڸյ�¼��������ڣ���ʱ�������κ����в���
} TssSdkSecurityResultType;


/* ���ֻ������־ */
typedef enum
{
    BIND_PHONE_NO = 0,         // δ���ֻ�����
    BIND_PHONE_YES = 1,        // �Ѱ��ֻ�����
    BIND_PHONE_UNKNOWN = 2,    // ��ȷ���Ƿ�����ֻ�����
} TssSdkSensitiveBindPhoneType;


/* ����̬״̬ */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* [out] ��ȫ�������� */
    TssSdkSecurityType security_type_;
    /* [out] ��ȫ���ƽ�� */
    TssSdkSecurityResultType security_result_;
    /* [out] �Ƿ���Ѱ��ֻ��ţ�0-δ�󶨣�1-�Ѱ� */
    TssSdkSensitiveBindPhoneType bind_phone_num_flag_;
    /* [out] �������ͣ� ����ֵ��Ӧ�ĵ���ui�ɲ�Ʒ�������Ϸ��Ŀ��Э�� */
    int pop_up_type_;
    /* [in\out] ������Ϣ���ݣ��TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN; message content, max TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN */
    char msg_[TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN];
} TssSdkSensitiveSafeModeInfo;

/* ͬ����ʽ��ѯ�û�����̬״̬��Ϣ�ӿ� */
typedef TssSdkProcResult(*TssSdkSensitiveQuerySensitiveSafeMode)(TssSdkSensitiveSafeModeInfo *input_info);


/* ���Ͷ�����֤����̬ */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* ��½�Ŀͻ���IP */
    char client_ip_[20];
} TssSdkSensitiveSMSInfo;

/* ���в������Ͷ�����֤����ӿ� */
typedef TssSdkProcResult(*TssSdkSensitiveSendSMSReq)(const TssSdkSensitiveSMSInfo *sms_request);


/* ������֤������Ϣ */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* [in] �ش�sequence���˴�������д������ˮ�� */
    unsigned int seq_;
    /* [in] ������֤������ */
    char sms_serial_[16];
} TssSdkSensitiveSMSVerifyInfo;

/* ���Ͷ�����֤����ӿ� */
typedef TssSdkProcResult(*TssSdkSensitiveSMSVerifyReq)(const TssSdkSensitiveSMSVerifyInfo *verify_request);


/* ������֤������� */
typedef enum
{
    SMS_RESULT_VERIFY_PASSED = 0,           // ������֤ͨ��
    SMS_RESULT_NO_BING_PHONE = 1,           // �û�û�а��ֻ�
    SMS_RESULT_VERIFY_UNPASSED = 2,         // ��֤��ͨ��
    SMS_RESULT_TIMEOUT = 3,                 // ��ʱ���ڲ�����
} TssSdkSensitiveSMSResultType;

/* ��֤����ͨ�������û����ֻ���֪ͨ���� */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* [in] �ش�sequence���˴�������д������ˮ�� */
    unsigned int seq_;
    /* [in] ������֤��� */
    TssSdkSensitiveSMSResultType sms_result_;
    /* [in] �������� */
    int pop_up_type_;
    /* [in] ������Ϣ���ݣ��TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN; message content, max TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN */
    char msg_[TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN];
} TssSdkSensitiveSMSResult;

/* ��Ϸ�������Ҫʵ�ֵĻ�ȡ������֤����Ļص����� */
typedef TssSdkProcResult(*TssSdkSensitiveSetSMSVerifyResult)(const TssSdkSensitiveSMSResult *verify_result);


/* ���ֻ�����������Ϣ */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* [in] �ش�sequence���˴�������д������ˮ�� */
    unsigned int seq_;
    /* [in] �ֻ����� */
    char mobile_phone_number_[16];
} TssSdkSensitiveBindPhoneNumInfo;

/* ���ֻ��������� */
typedef TssSdkProcResult(*TssSdkSensitiveBindPhoneNumReq)(const TssSdkSensitiveBindPhoneNumInfo *bind_request);


/* ���ֻ����������� */
typedef enum
{
    BIND_RESULT_SUCCESS = 0,           // ���ֻ�����ɹ�
    BIND_RESULT_FAILED = 1,            // ���ֻ�����ʧ��
    BIND_RESULT_TIMEOUT = 2,           // ��ʱ���ڲ�����
} TssSdkSensitiveBindPhoneResultType;

/* ���ֻ������� */
typedef struct
{
    /* [in] �û�������Ϣ */
    TssSdkSensitiveUserInfoBase user_info_;
    /* [in] �ش�sequence���˴�������д������ˮ�� */
    unsigned int seq_;
    /* [in] ���ֻ������� */
    TssSdkSensitiveBindPhoneResultType bind_result_;
    /* [in] �������� */
    int pop_up_type_;
    /* [in] ������Ϣ���ݣ��TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN; message content, max TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN */
    char msg_[TSS_SDK_SAFEMODE_MAX_MESSAGE_LEN];
} TssSdkSensitiveBindPhoneNumResult;

/* ��Ϸ�������Ҫʵ�ֵĻ�ȡ������֤����Ļص����� */
typedef TssSdkProcResult(*TssSdkSensitiveSetBindPhoneNumResult)(const TssSdkSensitiveBindPhoneNumResult *bind_result);


/* ���в����ӿ� */
typedef struct
{
    /* �����ѯ��ҵı���̬״̬��Ϣ */
    TssSdkSensitiveQuerySensitiveSafeMode query_safe_mode_info_;

    /* ������ֻ����� */
    TssSdkSensitiveBindPhoneNumReq req_bind_phone_num_;

    /* ���������в���������֤�� */
    TssSdkSensitiveSendSMSReq req_send_sms_;

    /* ����Զ�����֤�����У�� */
    TssSdkSensitiveSMSVerifyReq req_verify_sms_;
} TssSdkSensitiveInterf;

/* ���в���ͳһ��֤�ӿڳ�ʼ������ */
typedef struct
{
    /* �����ֻ�����󶨽�� */
    TssSdkSensitiveSetBindPhoneNumResult set_bind_phone_num_result_;

    /* ���ö�����֤��У���� */
    TssSdkSensitiveSetSMSVerifyResult set_sms_verify_result_;
} TssSdkSensitiveInitData;

#define TSS_SDK_GET_SENSITIVE_INTERF(init_data) \
    (const TssSdkSensitiveInterf*)tss_sdk_get_busi_interf("tss_sdk_get_sensitive_interf", (const TssSdkSensitiveInitData *)(init_data))


#pragma pack()

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif // TSS_SDK_SENSITIVE_H_

