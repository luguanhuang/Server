#include "pch.h"
#include "firstpass/ptcm2g_firstpassnewcommendrewardntf.h"
#include "firstpass/ptcg2c_firstpassrewardntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/8/26 19:37:21

void PtcM2G_FirstPassNewCommendRewardNtf::Process(UINT32 dwConnID)
{
	//CHECK_HALL_GS
	PtcG2C_FirstPassRewardNtf ntf;
	//RoleManager::Instance()->BroadCast(ntf);

	auto begin = RoleManager::Instance()->Begin();
	auto end = RoleManager::Instance()->End();
	for (auto iter = begin; iter != end; ++iter)
	{
		bool isin = false;
		for (int i = 0; i < m_Data.serverids_size(); ++i)
		{
			if (m_Data.serverids(i) == (*iter)->GetServerId())
			{
				isin = true;
				break;
			}
		}
		if (isin)
		{
			(*iter)->Send(ntf);
		}
	}
}
