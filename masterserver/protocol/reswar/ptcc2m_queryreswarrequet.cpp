#include "pch.h"
#include "reswar/ptcc2m_queryreswarrequet.h"
#include "reswar/ptcm2c_reswarguildbriefntf.h"
#include "reswar/reswarmgr.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"

// generate by ProtoGen at date: 2016/11/11 21:12:55

void PtcC2M_QueryResWarRequet::Process(UINT32 dwConnID)
{
	PtcM2C_ResWarGuildBriefNtf ntf;
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole)  
	{
		ntf.m_Data.set_error(KKSG::ERR_UNKNOWN);
		pRole->Send(ntf);
		return;
	}
	bool bOpen = ResWarMgr::Instance()->GetState();
	if (!bOpen)
	{
		ntf.m_Data.set_error(KKSG::ERR_RESWAR_ACTIVITY);
		pRole->Send(ntf);
		return;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild)
	{
		ntf.m_Data.set_error(KKSG::ERR_NOTGUILD);
		pRole->Send(ntf);
		return;
	}
	GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(pGuild->GetID());
	if (!pGuildTeam)
	{
		ntf.m_Data.set_error(KKSG::ERR_RESWAR_GROUP);
		pRole->Send(ntf);
		return;
	}
	Team *pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	GuildFightTeam *pFightTeam = NULL;
	if (pTeam)
	{
		pFightTeam =  ResWarMgr::Instance()->GetGuildFightTeam(pGuild->GetID(), pTeam->GetID());
	}
	KKSG::ResWarGuildBrief *pBrief = &ntf.m_Data;
	ResWarMgr::Instance()->GetResWarGuildBrief(pGuildTeam,pFightTeam,pBrief);
	pRole->Send(ntf);
}
