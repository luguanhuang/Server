#include "pch.h"
#include "leaguebattle/ptck2m_leaguecrosselintfms.h"
#include "leaguebattle/leaguecrosselimgr.h"

// generate by ProtoGen at date: 2017/2/6 10:30:26

void PtcK2M_LeagueCrossEliNtfMs::Process(UINT32 dwConnID)
{
	if (m_Data.has_alldata())
	{
		LeagueCrossEliMgr::Instance()->UpdateAllData(m_Data.alldata());
	}
	if (m_Data.has_oneroom())
	{
		LeagueCrossEliMgr::Instance()->UpdateOneRoom(m_Data.oneroom());
	}
	if (m_Data.has_chamption())
	{
		LeagueCrossEliMgr::Instance()->UpdateChamption(m_Data.chamption());
	}
}
