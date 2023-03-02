#include "pch.h"
#include "pay/rpcc2g_growthfundaward.h"
#include "unit/rolemanager.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/loginrecord.h"
#include "role/ptcg2c_opensystemntf.h"
#include "gamelogic/payconfig.h"
#include <time.h>
#include "table/OpenSystemMgr.h"


// generate by ProtoGen at date: 2016/8/22 19:39:49

RPC_SERVER_IMPLEMETION(RpcC2G_GrowthFundAward, GrowthFundAwardArg, GrowthFundAwardRes)

void RpcC2G_GrowthFundAward::OnCall(const GrowthFundAwardArg &roArg, GrowthFundAwardRes &roRes)
{
	roRes.set_errcode(ERR_FAILED);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if (CPayConfig::Instance()->GetGrowthFundSystemID() && !pRole->Get<CRoleSystem>()->IsSystemOpened(CPayConfig::Instance()->GetGrowthFundSystemID()))
	{
		roRes.set_errcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	if (roArg.type() != growthfund_award_level && roArg.type() != growthfund_award_login)
	{
		SSWarn<<"invalid growthfund type "<<roArg.type()<<END;
		return ;
	}
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	RechargeTable::RowData* pRowData = CPayConfig::Instance()->GetGrowthFundConf();
	if (NULL == pRowData)
	{
		SSWarn<<"invalid RechargeTable.txt"<<END;
		return ;
	}

	INT32 nAmount = 0;
	KKSG::PayAwardRecord rInfo;
	if (roArg.type() == growthfund_award_level)
	{
		//判断是否领取过该等级的奖励;
		for (auto i = pRecord->fundLevelInfo.begin(); i != pRecord->fundLevelInfo.end(); i ++)
		{
			if (i->id() == roArg.value())
			{
				SSWarn<<"already got this award level"<<roArg.value()<<END;
				return;
			}
		}

		//判断配置中是否有该等级的;
		for (UINT32 i = 0; i < pRowData->RoleLevels.size(); i ++)
		{
			if (pRowData->RoleLevels[i][0] == roArg.value())
			{
				nAmount = pRowData->RoleLevels[i][1];
				break;
			}
		}
		if (!nAmount)
		{
			SSWarn<<"invalid request level "<<roArg.value()<<END;
			return ;
		}

		//判断等级是否足够;
		if (roArg.value() > pRole->GetLevel())
		{
			SSWarn<<"level "<<roArg.value()<<"not enough "<<pRole->GetLevel()<<END;
			return;
		}
	}
	else
	{
		for (auto i = pRecord->fundLoginInfo.begin(); i != pRecord->fundLoginInfo.end(); i ++)
		{
			if (i->id() == roArg.value())
			{
				SSWarn<<"already got this award login"<<roArg.value()<<END;
				return;
			}
		}

		for (UINT32 i = 0; i < pRowData->LoginDays.size(); i ++)
		{
			if (pRowData->LoginDays[i][0] == roArg.value())
			{
				nAmount = pRowData->LoginDays[i][1];
				break;
			}
		}
		if (!nAmount)
		{
			SSWarn<<"invalid request login days "<<roArg.value()<<END;
			return ;
		}

		//判断登录天数是否足够;
		if ((UINT32)roArg.value() > pRole->Get<MyLoginRecord>()->GetRealLoginDayCount())
		{
			SSWarn<<"login days "<<roArg.value()<<"not enough "<<pRole->Get<MyLoginRecord>()->GetRealLoginDayCount()<<END;
			return;
		}
	}
	//开始加钻石
	if (nAmount && pRole->AddDiamond(nAmount, ItemFlow_Pay, ItemFlow_GrowthFundAward))
	{
		rInfo.set_id(roArg.value());
		rInfo.set_lastgetawardtime(UINT32(time(NULL)));
		if (roArg.type() == growthfund_award_level)
		{
			pRecord->fundLevelInfo.push_back(rInfo);
		}
		else
		{
			pRecord->fundLoginInfo.push_back(rInfo);
		}
	}
	else
	{
		SSWarn << "add diamond send to ms error" << END;
		return;
	}

	pRecord->m_isModified.Set();
	roRes.set_errcode(ERR_SUCCESS);
	for (auto i = pRecord->fundLevelInfo.begin(); i != pRecord->fundLevelInfo.end(); i ++)
	{
		roRes.add_growthfundlevelinfo(i->id());
	}
	for (auto i = pRecord->fundLoginInfo.begin(); i != pRecord->fundLoginInfo.end(); i ++)
	{
		roRes.add_growthfundlogininfo(i->id());
	}

	//判断是否已经领取完奖励;
	if (pRowData->RoleLevels.size() == pRecord->fundLevelInfo.size() && pRowData->LoginDays.size() == pRecord->fundLoginInfo.size())
	{
		PayV2Mgr::Instance()->CloseGrowthFund(pRole);
	}
}
void RpcC2G_GrowthFundAward::OnDelayReplyRpc(const GrowthFundAwardArg &roArg, GrowthFundAwardRes &roRes, const CUserData &roUserData)
{
}
