#include "pch.h"
#include "gm/ptcp2m_updateplatnoticemsntf.h"
#include "idip/platnotice.h"

// generate by ProtoGen at date: 2016/11/19 14:18:46

void PtcP2M_UpdatePlatNoticeMsNtf::Process(UINT32 dwConnID)
{
	CPlatNoticeMgr::Instance()->UpdateNotice(m_Data);
}
