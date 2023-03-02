#include "pch.h"
#include "guild/rpcc2g_askguildskillinfo.h"

#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/7/2 20:07:10

RPC_SERVER_IMPLEMETION(RpcC2G_AskGuildSkillInfo, AskGuildSkillInfoArg, AskGuildSkillInfoReq)

void RpcC2G_AskGuildSkillInfo::OnCall(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes)
{
	/*
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);

	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* pGuild = GuildMgr::Instance()->FindGuildByRoleId(pRole->GetID());
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
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	*/
}

void RpcC2G_AskGuildSkillInfo::OnDelayReplyRpc(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes, const CUserData &roUserData)
{
}
