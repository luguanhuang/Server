#include "pch.h"
#include "guild/rpcc2g_studyguildskill.h"

#include "unit/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildconfig.h"
#include "table/XSkillEffectMgr.h"
#include "gamelogic/noticemgr.h"

// generate by ProtoGen at date: 2016/7/3 15:49:02

RPC_SERVER_IMPLEMETION(RpcC2G_StudyGuildSkill, StudyGuildSkillArg, StudyGuildSkillRes)

void RpcC2G_StudyGuildSkill::OnCall(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	#if GUILD_GS
	Guild* pGuild = GuildMgr::Instance()->FindGuildByRoleId(pRole->GetID());
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
#endif
}

void RpcC2G_StudyGuildSkill::OnDelayReplyRpc(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes, const CUserData &roUserData)
{
}
