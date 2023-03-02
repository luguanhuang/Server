#include "pch.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2016/12/21 23:24:06

RPC_CLIENT_IMPLEMETION(RpcM2G_PayConsumeAddGoods, PayConsumeAddgoodsArg, PayConsumeAddGoodsRes)

void RpcM2G_PayConsumeAddGoods::OnReply(const PayConsumeAddgoodsArg &roArg, const PayConsumeAddGoodsRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == ERR_SUCCESS)
	{
		CPayMgr::Instance()->ConsumeFinish(roArg.roleid(), roArg.balance(), roArg.list().billno());
		stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(roArg.roleid());
		if (NULL != pData)
		{
			auto it = pData->m_oConsumeList.find(roArg.list().billno());
			if (it != pData->m_oConsumeList.end())
			{
				pData->m_oConsumeList.erase(it);
				CPayMgr::Instance()->SaveData2DB(roArg.roleid(), *pData);
			}
		}
	}
}

void RpcM2G_PayConsumeAddGoods::OnTimeout(const PayConsumeAddgoodsArg &roArg, const CUserData &roUserData)
{
}
