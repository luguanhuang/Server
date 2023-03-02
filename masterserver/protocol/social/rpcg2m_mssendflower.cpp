#include "pch.h"
#include "social/rpcg2m_mssendflower.h"
#include "social/flowerop.h"
#include "social/flower.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "scene/sceneconfig.h"
#include "social/ptcm2c_msreceiveflowerntf.h"
#include "social/ptcm2c_msflowerrainntf.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "friend/friendop.h"
#include "notice/noticemgr.h"
#include "role/rolefindercondition.h"

// generate by ProtoGen at date: 2016/8/18 16:37:31

RPC_SERVER_IMPLEMETION(RpcG2M_MSSendFlower, MSSendFlowerArg, MSSendFlowerRes)

static UINT32 s_flowerRainNtfTime = 0;
void RpcG2M_MSSendFlower::OnCall(const MSSendFlowerArg &roArg, MSSendFlowerRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error(KKSG::ERR_UNKNOWN);
		LogError("send flower missing role session:%llu", m_sessionID);
		return;
	}
	FlowerOp op(roArg.roleid());
	op.SendFlower(role->GetID(), roArg.itemid(), roArg.itemcount());

	CRoleSummary* thisSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (!thisSummary)
	{
		roRes.set_error(KKSG::ERR_SUCCESS);
		LogError("send flower missing send role summary:%llu", role->GetID());
		return;
	}
	CRoleSummary* otherrole = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.roleid());
	if (NULL != otherrole && roArg.itemcount())
	{
		NoticeMgr::Instance()->SendFlowerNotice(thisSummary, otherrole, roArg.itemid(), roArg.itemcount());
		LogInfo("send flower role %llu, send to %llu ,%u, %u", role->GetID(), otherrole->GetID(), roArg.itemid(), roArg.itemcount());
	}

	CRole* receiveRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	const FlowerSendNoticeTable::RowData* data = FlowerConfig::Instance()->GetNoticeData(roArg.itemid(), roArg.itemcount());
	if (data && strcmp(data->ThanksWords.c_str(),"") != 0 &&  receiveRole)
	{
		FriendOp friendOp(receiveRole->GetID());
		bool isBlack = friendOp.IsInBlackList(role->GetID());
		if (!isBlack && receiveRole->GetMapID())
		{
			auto conf = CSceneConfig::Instance()->GetSceneConf(receiveRole->GetMapID());
			if (!conf)
			{
				roRes.set_error(KKSG::ERR_SUCCESS);
				LogError("receive role:%llu, in wrong map: %u", receiveRole->GetID(), receiveRole->GetMapID());
				return;
			}
			if ((conf->type == KKSG::SCENE_HALL || conf->type == KKSG::SCENE_GUILD_HALL || conf->type == KKSG::SCENE_FAMILYGARDEN))
			{
				PtcM2C_MSReceiveFlowerNtf  send;
				send.m_Data.set_sendroleid(role->GetID());
				send.m_Data.set_sendname(role->GetName());
				send.m_Data.set_power(thisSummary->GetPowerPoint());
				send.m_Data.set_profession(thisSummary->GetProfession());
				send.m_Data.set_vip(thisSummary->GetVipLevel());
				send.m_Data.set_itemid(roArg.itemid());
				send.m_Data.set_itemcount(roArg.itemcount());
				receiveRole->Send(send);
			}
		}
	}

	bool isRain = FlowerConfig::Instance()->IsFlowerRain(roArg.itemid(), roArg.itemcount());
	if (isRain)
	{
		int now = time(0);
		if (now < s_flowerRainNtfTime + GetGlobalConfig().FlowerRainTime)
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			return;
		}
		bool hasSend = false;
		PtcM2C_MSFlowerRainNtf send;
		send.m_Data.set_itemid(roArg.itemid());
		send.m_Data.set_itemcount(roArg.itemcount());

		std::vector<UINT32> sceneIDs;
		sceneIDs.push_back(HALL_MAP_ID);
		sceneIDs.push_back(GUILD_MAP_ID);
		sceneIDs.push_back(FAMILYGARDEN_MAP_ID);
		RoleSceneFindCondition condition(sceneIDs);
		CRoleManager::Instance()->Foreach(condition);
		for(auto iter = condition.GetFoundRoles().begin(); iter != condition.GetFoundRoles().end(); ++iter)
		{
			if (*iter)
			{
				(*iter)->Send(send);
				hasSend = true;
			}
		}

		if (hasSend)
		{
			s_flowerRainNtfTime = now;
		}
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcG2M_MSSendFlower::OnDelayReplyRpc(const MSSendFlowerArg &roArg, MSSendFlowerRes &roRes, const CUserData &roUserData)
{
}
