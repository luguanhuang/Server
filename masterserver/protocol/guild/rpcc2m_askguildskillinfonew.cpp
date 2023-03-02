#include "pch.h"
#include "guild/rpcc2m_askguildskillinfonew.h"
#include "guild/rpcm2g_reqroleguildskill.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "network/gslink.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/8 16:31:16

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildSkillInfoNew, AskGuildSkillInfoArg, AskGuildSkillInfoReq)

void RpcC2M_AskGuildSkillInfoNew::OnCall(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes)
{

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);

	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	RpcM2G_ReqRoleGuildSkill* pMsg = RpcM2G_ReqRoleGuildSkill::CreateRpc();
	pMsg->m_oArg.set_roleid(pRole->GetID());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
	return ;


		/*
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);


	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	const std::map<UINT32, UINT32>& studySkillMap = pGuild->GetStudySkill();
	for (auto iter = studySkillMap.begin(); iter != studySkillMap.end(); ++iter)
	{
		KKSG::GuildSkillData* pData = roRes.add_skilllel();
		pData->set_skillid(iter->first);
		pData->set_maxlvl(iter->second);
	}

	roRes.set_lastguildexp(pGuild->GetLastExp());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);*/
}

void RpcC2M_AskGuildSkillInfoNew::OnDelayReplyRpc(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	std::map<UINT32, UINT32>& roleSkillMap = *((std::map<UINT32, UINT32>*)roUserData.m_pUserPtr); 
	foreach (i in roleSkillMap)
	{
		KKSG::GuildSkillData* pData = roRes.add_roleskills();
		pData->set_skillid(i->first);
		pData->set_maxlvl(i->second);
	}

	const std::map<UINT32, UINT32>& studySkillMap = pGuild->GetStudySkill();
	for (auto iter = studySkillMap.begin(); iter != studySkillMap.end(); ++iter)
	{
		KKSG::GuildSkillData* pData = roRes.add_skilllel();
		pData->set_skillid(iter->first);
		pData->set_maxlvl(iter->second);
	}

	roRes.set_lastguildexp(pGuild->GetLastExp());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}
