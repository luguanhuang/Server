#ifndef __MS_TSS_HANDLER_H__
#define __MS_TSS_HANDLER_H__
#include "tsssdk/tss_sdk_anti.h"
#include "tsssdk/tss_sdk_secoptverify.h"
#include "tsssdk/tss_sdk_uic.h"
#include <time.h>

class CTssHandler
{
public:
	CTssHandler();
	~CTssHandler();
	DECLARE_SINGLETON(CTssHandler)

public:
	bool Init();
	void Uninit();
	INT32 init_anti_interf();
	INT32 on_timer();

	//同步判断名字，
	INT32 judge_user_input_name(TssSdkUicNameUserInputInfoV2* info);
	KKSG::ErrorCode ValidNameCheck(std::string Name);

private:
	const TssSdkBusiInterf *busi_interf_;//内部数据处理函数
	const TssSdkUicInterfV2 *uic_interf_;
	UINT64 last_time;
};
TssSdkProcResult uic_chat_on_judge(const TssSdkUicChatJudgeResultInfoV2 *result_info);
#endif
