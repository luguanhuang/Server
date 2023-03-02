#include "pch.h"
#include "MsTssHandler.h"
#include "util.h"
#include "timeutil.h"
#include "config.h"

INSTANCE_SINGLETON(CTssHandler)

CTssHandler::CTssHandler()
{
	last_time = TimeUtil::GetMilliSecond();
}

CTssHandler::~CTssHandler()
{

}

bool CTssHandler::Init()
{
	int ret = init_anti_interf();
	if (ret != 0)
	{
		printf("init anti interface fail. err=%d", ret);
		return false;
	}

	return true;
}

void CTssHandler::Uninit()
{
	tss_sdk_unload();
	return;
}
INT32 CTssHandler::on_timer()
{
	if (NULL == busi_interf_)
	{
		return -1;
	}
	static const unsigned int PROC_INTERVAL_MILSECOND = 50;
	if ( TimeUtil::GetMilliSecond() - last_time >= PROC_INTERVAL_MILSECOND)
	{
		busi_interf_->proc_();
		last_time = TimeUtil::GetMilliSecond();
	}

	return 0;
}
INT32 CTssHandler::init_anti_interf()
{
	// get sdk driver interface handle busi_interf_
	TssSdkInitInfo init_data;
	//必须保证每个服务器的id不一样
	init_data.unique_instance_id_ = MSConfig::Instance()->GetServerID();//用区服ID来区分,使用config保证不重复即可;
	init_data.tss_sdk_conf_ = "gsconf/serveronly";//sdk 配置的路径
	std::string path = "./gsconf/";
	busi_interf_ = tss_sdk_load(path.c_str(), &init_data);
	if (busi_interf_ == NULL)
	{
		LogError("load tss sdk interface fail.");
		return -1;
	}

	// get Uic service interface handle uic_interf_
	TssSdkUicInitInfoV2 uic_init_info;
	memset(&uic_init_info, 0, sizeof(uic_init_info));
	uic_init_info.on_chat_judge_result_ = uic_chat_on_judge;

	uic_interf_ = TSS_SDK_GET_UIC_INTERF_V2(&uic_init_info);
	if (uic_interf_ == NULL)
	{
		LogError("get uic_interf interf fail.");
		return -4;
	}
	return 0;
}
TssSdkProcResult uic_chat_on_judge(const TssSdkUicChatJudgeResultInfoV2 *result_info)
{
	if (!result_info)
	{
		LogError("tx sdk uic char judge result return null.");
		return TSS_SDK_PROC_FAIL;
	}
	return TSS_SDK_PROC_OK;
}
INT32 CTssHandler::judge_user_input_name(TssSdkUicNameUserInputInfoV2* info)
{	
	if (NULL == uic_interf_)
	{
		return -1;
	}

	TssSdkProcResult ret = uic_interf_->judge_user_input_name_(info);
	if (ret != TSS_SDK_PROC_OK)
	{
		LogError("judge_user_input_name_ failed, ret: %d", ret);
		return -1;
	}
	return 0;
}

KKSG::ErrorCode CTssHandler::ValidNameCheck(std::string Name)
{
	TssSdkUicNameUserInputInfoV2 info;
	memset(&info, 0, sizeof(info));
	info.msg_ = (unsigned char*)Name.c_str();
	info.msg_len_ = Name.length();
	info.door_level_ = 1;
	info.if_replace_ = 0;
	if (0 == judge_user_input_name(&info))
	{
		if (info.msg_result_flag_ != MSG_NORMAL_FLAG)
		{
			return KKSG::ERR_WORD_FORBID;
		}
	}
	return KKSG::ERR_SUCCESS;
}
