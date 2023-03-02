#include "pch.h"
#include "scene/ptcc2g_enterscenecooldownquery.h"
#include "unit/rolemanager.h"
#include "gamelogic/stagemgr.h"
#include "scene/ptcg2c_enterscenecooldownnotify.h"

// generate by ProtoGen at date: 2015/7/30 18:15:16

void CallbackFun(void *ptr, UINT32 SceneID, UINT32 CoolDown)
{
	PtcG2C_EnterSceneCoolDownNotify* ptc = (PtcG2C_EnterSceneCoolDownNotify*)ptr;
	ptc->m_Data.add_groupid(SceneID);
	ptc->m_Data.add_cooldown(CoolDown);
}

void PtcC2G_EnterSceneCoolDownQuery::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	StageMgr *pStageMgr = pRole->Get<StageMgr>();

	PtcG2C_EnterSceneCoolDownNotify ptc;
	std::set<UINT32> groupids;
	if (0 == m_Data.groupid_size())
	{	
		pStageMgr->ForeachSceneCoolDown(CallbackFun, &ptc, groupids);
	}
	else
	{
		for (int i = 0; i < m_Data.groupid_size(); ++i)
		{
			groupids.insert(m_Data.groupid(i));
		}
		pStageMgr->ForeachSceneCoolDown(CallbackFun, &ptc, groupids);
	}	
	pRole->Send(ptc);
}
