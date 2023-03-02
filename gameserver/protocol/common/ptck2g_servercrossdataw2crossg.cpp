#include "pch.h"
#include "common/ptck2g_servercrossdataw2crossg.h"
#include "servermgr/servercrossdatamgr.h"

// generate by ProtoGen at date: 2017/7/6 11:27:00

void PtcK2G_ServerCrossDataW2CrossG::Process(UINT32 dwConnID)
{
	SSInfo << " server cross data size = " << m_Data.serverdata_size() << END;
	for (int i = 0; i < m_Data.serverdata_size(); ++i)
	{
		ServerCrossDataMgr::Instance()->AddData(m_Data.serverdata(i));
	}
}
