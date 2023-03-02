#include "pch.h"
#include "guild/ptcm2g_synfatigueresult.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/9/19 10:58:23

void PtcM2G_SynFatigueResult::Process(UINT32 dwConnID)
{

	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	CGuildRecord *pRecord = pRole->Get<CGuildRecord>();
	if (pRecord == NULL)
	{
		return ;
	}

	if (m_Data.isfromrole() == 0)
	{
		pRecord->AddRecvFatigue(m_Data.addtimes());

		BagGiveItemTransition transition(pRole);
		transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Fatigue);
		transition.GiveItem(FATIGUE,  GetGlobalConfig().GetInt("GuildSendFatigue", 0) * m_Data.totalrecv());
		transition.NotifyClient();
	}
	else
	{
		pRecord->AddRecvFatigue();
		BagGiveItemTransition transition(pRole);
		transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Fatigue);
		transition.GiveItem(FATIGUE,  GetGlobalConfig().GetInt("GuildSendFatigue", 2));
		transition.NotifyClient();
	}
}
