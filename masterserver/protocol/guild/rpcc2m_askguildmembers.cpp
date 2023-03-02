#include "pch.h"
#include "guild/rpcc2m_askguildmembers.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"
#include "guild/rpcm2g_reqguildinherittimes.h"
#include "network/gslink.h"
#include "task/roletask.h"
#include "task/roletaskmgr.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/9/6 10:43:57

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildMembers, GuildMemberArg, GuildMemberRes)

struct GetMemberParams
{
	CRole* pRole;
	Guild *pGuild;
	GuildMemberRes *pRes;
	GuildFatigue *pGF;
	UINT64 qwRoleID;
};

static void TravelMember(GuildMember *pMember, void *arg)
{
	//UINT32 nowtime = (UINT32)time(NULL);
	GetMemberParams *pParam = (GetMemberParams *)arg;
	GuildMemberRes &roRes = *pParam->pRes;
	CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pMember->qwRoleID);
	if (pRoleSum != NULL)
	{
		KKSG::GuildMemberData *pData = roRes.add_members();
		pData->set_name(pRoleSum->GetName());
		pData->set_roleid(pRoleSum->GetID());
		pData->set_position(pMember->position);
		pData->set_contribute(pMember->contribute);
		pData->set_ppt(pRoleSum->GetPowerPoint());
		pData->set_level(pRoleSum->GetLevel());
		pData->set_profession((KKSG::RoleType)pRoleSum->GetProfession());
		pData->set_vip(pRoleSum->GetVipLevel());
		pData->set_lastlogin(pRoleSum->GetLastLogout());
		pData->set_paymemberid(pRoleSum->GetPayMemberID());
		pData->set_title(pRoleSum->GetTitleID());
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRoleSum->GetID());
		if (pGuild != NULL)
		{
			CGuildActivity* pActivity = pGuild->Get<CGuildActivity>();
			if (pActivity != NULL)
			{
				pData->set_activity(pActivity->GetRoleActivity(pRoleSum->GetID()));
			}
		}
		CRoleTask* pRoleTask = RoleTaskMgr::Instance()->GetRoleTask(pMember->qwRoleID);
		if (pRoleTask)
		{
			pData->set_task_luck(pRoleTask->m_luck);
		}

		int flag = 0;
		if (0 == pParam->qwRoleID)
		{
			pData->set_flag(flag);
			return;
		}

		/*if (pParam->pGF->CanSendFatigueTo(pParam->qwRoleID, pMember->qwRoleID))
		{
			flag |= KKSG::SEND_FATIGUE;
		}
		bool hasRecived = false;
		if (pParam->pGF->CanRecvFatigueFrom(pParam->qwRoleID, pMember->qwRoleID, hasRecived))
		{
			flag |= KKSG::RECV_FATIGUE;
		}
		if (hasRecived)
		{
			flag |= KKSG::RECVED_FATIGUE;
		}*/
		pData->set_flag(flag);
	}
}

void RpcC2M_AskGuildMembers::OnCall(const GuildMemberArg &roArg, GuildMemberRes &roRes)
{
	roRes.set_recvfatigue(0);
	roRes.set_fatiguemax(0);

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if (pRole->IsInCrossGs())
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT64 guildid = roArg.guildid();
	if (0 == guildid)
	{
		guildid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
		if (guildid == INVALID_GUILDID)
		{
			return;
		}
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuild(guildid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	GetMemberParams params;
	params.pGuild = pGuild;
	params.pRes = &roRes;
	params.pRole = pRole;
	params.pGF = pGuild->Get<GuildFatigue>();
	params.qwRoleID = (NULL == pRole) ? 0 : pRole->GetID();
	pGuild->ForeachMember(TravelMember, &params);

// 设置能收取体力次数上限，等于公会人数
	Guild* pGuildSelf = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuildSelf != NULL)
	{
		roRes.set_fatiguemax(pGuildSelf->GetMemberCount());
	}

	RpcM2G_ReqGuildInheritTimes* pMsg = RpcM2G_ReqGuildInheritTimes::CreateRpc();
	pMsg->m_oArg.set_roleid(pRole->GetID());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pMsg->m_oArg.set_roleid(pRole->GetID());
	const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
	for (auto iter = members.begin(); iter != members.end(); ++iter)
	{
		if (!CRoleManager::Instance()->IsOnline(iter->first))
		{
			continue;
		}

		if (iter->first != pRole->GetID() && CRoleManager::Instance()->GetByRoleID(iter->first) != NULL)
		{
			pMsg->m_oArg.add_members(iter->first);
		}
	}
	GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
	return ;

	//roRes.set_recvfatigue(pRole->Get<CGuildRecord>()->GetRecvFatigue());
	//roRes.set_result(KKSG::ERR_SUCCESS);

	
}

void RpcC2M_AskGuildMembers::OnDelayReplyRpc(const GuildMemberArg &roArg, GuildMemberRes &roRes, const CUserData &roUserData)
{
	std::set<UINT64>& roleSet = *((std::set<UINT64>*)roUserData.m_pUserPtr); 

	foreach (i in roleSet)
	{
		roRes.add_guildinheritid(*i);
	}

	//roRes.set_recvfatigue(pRole->Get<CGuildRecord>()->GetRecvFatigue());
	roRes.set_result(KKSG::ERR_SUCCESS);
}
