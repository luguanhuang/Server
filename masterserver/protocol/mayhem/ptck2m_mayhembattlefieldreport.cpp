#include "pch.h"
#include "mayhem/ptck2m_mayhembattlefieldreport.h"
#include "notice/notice.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"
#include "mayhem/mayhemmgr.h"

// generate by ProtoGen at date: 2017/6/16 14:58:17

void PtcK2M_MayhemBattlefieldReport::Process(UINT32 dwConnID)
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return ;
	}
	// 每轮战报
	if ( 1== m_Data.report_type())
	{
		Notice notice(MayHemRoundResultNotice);		
		notice.Replace("{0}",m_Data.round());
		notice.Replace("{1}",m_Data.server_name());
		notice.Replace("{2}",m_Data.player_name());
		notice.Send();
	}
	else
	{
		Notice notice(MayHemEndNotice);			
		notice.Replace("{0}",m_Data.server_name());
		notice.Replace("{1}",m_Data.player_name());
		notice.Send();
		MayhemMsMgr::Instance()->ShowAwardIcon();

	}
}
