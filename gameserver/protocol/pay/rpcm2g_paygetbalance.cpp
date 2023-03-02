#include "pch.h"
#include "pay/rpcm2g_paygetbalance.h"
#include "gamelogic/bag.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payconfig.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/ibshoprecord.h"

// generate by ProtoGen at date: 2016/12/21 23:38:22

RPC_SERVER_IMPLEMETION(RpcM2G_PayGetBalance, PayGetBalanceArg, PayGetBalanceRes)

void RpcM2G_PayGetBalance::OnCall(const PayGetBalanceArg &roArg, PayGetBalanceRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (pRole == NULL)
	{
		return;
	}

	INT32 iDiamond = (INT32)pRole->Get<Bag>()->CountVirtualItem(DIAMOND);
	pRole->Get<IBShopRecord>()->SetPayDegree(roArg.costsum());
	PayV2Mgr::Instance()->BuyPay(pRole, roArg.balance(), roArg.saveamt());
	for (INT32 i = 0; i < roArg.tsslist_size(); i ++)
	{
		std::string paramid = CPayConfig::Instance()->GetParamByServiceCode(roArg.tsslist(i).innerproductid());
		if (paramid.empty())
		{
			continue;
		}
		KKSG::PaytssInfo oData;
		INT32 b = INT32(XCommon::ParseTime(roArg.tsslist(i).begintim()));
		INT32 e = INT32(XCommon::ParseTime(roArg.tsslist(i).endtime()));
		if (b && b < e)
		{
			oData.set_begintime(b);
			oData.set_endtime(e);
			
			INT32 ret = PayV2Mgr::Instance()->Pay(pRole, paramid, oData);
			if (ret == KKSG::ERR_SUCCESS || ret == KKSG::ERR_ALREADY_BUYAILEEN)
			{
				roRes.add_paracode(roArg.tsslist(i).innerproductid());
			}
		}
	}
	if (iDiamond < roArg.balance())
	{
		TMoneyFlowLog oLog(pRole);
		oLog.m_nMoneyType = TX_DIAMOND;
		oLog.m_nMoneyChange = roArg.balance() - iDiamond;
		oLog.m_nReason = ItemFlow_Pay;
		oLog.m_nSubReason = ItemFlow_BuyPay;
		oLog.m_nAddOrReduce = TX_ADD;
		oLog.Do();
	}
}

void RpcM2G_PayGetBalance::OnDelayReplyRpc(const PayGetBalanceArg &roArg, PayGetBalanceRes &roRes, const CUserData &roUserData)
{
}
