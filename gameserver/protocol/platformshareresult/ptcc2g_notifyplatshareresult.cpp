#include "pch.h"
#include "platformshareresult/ptcc2g_notifyplatshareresult.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/platformshare_record.h"

// generate by ProtoGen at date: 2017/5/31 21:33:22

void PtcC2G_NotifyPlatShareResult::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		return;
	}
	if (m_Data.redpoint_disappear())
	{
		role->Get<PlatformShareRecord>()->SetDisappearRedPoint();
	}
	else
	{
		if (m_Data.scene_id() > 0)
		{
			role->Get<PlatformShareRecord>()->AddFirstShare(m_Data.scene_id());
		}
		else
		{	
			role->Get<PlatformShareRecord>()->AddShareNumWeekly();
		}
	}
}
