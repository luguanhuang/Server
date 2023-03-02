#include "pch.h"
#include "gm/ptcp2i_updateplatnoticeloginntf.h"
#include "notice/platnoticemgr.h"

// generate by ProtoGen at date: 2016/11/19 14:20:13

void PtcP2I_UpdatePlatNoticeLoginNtf::Process(UINT32 dwConnID)
{
	CPlatNoticeMgr::Instance()->UpdateNotice(m_Data);
}
