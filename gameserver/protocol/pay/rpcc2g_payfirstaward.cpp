#include "pch.h"
#include "pay/rpcc2g_payfirstaward.h"
#include "unit/rolemanager.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/bagtransition.h"
#include "role/ptcg2c_opensystemntf.h"
#include <time.h>
#include "gamelogic/payconfig.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2016/8/22 19:30:24

RPC_SERVER_IMPLEMETION(RpcC2G_PayFirstAward, PayFirstAwardArg, PayFirstAwardRes)

void RpcC2G_PayFirstAward::OnCall(const PayFirstAwardArg &roArg, PayFirstAwardRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	PayFirst::RowData* pRowData = CPayConfig::Instance()->GetPayFirstConf();
	if (NULL == pRowData)
	{
		roRes.set_errcode(ERR_FAILED);
		SSWarn<<"error pay first config."<<END;
		return;
	}

	if (pRowData->SystemID && !pRole->Get<CRoleSystem>()->IsSystemOpened(pRowData->SystemID))
	{
		roRes.set_errcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	//判断是否达到要求
	if ((INT32)(pRecord->m_TotalPay + pRecord->m_VipPoint) < pRowData->Money)
	{
		roRes.set_errcode(ERR_FAILED);
		SSWarn<<"pay money not enough"<<END;
		return;
	}

	//判断是否领取过
	if (pRecord->m_uLastFirstPayAwardTime)
	{
		roRes.set_errcode(ERR_FAILED);
		SSWarn<<"already get firstpay award"<<END;
		return;
	}
	pRecord->m_uLastFirstPayAwardTime = (UINT32)time(NULL);
	pRecord->m_isModified.Set();

	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Pay, ItemFlow_PayFirstAward);
	transition.GiveItem(pRowData->Award, 1);
	transition.NotifyClient();
	roRes.set_errcode(ERR_SUCCESS);

	pRole->Get<CRoleSystem>()->SetSystemOpened(CPayConfig::Instance()->GetPayFirstSystemID(), false);
	//PtcG2C_OpenSystemNtf oSysPtc;
	//oSysPtc.m_Data.add_closesysids(CPayConfig::Instance()->GetPayFirstSystemID());
	//pRole->Send(oSysPtc);
	std::vector<UINT32> closeids;
	closeids.push_back(CPayConfig::Instance()->GetPayFirstSystemID());
	OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &closeids);
}

void RpcC2G_PayFirstAward::OnDelayReplyRpc(const PayFirstAwardArg &roArg, PayFirstAwardRes &roRes, const CUserData &roUserData)
{
}
