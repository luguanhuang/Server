#include "pch.h"
#include "cross/ptck2m_crosszonestatusntf.h"
#include "cross/crosszonestatus.h"

// generate by ProtoGen at date: 2016/11/18 16:06:46

void PtcK2M_CrossZoneStatusNtf::Process(UINT32 dwConnID)
{
	CrossZoneStatus::Instance()->SetCrossGsNum(m_Data.gs_num());
}
