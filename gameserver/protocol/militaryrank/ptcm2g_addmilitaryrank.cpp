#include "pch.h"
#include "militaryrank/ptcm2g_addmilitaryrank.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/military_rank_record.h"

// generate by ProtoGen at date: 2017/6/24 19:08:26

void PtcM2G_AddMilitaryRank::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole == NULL)
	{
		LogWarn("role [%llu] is not in this gs, AddMilitaryRank Failed, sceneType [%u]", m_Data.roleid(), m_Data.scenetype());
		return;
	}
	if (m_Data.iswin())
	{
		pRole->Get<MilitaryRankRecord>()->AddBattleRecord(m_Data.scenetype());
	}
	else
	{
		pRole->Get<MilitaryRankRecord>()->ReduceBattleRecord(m_Data.scenetype());
	}
}
