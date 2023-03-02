#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcard.h"
#include "role/rolesummarymgr.h"
#include "guild/ptcc2m_guildcardrankreq.h"
#include "guild/ptcm2c_guildcardrankntf.h"
#include "pokertournament/cardmatchmgr.h"
// generate by ProtoGen at date: 2016/10/1 20:09:28

void PtcC2M_GuildCardRankReq::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		return;
	}
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if ( 3 != m_Data.type())
	{
		if (NULL == guild)
		{
			SSWarn<<"role has no guild, roleid:"<<role->GetID()<<END;
			return;
		}
	}
	const std::set<CardPoint>* points = NULL;
	if (0 == m_Data.type())
	{
		points = guild->Get<CGuildCard>()->GetRecord(role->GetID());
	}
	else if (1 == m_Data.type())
	{
		points = guild->Get<CGuildCard>()->GetTopRecords();
	}
	else if (2 == m_Data.type())
	{
		points = guild->Get<CGuildCard>()->GetMatchPoints();
	}
	else if (3 == m_Data.type())
	{
		GuildCardMatch* card_match = CardMatchMgr::Instance()->GetCardMatchByRoleID(role->GetID());
		if (NULL != card_match)
		{
			points = card_match->GetMatchPoints();			
		}
	}
	if (NULL == points)
	{
		SSWarn<<"points is NULL, type:"<<m_Data.type()<<END;
		return;
	}
	PtcM2C_GuildCardRankNtf ntf;
	ntf.m_Data.set_type(m_Data.type());
	for (auto i = points->begin(); i != points->end(); ++i)
	{
		ntf.m_Data.add_name(CRoleSummaryMgr::Instance()->GetRoleName(i->roleid));	
		ntf.m_Data.add_point(i->point);	
	}
	role->Send(ntf);
}
