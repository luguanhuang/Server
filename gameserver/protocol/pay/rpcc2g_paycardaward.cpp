#include "pch.h"
#include "pay/rpcc2g_paycardaward.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "unit/rolemanager.h"
#include "util/XCommon.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "gamelogic/payconfig.h"
#include "util/gametime.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/8/11 19:44:19

RPC_SERVER_IMPLEMETION(RpcC2G_PayCardAward, PayCardAwardArg, PayCardAwardRes)

void RpcC2G_PayCardAward::OnCall(const PayCardAwardArg &roArg, PayCardAwardRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(Welfare_DiamondCard))
	{
		roRes.set_errcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	if (roArg.type() != KKSG::WEEK_CARD && roArg.type() != KKSG::MONTH_CARD)
	{
		SSWarn<<"invalid pay card type"<<roArg.type()<<END;
		return ;
	}
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if( NULL == pRecord )
	{
		roRes.set_errcode(ERR_FAILED);
		return; 
	}
	INT32 nDays = 0;
	KKSG::PayCard* pInfo = roRes.mutable_info();
	INT32 nDiamondCount = 0;
	if (roArg.type() == KKSG::WEEK_CARD)
	{
		//判断是否开通过周卡,是否过期;
		if(pRecord->payWeekCard.m_iEndTime < (INT32)GameTime::GetTime() || XCommon::IsDaySame((time_t)pRecord->payWeekCard.m_iEndTime, GameTime::GetTime()))
		{
			SSWarn<<"didn't exist pay week card info"<<roArg.type()<<END;
			return ;
		}

		//判断今天是否已经领取过;
		if (XCommon::IsDaySame(GameTime::GetTime(), pRecord->payWeekCard.m_iLastGetAwardTime))
		{
			SSWarn<<"already got week card award today"<<roArg.type()<<END;
			return ;
		}
		nDays = PayV2Mgr::Instance()->GetRemainedDays(pRecord->payWeekCard.m_iEndTime)-1;
		nDiamondCount = CPayConfig::Instance()->GetCardAward(KKSG::WEEK_CARD);
		if (nDiamondCount && !pRole->AddDiamond(nDiamondCount, ItemFlow_Pay, ItemFlow_BuyPayCard))
		{
			SSWarn<<"add diamond send to ms error"<<END;
			return;

		}
		pInfo->set_type(KKSG::WEEK_CARD);
		pInfo->set_remainedcount(nDays);
		pInfo->set_isget(true);
		pRecord->payWeekCard.m_iLastGetAwardTime = (INT32)GameTime::GetTime();
	}
	else
	{
		//判断是否开通过月卡
		if(pRecord->payMonthCard.m_iEndTime < (INT32)GameTime::GetTime() || XCommon::IsDaySame((time_t)pRecord->payMonthCard.m_iEndTime, GameTime::GetTime()))
		{
			SSWarn<<"didn't exist pay month card info"<<roArg.type()<<END;
			return ;
		}

		//判断今天是否已经领取过
		if (XCommon::IsDaySame(GameTime::GetTime(), pRecord->payMonthCard.m_iLastGetAwardTime))
		{
			SSWarn<<"already got month card award today"<<roArg.type()<<END;
			return ;
		}

		//判断领取次数是否足够
		nDiamondCount = CPayConfig::Instance()->GetCardAward(KKSG::MONTH_CARD);
		if (nDiamondCount && !pRole->AddDiamond(nDiamondCount, ItemFlow_Pay, ItemFlow_BuyPayCard))
		{
			SSWarn<<"add diamond send to ms error"<<END;
			return;
		}
		else
		{
			nDays = PayV2Mgr::Instance()->GetRemainedDays(pRecord->payMonthCard.m_iEndTime)-1;
			pInfo->set_type(KKSG::MONTH_CARD);
			pInfo->set_remainedcount(nDays);
			pInfo->set_isget(true);
			pRecord->payMonthCard.m_iLastGetAwardTime = (INT32)GameTime::GetTime();
		}
	}
	roRes.set_errcode(ERR_SUCCESS);
	pRecord->m_isModified.Set();
	
}

void RpcC2G_PayCardAward::OnDelayReplyRpc(const PayCardAwardArg &roArg, PayCardAwardRes &roRes, const CUserData &roUserData)
{
}
