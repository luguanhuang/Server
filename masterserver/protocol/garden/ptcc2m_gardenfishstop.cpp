#include "pch.h"
#include "garden/ptcc2m_gardenfishstop.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
// generate by ProtoGen at date: 2016/11/15 11:05:48

void PtcC2M_GardenFishStop::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;		
		return;
	}

	// 通知钓鱼结束		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::FISH_STOP);	
	arg.set_role_id(role->GetID());
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,m_Data.garden_id());
}
