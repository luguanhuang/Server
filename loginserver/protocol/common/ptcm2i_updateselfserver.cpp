#include "pch.h"
#include "verify/tokenverifymgr.h"
#include "verify/DBUpdateSelfServerTask.h"
#include "common/ptcm2i_updateselfserver.h"

// generate by ProtoGen at date: 2016/8/24 20:55:04

void PtcM2I_UpdateSelfServer::Process(UINT32 dwConnID)
{
	if (0 == m_Data.level())
	{
		SSWarn<<"level is 0, account:"<<m_Data.accountid()<<END;
		return;
	}
	DBUpdateSelfServerTask* task = new DBUpdateSelfServerTask(m_Data.accountid(), m_Data.serverid(), m_Data.level());
	TokenVerifyMgr::Instance()->AddDBTask(task);

	SSInfo<<"update self server, account:"<<m_Data.accountid()<<" serverid:"<<m_Data.serverid()<<" level:"<<m_Data.level()<<END;
}
