/**
 * @file:   tss_sdk.h
 * @brief:  This header file provides the interface of TSS SDK.
 * @copyright: 2012 Tencent. All Rights Reserved.
 */

#ifndef TSS_SDK_H__
#define TSS_SDK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if (defined(WIN32) || defined(_WIN64))

#include <tchar.h>

#if _MSC_VER >= 1300
typedef unsigned long long  TSS_UINT64;
typedef long long   TSS_INT64;
#else /* _MSC_VER */
typedef unsigned __int64    TSS_UINT64;
typedef __int64 TSS_INT64;
#endif /* _MSC_VER */
typedef TCHAR TSS_TCHAR;

#else // (defined(WIN32) || defined(_WIN64))

#include <stdint.h>
typedef uint64_t TSS_UINT64;
typedef int64_t TSS_INT64;
typedef char TSS_TCHAR;

#endif

/* �ӿ�ͨ�ô����� */
/* Interface general processing results */
typedef enum
{
    TSS_SDK_PROC_OK = 0,             /* ����ɹ� Processing Successfully */
    TSS_SDK_PROC_INVALID_ARG = 1,    /* ��Ч���� Invalid parameters */
    TSS_SDK_PROC_INTERNAL_ERR = 2,   /* �ڲ����� Internal error */
    TSS_SDK_PROC_FAIL = 3,           /* ����ʧ�� Processing failed */
} TssSdkProcResult;

/* callback function of sdk proc */
typedef int (*TssSdkProc)();

/* SDK interface */
typedef struct
{
    /* tss_sdk �ڲ����ݴ���������Ҫ��Ϸ��ʱ���ã�
    Ƶ��������Ҫ�ﵽ10��/s����������ֻ����һ�������� */
    /* tss_sdk internal data processing function which gamesvr calls timely
    with frequency of at least 10 times/s, only be called by one place of the whole process */
    TssSdkProc proc_;
} TssSdkBusiInterf;

/*openid of the role*/
typedef struct
{
    unsigned char *openid_;
    unsigned char openid_len_;
} TssSdkOpenid;

typedef struct
{
    /*
    ������tss_sdk�Ľ���ʵ��id,
    �������id��sdk����˵�ͨѶ��أ�����
    ���ͬһ̨�������ж�����̼�����tss_sdk����Ҫ��������������
    1. ��ͬ���̴����id��һ����
    2. ͬһ����������ʱ���ʵ��idҲ��Ҫ�������ϴ�һ��
    */
    /*
    Load process tss_sdk entity id,
    because this id is related with the sdk to the back-end communication
    On the same machine multiple process load tss_sdk need to meet two conditions:
    1. Different process passes different id
    2. The process also need to maintain consistency with the previous entity id when restart.
    */
    unsigned int unique_instance_id_;
    /*
    tss_sdk������·��
    */
    const char *tss_sdk_conf_;
} TssSdkInitInfo;

/*
*
* @fn     tss_sdk_load
* @brief  ����sdk load sdk
*
* @param  shared_lib_dir [in] tss_sdk ��̬��Ŀ¼ tss_sdk Dynamic library directory
* @param  init_info [in] sdk ��ʼ������ sdk Initialization parameters
*
* @return NULL --����ʧ�ܣ�������Ŀ¼���ԣ�����sdk��ʼ��ʧ�� load failed, maybe it is the wrong directory, or sdk failed to initialize
*         ��NULL --���سɹ����뱣�����صĵ�ַ���������proc�ӿ���Ҫ load successfully, Please keep the return address, the following needs it to call proc interface
*/
const TssSdkBusiInterf *tss_sdk_load(const TSS_TCHAR *shared_lib_dir,
                                     const TssSdkInitInfo *init_info);
/* SDK unload function */
int tss_sdk_unload();

/*
��ȡsdk��ҵ��ӿڽṹ�� �˺�����Ҫֱ��ʹ�ã�
ҵ��Ľӿڻ�ȡֱ��ʹ��ҵ���ṩ�ĺ꣬����TSS_SDK_GET_XX_INTERF��
*/
/*
Get the sdk service interface structure, this function is not used directly,
use the business macri to obtain business interface, similar with TSS_SDK_GET_XX_INTERF
*/
const void *tss_sdk_get_busi_interf(const char *syml_name,
                                    const void *data);


/*
�û���չ����

��ǰ�İ汾sdk�ڵ��ûص�������ʱ��,ֻ�ᴫ��openid��ҵ��svr

���ǲ���ҵ��svr�ڲ���ʹ���Լ���uid��ʶ�û���,Ϊ�˽�sdk����
�ڼ���һ��openid���ڲ�uid��ӳ���

Ϊ�˽���ҵ��svr�Ĺ�����,��sdk�ӿ��϶������һ���û���չ����,
ҵ����԰��ڲ�uid�ŵ�����,���ڵ���sdk�ӿ�ʱ���ݸ�sdk,
��sdk����ҵ��svr�ṩ�Ļص�����ʱ,�Ὣ��������openidһ�𴫵�
��ҵ��svr,ҵ��svr����ʹ�ø����������ٲ���


ע��: ���uid�����ڲ�ͬ��openid�临�õĿ���,
��ô�ڸ�����չ���ݲ��ҵ��û���Ϣ��,Ӧ���ٺ˶�һ��openid,ȷ�����������������û�
*/

/*������������SDK�ؼ�����*/
#define TSS_SDK_USER_EXT_DATA_MAX_LEN 1024

typedef struct
{
    /*
        �û���չ����
    */
    char user_ext_data_[TSS_SDK_USER_EXT_DATA_MAX_LEN];
    /*
        ��չ�������ݵĳ���
    */
    unsigned int ext_data_len_;
} TssSdkUserExtData;



#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* end of TSS_SDK_H__ */

