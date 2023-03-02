#include "pch.h"
#include "team/ptcc2g_fmbrefuse.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/7/4 11:50:47

void PtcC2G_FMBRefuse::Process(UINT32 dwConnID)
{
	//if(!GSConfig::Instance()->IsHallGS())
	//{
	//	LogWarn("Receive request not in hall gs");
	//	return;
	//}

	//Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	//if (pRole == NULL)
	//{
	//	return;
	//}

	//if(m_Data.refuse())
	//{
	//	//用来停止匹配 （概念混淆）需要优化
	//	TeamMatchMgr::Instance()->StopMatch(pRole);
	//}
}
