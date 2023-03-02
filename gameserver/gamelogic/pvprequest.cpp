#include "pch.h"
#include "pvprequest.h"
#include "team.h"
#include "teammgr.h"
#include "unit/role.h"
#include "pvpmgr.h"
#include "pvprecord.h"
#include "unit/rolemanager.h"
#include "bagtransition.h"
#include "globalconfig.h"
#include "teammember.h"

PvpRequest::PvpRequest( Role* pRole )
{
	m_role = pRole;
}

int PvpRequest::GetWeekReward()
{
	if(NULL == m_role)
	{
		return KKSG::ERR_FAILED;
	}

	MyPvpRecord* mypvp = m_role->Get<MyPvpRecord>();
	if(!mypvp->GetWeekRewardHaveGet() && mypvp->GetWinCountThisWeek() >= GetGlobalConfig().PVPWeekReqCount)
	{
		BagGiveItemTransition give(m_role);
		give.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pvp);
		for(size_t i = 0; i <  GetGlobalConfig().PVPWeekRewards.size(); ++i)
		{
			Sequence<UINT32,2>& seq =  GetGlobalConfig().PVPWeekRewards[i];
			give.GiveItem(seq[0], seq[1], false);
		}

		give.NotifyClient();

		mypvp->SetWeekRewardHaveGet();
	}

	return 0;
}
