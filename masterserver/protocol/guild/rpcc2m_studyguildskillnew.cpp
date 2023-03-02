#include "pch.h"
#include "guild/rpcc2m_studyguildskillnew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/9/8 16:42:42

RPC_SERVER_IMPLEMETION(RpcC2M_StudyGuildSkillNew, StudyGuildSkillArg, StudyGuildSkillRes)

void RpcC2M_StudyGuildSkillNew::OnCall(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes)
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
		roRes.set_errorcode(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	UINT32 skillId = roArg.skillid();
	KKSG::ErrorCode errorCode = (KKSG::ErrorCode)pGuild->canStudySkill(pRole->GetID(), skillId); 
	if (errorCode != ERR_SUCCESS)
	{
		roRes.set_errorcode(errorCode);
		roRes.set_skillid(skillId);
		roRes.set_skilllel(pGuild->GetSkillLvl(skillId));
		roRes.set_lastexp(pGuild->GetLastExp());
		return;
	}

	pGuild->doStudySkill(skillId);

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	roRes.set_skillid(skillId);
	roRes.set_skilllel(pGuild->GetSkillLvl(skillId));
	roRes.set_lastexp(pGuild->GetLastExp());	

	pGuild->UpdateGuildSkillDataToEveryone();
}

void RpcC2M_StudyGuildSkillNew::OnDelayReplyRpc(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes, const CUserData &roUserData)
{
}
