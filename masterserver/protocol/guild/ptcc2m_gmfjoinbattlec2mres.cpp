#include "pch.h"
#include "guild/ptcc2m_gmfjoinbattlec2mres.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "gmf/guildmatchfightmgr.h"

// generate by ProtoGen at date: 2016/9/20 17:04:52

void PtcC2M_GmfJoinBattleC2MRes::Process(UINT32 dwConnID)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		return;
	}
	if(m_Data.join())
	{
		GuildMatchFightMgr::Instance()->JoinRequest(pRole);
	}

}
