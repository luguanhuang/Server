#include "pch.h"
#include "unit/rolemanager.h"
#include "pay/rpcc2g_payclick.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/payconfig.h"
#include <time.h>

// generate by ProtoGen at date: 2016/8/17 17:29:57

RPC_SERVER_IMPLEMETION(RpcC2G_PayClick, PayClickArg, PayClickRes)

void RpcC2G_PayClick::OnCall(const PayClickArg &roArg, PayClickRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	switch (roArg.buttontype())
	{
		case KKSG::PAY_PARAM_CARD:
			pRecord->payCardButtonStatus = KKSG::PAY_BUTTON_CLICK;
			break;
		case KKSG::PAY_PARAM_AILEEN:
			pRecord->payAileenButtonStatus = KKSG::PAY_BUTTON_CLICK;
			break;
		case KKSG::PAY_PARAM_FIRSTAWARD:
			pRecord->m_uPayFirstAwardButtonStatus = KKSG::PAY_BUTTON_CLICK;
			break;
		case KKSG::PAY_PARAM_GROWTH_FUND:
			pRecord->m_uGrowthFundButtonStatus = KKSG::PAY_BUTTON_CLICK;
			break;
		case KKSG::PAY_PARAM_MEMBER:
			{
				auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(roArg.memberid()));
				if (it != pRecord->memberInfo.end())
				{
					it->set_buttonstatus(KKSG::PAY_BUTTON_CLICK);
				}
				else
				{
					KKSG::PayMemberRecord roMemInfo;
					roMemInfo.set_id(roArg.memberid());
					roMemInfo.set_isclick(true);
					roMemInfo.set_expiretime(0);
					roMemInfo.set_buttonstatus(KKSG::PAY_BUTTON_CLICK);
					pRecord->memberInfo.push_back(roMemInfo);
				}
			}
			break;
		default:
			break;
	}
	pRecord->m_isModified.Set();
	roRes.set_errcode(ERR_SUCCESS);
	roRes.set_paycardfirstclick(pRecord->payCardButtonStatus==KKSG::PAY_BUTTON_CLICK?true:false);
	roRes.set_payaileenfirstclick(pRecord->payAileenButtonStatus==KKSG::PAY_BUTTON_CLICK?true:false);
	roRes.set_payfirstawardclick(pRecord->m_uPayFirstAwardButtonStatus==KKSG::PAY_BUTTON_CLICK?true:false);
	roRes.set_growthfundclick(pRecord->m_uGrowthFundButtonStatus==KKSG::PAY_BUTTON_CLICK?true:false);
	INT32 nowTime = INT32(time(NULL));
	for (auto i = pRecord->memberInfo.begin(); i != pRecord->memberInfo.end(); i ++)
	{
		KKSG::PayMember* pMemInfo = roRes.add_info();
		pMemInfo->set_id(i->id());
		pMemInfo->set_isclick(i->buttonstatus()==KKSG::PAY_BUTTON_CLICK?true:false);
		pMemInfo->set_expiretime(nowTime>i->expiretime()?0:i->expiretime()-nowTime);
	}
}

void RpcC2G_PayClick::OnDelayReplyRpc(const PayClickArg &roArg, PayClickRes &roRes, const CUserData &roUserData)
{
}
