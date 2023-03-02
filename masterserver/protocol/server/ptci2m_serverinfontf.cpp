#include "pch.h"
#include "server/ptci2m_serverinfontf.h"
#include "other/serverinfomgr.h"

// generate by ProtoGen at date: 2017/1/12 20:19:49

void PtcI2M_ServerInfoNtf::Process(UINT32 dwConnID)
{
	for(int i = 0; i < m_Data.serverinfo_size(); ++i)
	{
		ZServerInfoMgr::Instance()->AddServerInfo(m_Data.serverinfo(i));
	}
}
