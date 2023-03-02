#include "pch.h"
#include "guild/rpcc2g_learnguildskill.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "table/XSkillEffectMgr.h"
#include "gamelogic/bagtransition.h"
#include "mslink.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/12 11:37:12

RPC_SERVER_IMPLEMETION(RpcC2G_LearnGuildSkill, LearnGuildSkillAgr, LearnGuildSkillRes)

void RpcC2G_LearnGuildSkill::OnCall(const LearnGuildSkillAgr &roArg, LearnGuildSkillRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return ;
	}

	// 先去ms  查询公会技能等级
	RpcG2M_OpGuildReqData* pMsg = RpcG2M_OpGuildReqData::CreateRpc();
	pMsg->m_oArg.set_roleid(pRole->GetID());
	pMsg->m_oArg.set_optype(STUDY_SKILL);
	pMsg->m_oArg.set_skillid(roArg.skillid());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
}

void RpcC2G_LearnGuildSkill::OnDelayReplyRpc(const LearnGuildSkillAgr &roArg, LearnGuildSkillRes &roRes, const CUserData &roUserData)
{

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return ;
	}

	CGuildRecord* pGuildRecord = pRole->Get<CGuildRecord>();
	if (pGuildRecord == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return ;
	}

	if (pGuildRecord->GetGuildSkillLvl(roArg.skillid()) >= *((UINT32*)roUserData.m_pUserPtr))
	{
		roRes.set_errorcode(KKSG::GUILD_SKILL_STUDY_LEVEL_LIMIT);
		return ;
	}

	std::map<UINT32, UINT32> costMap;
	XSkillEffectMgr::Instance()->GetGuildLearnCost(roArg.skillid(), pGuildRecord->GetGuildSkillLvl(roArg.skillid()), costMap);

	BagTakeItemTransition taketransition(pRole);
	taketransition.SetReason(ItemFlow_Skill, ItemFlow_Skill_Up);
	foreach (i in costMap)
	{
		if (!taketransition.TakeItem(i->first, i->second))
		{
			taketransition.RollBack();
			roRes.set_errorcode(KKSG::ERR_SHOP_LACKMONEY);
			return ;
		}
	}
	taketransition.NotifyClient();

	pGuildRecord->GuildSkillUp(roArg.skillid());
	pRole->RecalcuateAttributeAndNotify(RecalAttr_GuildSkill);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}
