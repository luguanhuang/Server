#include "pch.h"
#include "TssHandler.h"
#include "util.h"
#include "timeutil.h"
#include "config.h"
#include "unit/rolemanager.h"
#include "tsssdk/ptcg2c_tsssdkantidatantf.h" 

INSTANCE_SINGLETON(TssHandler)
TssHandler::TssHandler()
{
	busi_interf_ = NULL;
	anti_interf_ = NULL;
	sens_interf_ = NULL;
	uic_interf_ = NULL;
	seq = 0;
	last_time = TimeUtil::GetMilliSecond();
}
TssHandler::~TssHandler()
{
}

bool TssHandler::Init()
{
	int ret = init_anti_interf();
	if (ret != 0)
	{
		printf("init anti interface fail. err=%d", ret);
		return false;
	}

	return true;
}
void TssHandler::Uninit()
{
	tss_sdk_unload();
	return;
}

INT32 TssHandler::on_timer()
{
	if (NULL == busi_interf_)
	{
		return -1;
	}
	static const unsigned int PROC_INTERVAL_MILSECOND = 10;
	if ( TimeUtil::GetMilliSecond() - last_time >= PROC_INTERVAL_MILSECOND)
	{
		busi_interf_->proc_();
		last_time = TimeUtil::GetMilliSecond();
	}

	return 0;
}
INT32 TssHandler::init_anti_interf()
{
	// get sdk driver interface handle busi_interf_
	TssSdkInitInfo init_data;
	//必须保证每个服务器的id不一样
	init_data.unique_instance_id_ = GSConfig::Instance()->GetServerID() * 100 + GSConfig::Instance()->GetLine();//用区服ID来区分,使用gsconfig保证不重复即可;
	init_data.tss_sdk_conf_ = "gsconf/serveronly";//sdk 配置的路径
	busi_interf_ = tss_sdk_load("./gsconf/", &init_data);
	if (busi_interf_ == NULL)
	{
		LogError("load tss sdk interface fail.");
		return -1;
	}

	// get antihacking service interface handle anti_interf_
	TssSdkAntiInitInfoV3 init_info;
	init_info.send_data_to_client_ = on_send_data_to_client;
	anti_interf_ =  TSS_SDK_GET_ANTI_INTERF_V3(&init_info);
	if (anti_interf_ == NULL)
	{
		LogError("get anti interface fail.\n");
		return -2;
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
INT32 TssHandler::anti_add_user(Role* role)
{
	if (NULL == anti_interf_ || NULL == role)
	{
		return -1;
	}
	TssSdkAntiAddUserInfoV3 user_info;
	memset(&user_info, 0, sizeof(user_info));
	user_info.openid_.openid_ = (unsigned char *)role->GetAccount().c_str();
	user_info.openid_.openid_len_ = strlen((const char *)user_info.openid_.openid_);
	//platid, 0: IOS, 1: Android
	user_info.plat_id_ = role->GetClientConf().platid();
	user_info.world_id_ = role->GetServerId();
	user_info.role_id_ = role->GetRoleID();

	TssSdkUserExtData user_ext_data;
	memset(&user_ext_data, 0, sizeof(user_ext_data));
	user_info.user_ext_data_ = &user_ext_data;

	user_info.client_ip_ = inet_addr(role->GetClientConf().ip().c_str());
	std::stringstream version;
	version << "1"; 
	std::string ver = role->GetClientConf().clientversion();
	for (UINT32 i = 0; i < ver.length(); i ++)
	{
		if (ver[i] >= '0' && ver[i] <= '9')
		{
			version << ver[i];
			if (version.str().length() >= 10)
			{
				break;
			}
		}
	}
	user_info.client_ver_ = convert<UINT32>(version.str());
	user_info.role_name_ = role->GetName().c_str();
	TssSdkProcResult ret = anti_interf_->add_user_(&user_info);
	if (ret != TSS_SDK_PROC_OK)
	{
		LogError("anti add user fail. err=%d", ret);
		return -1;
	}
	return 0;
}
INT32 TssHandler::anti_del_user(Role* role)
{
	if (NULL == anti_interf_ || NULL == role)
	{
		return -1;
	}
	TssSdkAntiDelUserInfoV3 user_info;
	user_info.openid_.openid_ = (unsigned char *)role->GetAccount().c_str();
	user_info.openid_.openid_len_ = strlen((const char *)user_info.openid_.openid_);
	//platid, 0: IOS, 1: Android
	user_info.plat_id_ = role->GetClientConf().platid();
	user_info.world_id_ = role->GetServerId();
	user_info.role_id_ = role->GetRoleID();

	TssSdkUserExtData user_ext_data;
	memset(&user_ext_data, 0, sizeof(user_ext_data));
	user_info.user_ext_data_ = &user_ext_data;

	TssSdkProcResult ret = anti_interf_->del_user_(&user_info);
	if (ret != TSS_SDK_PROC_OK)
	{
		LogError("anti_del_user fail. ret=%d openid=%s",
			ret, user_info.openid_.openid_);
		return -1;
	}

	return 0;
}
INT32 TssHandler::on_recv_anti_data(Role* role, const unsigned char* anti_data, UINT32 anti_data_len)
{
	if (NULL == anti_interf_ || NULL == role)
	{
		return -1;
	}
	TssSdkAntiRecvDataInfoV3 pkg_info;
	pkg_info.openid_.openid_ = (unsigned char *)role->GetAccount().c_str();
	pkg_info.openid_.openid_len_ = strlen((const char *)pkg_info.openid_.openid_);
	//platid, 0: IOS, 1: Android
	pkg_info.plat_id_ = role->GetClientConf().platid();
	pkg_info.world_id_ = role->GetServerId();
	pkg_info.role_id_ = role->GetRoleID();

	TssSdkUserExtData user_ext_data;
	memset(&user_ext_data, 0, sizeof(user_ext_data));
	pkg_info.user_ext_data_ = &user_ext_data;

	pkg_info.anti_data_ = anti_data;
	pkg_info.anti_data_len_ = anti_data_len;

	TssSdkProcResult proc_result = anti_interf_->recv_anti_data_(&pkg_info);

	return 0;
}

INT32 TssHandler::judge_user_input_name(TssSdkUicNameUserInputInfoV2* info)
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
INT32 TssHandler::judge_user_input_chat(Role* role, UicMsgCategory utype, UINT32 channer_id, INT32 door_level_, const char* content, UINT32 uChatID)
{
	if (NULL == uic_interf_ || NULL == role)
	{
		return -1;
	}
	TssSdkUicChatUserInputInfoV2 input_info;
	memset(&input_info, 0, sizeof(input_info));
	input_info.openid_.openid_ = (unsigned char *)role->GetAccount().c_str();
	input_info.openid_.openid_len_ = strlen((const char *)input_info.openid_.openid_);
	input_info.plat_id_ = role->GetClientConf().platid();
	input_info.world_id_ = role->GetServerId();
	input_info.msg_category_ = utype;
	input_info.channel_id_ = channer_id;
	input_info.client_ip_ = 0;
	input_info.role_id_ = role->GetRoleID();
	input_info.role_level_ = role->GetLevel();
	input_info.role_name_len_ = role->GetName().length();
	input_info.role_name_ = (unsigned char*)role->GetName().c_str();
	input_info.msg_ = (unsigned char*)content;
	input_info.msg_len_ = strlen(content);
	input_info.door_level_ = door_level_;
	std::string str = ToString<UINT32>(uChatID);
	input_info.callback_data_ = (unsigned char*)str.c_str();
	input_info.callback_data_len_ = strlen((const char *)input_info.callback_data_);
	uic_interf_->judge_user_input_chat_(&input_info);
	return 0;
}
TssSdkProcResult on_send_data_to_client(const TssSdkAntiSendDataInfoV3 *send_data_info)
{
	if (NULL == send_data_info)
	{
		LogError("send_data_info is null");
		return TSS_SDK_PROC_FAIL;
	}
	Role* pRole = RoleManager::Instance()->FindByRoleID(send_data_info->role_id_);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, role id:"<<send_data_info->role_id_<<END;
		return TSS_SDK_PROC_FAIL;
	}

	PtcG2C_TssSdkAntiDataNtf ntf;
	std::string data((char *)send_data_info->anti_data_, send_data_info->anti_data_len_);
	ntf.m_Data.set_anti_data(data);
	ntf.m_Data.set_anti_data_len(send_data_info->anti_data_len_);
	pRole->Send(ntf);

	return TSS_SDK_PROC_OK;
}

TssSdkProcResult uic_chat_on_judge(const TssSdkUicChatJudgeResultInfoV2 *result_info)
{
	if (!result_info)
	{
		LogError("tx sdk uic char judge result return null.");
		return TSS_SDK_PROC_FAIL;
	}
	if (result_info->msg_result_flag_ != MSG_EVIL_FLAG)//
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(result_info->role_id_);
		if (NULL == pRole)
		{
			SSWarn<<"pRole is NULL, role id:"<<result_info->role_id_<<END;
			return TSS_SDK_PROC_FAIL;
		}
		//验证完毕后，发送该聊天信息
		UINT32 uChatID = convert<UINT32>((char *)result_info->callback_data_);
		pRole->SendChatToMS(result_info->msg_, result_info->msg_len_, uChatID);
	}
	return TSS_SDK_PROC_OK;
}
