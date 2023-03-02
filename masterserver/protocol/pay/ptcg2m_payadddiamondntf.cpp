#include "pch.h"
#include "pay/ptcg2m_payadddiamondntf.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2016/12/22 10:40:28

void PtcG2M_PayAddDiamondNtf::Process(UINT32 dwConnID)
{
	//无论在线与否，都需要给这个玩家处理;
	CPayMgr::Instance()->AddDiamond(m_Data.roleid(), m_Data.data());
}
