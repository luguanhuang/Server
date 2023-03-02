#include "pch.h"
#include "pay/ptcm2g_balanceinfontf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/payv2Mgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/bag.h"

// generate by ProtoGen at date: 2016/12/21 23:21:07

void PtcM2G_BalanceInfoNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (NULL == pRole)
	{
		return ;
	}
	INT32 iDiamond = (INT32)pRole->Get<Bag>()->CountVirtualItem(DIAMOND);
	if (iDiamond != m_Data.balance())
	{
		PayV2Mgr::Instance()->NotifyDiamond(pRole, m_Data.balance());
		TMoneyFlowLog oLog(pRole);
		oLog.m_nMoneyType = TX_DIAMOND;
		if (m_Data.list().amt())
		{
			oLog.m_nMoneyChange = m_Data.list().amt();
			oLog.m_nReason = m_Data.list().reason();
			oLog.m_nSubReason = m_Data.list().subreason();
			oLog.m_nAddOrReduce = TX_ADD;
			oLog.SetTransTag(m_Data.list().billno());
		}
		else
		{
			oLog.m_nAddOrReduce = iDiamond<m_Data.balance()?TX_ADD:TX_REDUCE;
			oLog.m_nMoneyChange = abs(iDiamond-m_Data.balance());
		}
		oLog.Do();
	}
}
