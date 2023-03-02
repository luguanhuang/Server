#include "pch.h"
#include "chat/ptcf2m_largeroomradiontf.h"
//#include "chat/ptcm2f_largeroomradiocopyntf.h"
#include "chat/fmmgr.h"
#include "network/fmlink.h"

// generate by ProtoGen at date: 2016/11/25 10:00:06

void PtcF2M_LargeRoomRadioNtf::Process(UINT32 dwConnID)
{
	FMMgr::Instance()->ClearAudio();
	for (UINT32 i = 0; i < m_Data.name_size(); i++)
	{
		std::string name = m_Data.name(i);
		LogDebug("--name=%s roleid=%llu--", m_Data.name(i).c_str(),m_Data.roleid(i));
		FMMgr::Instance()->AddAudio(name,m_Data.roleid(i));
	}
	FMMgr::Instance()->SendWhiteList();
	//PtcM2F_LargeRoomRadioCopyNtf ntf;
	//ntf.m_Data.CopyFrom(m_Data);
	//FMLink::Instance()->SendOther(dwConnID,ntf);
}
