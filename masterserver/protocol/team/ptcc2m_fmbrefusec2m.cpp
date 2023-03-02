#include "pch.h"
#include "team/ptcc2m_fmbrefusec2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teammatchmgr.h"
#include "other/matchhandler.h"

// generate by ProtoGen at date: 2016/8/19 15:52:06

void PtcC2M_FMBRefuseC2M::Process(UINT32 dwConnID)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	if(m_Data.refuse())
	{
		//用来停止匹配 （概念混淆）需要优化
		//TeamMatchMgr::Instance()->StopMatch(pRole);
		HMatchHandler handler;
		handler.Stop(pRole, KKSG::KMT_EXP);
	}

}
