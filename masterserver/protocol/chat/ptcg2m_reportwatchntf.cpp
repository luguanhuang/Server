#include "pch.h"
#include "chat/ptcg2m_reportwatchntf.h"
#include "chat/chatmgr.h"

// generate by ProtoGen at date: 2016/8/4 6:43:23

void PtcG2M_ReportWatchNtf::Process(UINT32 dwConnID)
{
	std::vector<UINT64> roleList;
	for(UINT32 i = 0; i < m_Data.roleidlist_size(); i++)
	{
		roleList.push_back(m_Data.roleidlist(i));
	}
	CChatMgr::Instance()->SendGroup(roleList, m_Data.chatinfo());
}
