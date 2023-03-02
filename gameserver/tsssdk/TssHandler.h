#ifndef _TSS_HANDLER_H__
#define _TSS_HANDLER_H__
#include "tsssdk/tss_sdk_anti.h"
#include "tsssdk/tss_sdk_secoptverify.h"
#include "tsssdk/tss_sdk_uic.h"
#include "unit/role.h"
#include <time.h>

class TssHandler
{
public:
	TssHandler();
	~TssHandler();
	DECLARE_SINGLETON(TssHandler);

public:
	bool Init();
	void Uninit();
	INT32 init_anti_interf();
	INT32 on_timer();

	INT32 anti_add_user(Role* role);
	INT32 anti_del_user(Role* role);
	INT32 on_recv_anti_data(Role* role, const unsigned char* anti_data, UINT32 anti_data_len);

	INT32 sensitive_query_verify(Role* role, Role* fromrole, UINT32 ItemID, UINT32 ItemCount);

	//同步判断名字，
	INT32 judge_user_input_name(TssSdkUicNameUserInputInfoV2* info);
	//异步判断输入
	INT32 judge_user_input_chat(Role* role, UicMsgCategory utype, UINT32 channer_id, INT32 door_level_, const char* content, UINT32 uChatID);

	const TssSdkBusiInterf* GetBusiInterf(){return busi_interf_;}
	const TssSdkAntiInterfV3* GetAntiInterf(){return anti_interf_;}
	const TssSdkSensitiveInterf* GetSensitiveInterf(){return sens_interf_;}
	const TssSdkUicInterfV2* GetUicInterf(){return uic_interf_;}

private:
	const TssSdkBusiInterf *busi_interf_;//内部数据处理函数
	const TssSdkAntiInterfV3 *anti_interf_;
	const TssSdkSensitiveInterf *sens_interf_;
	const TssSdkUicInterfV2 *uic_interf_;

	UINT64 last_time;
	UINT32 seq;
	//GsUserMgr user_mgr_;
};

TssSdkProcResult on_send_data_to_client(const TssSdkAntiSendDataInfoV3 *send_data_info);
TssSdkProcResult uic_chat_on_judge(const TssSdkUicChatJudgeResultInfoV2 *result_info);
#endif
