#include "pch.h"
#include "lottery/rpcc2g_getplatformsharechest.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/lotteryrecord.h"
#include "httptask/httptaskmgr.h"
#include "httptask/queryplatformsharechestid.h"
#include "unit/systeminfo.h"
#include "httptask/querywebchatsharechestid.h"
// generate by ProtoGen at date: 2017/4/11 16:26:35

RPC_SERVER_IMPLEMETION(RpcC2G_GetPlatformShareChest, GetPlatformShareChestArg, GetPlatformShareChestRes)

	void RpcC2G_GetPlatformShareChest::OnCall(const GetPlatformShareChestArg &roArg, GetPlatformShareChestRes &roRes)
{
	roRes.set_errorcode(KKSG::ERR_PLATSHARE_FAILED);
	Role *role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{	
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);		
		return;
	}

	// 系统是否已开
	if(!role->Get<CRoleSystem>()->IsSystemOpened(SYS_PLATFORMSHARE_BOX))
	{
		return;
	}

	if (!role->Get<LotteryRecord>()->IsHaveShareGiftCount())
	{
		return;
	}

	bool exist_act =  false;
	UINT32 act_id = 0;
	if (role->GetClientConf().logintype() == KKSG::LOGIN_QQ_PF)
	{
		UINT32 num = 0;
		UINT32 people_num = 0;
		auto chest_info = GetGlobalConfig().OpenCGIChest;
		for (auto it = chest_info.begin(); it != chest_info.end(); ++it)
		{
			if (it->seq[0] ==  roArg.box_id())
			{
				act_id = it->seq[1];
				num = it->seq[2];
				people_num = it->seq[3];

				roRes.set_act_id(act_id);
				exist_act = true;
			}
		}

		if (!exist_act)
		{
			return;
		}

		QueryPlatformShareChestid* task = new QueryPlatformShareChestid();			
		task->SetRPCID(DelayRpc());	
		task->SetOpenID(role->GetAccount());
		task->SetToken(roArg.open_key());
		task->SetPf(role->GetClientConf().pf());
		task->SetActId(act_id);
		task->SetNum(num);
		task->SetPeopleNum(people_num);
		task->BuildPostData();
		HttpTaskMgr::Instance()->AddTask(task);
	}
	else if (role->GetClientConf().logintype() == KKSG::LGOIN_WECHAT_PF)
	{		
		auto chest_info = GetGlobalConfig().OpenWebChatChest;
		for (auto it = chest_info.begin(); it != chest_info.end(); ++it)
		{
			if (it->seq[0] ==  roArg.box_id())
			{				
				roRes.set_act_id(act_id);
				exist_act = true;
			}
		}

		if (!exist_act)
		{
			return;
		}

		QueryWebChatShareChestid *task = new QueryWebChatShareChestid();
		task->SetRPCID(DelayRpc());
		task->SetOpenID(role->GetAccount());
		/*task->SetActId(act_id);
		task->SetTypeId(key_id);
		task->SetBussid(bussid);*/
		HttpTaskMgr::Instance()->AddTask(task);
	}
}

void RpcC2G_GetPlatformShareChest::OnDelayReplyRpc(const GetPlatformShareChestArg &roArg, GetPlatformShareChestRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ERR_PLATSHARE_FAILED);
	GetPlatformShareChestRes* res = (GetPlatformShareChestRes*)roUserData.m_pUserPtr;
	if (NULL == res)
	{
		return ;
	}

	Role *role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{		
		return;
	}

	// 系统是否已开
	if(!role->Get<CRoleSystem>()->IsSystemOpened(SYS_PLATFORMSHARE_BOX))
	{
		return;
	}

	role->Get<LotteryRecord>()->AddShareGiftCount();
	res->set_act_id(roRes.act_id());	
	if (res->errorcode() != KKSG::ERR_SUCCESS)
	{
		res->set_errorcode(KKSG::ERR_PLATSHARE_FAILED);
	}
	roRes.CopyFrom(*res);
}
