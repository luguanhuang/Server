#include "pch.h"
#include "common/ptcg2t_opengate.h"
#include "serverstateinfo.h"
#include "session/sessionmanager.h"
#include "gslink.h"

// generate by ProtoGen at date: 2015/7/7 10:57:41

void PtcG2T_OpenGate::Process(UINT32 dwConnID)
{
	//ServerStateInfo::Instance()->SetOpenGateFlag(m_Data.flag());

	/*GSLink::Instance()->SetGSOpenFlag(dwConnID, m_Data.flag());

	GameServerInfo* pGSInfo = GSLink::Instance()->GetGameServerInfoByConn(dwConnID);
	if (NULL == pGSInfo)
	{
		return;
	}

	if (m_Data.flag())
	{
		LogWarn("open gate");
		return;
	}
	SessionManager::Instance()->GSShutDownCloseAllSession(pGSInfo->dwGSId);
	LogWarn("close gate");
	*/
}
