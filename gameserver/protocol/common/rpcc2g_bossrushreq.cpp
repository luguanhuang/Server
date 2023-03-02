#include "pch.h"
#include "common/rpcc2g_bossrushreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bossrushmgr.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/7/11 21:48:22

RPC_SERVER_IMPLEMETION(RpcC2G_BossRushReq, BossRushArg, BossRushRes)

void RpcC2G_BossRushReq::OnCall(const BossRushArg &roArg, BossRushRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_ret(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_ret(KKSG::ERR_SUCCESS);

	switch(roArg.type())
	{
	//case KKSG::BOSSRUSH_RESULT_WIN:
	//	{
	//		BossRushMgr::Instance()->FinishOneCount(pRole, true);
	//		int leftcount = BossRushMgr::Instance()->LeftBossRushCount(pRole);
	//		roRes.set_leftcount(leftcount);
	//	}
	//	break;
	//case KKSG::BOSSRUSH_RESULT_FAILED:
	//	{
	//		BossRushMgr::Instance()->FinishOneCount(pRole, false);
	//		int leftcount = BossRushMgr::Instance()->LeftBossRushCount(pRole);
	//		roRes.set_leftcount(leftcount);
	//	}
	//	break;
	case KKSG::BOSSRUSH_REQ_BASEDATA:
		{
			BossRushMgr::Instance()->FillRoleStatus(pRole, roRes.mutable_data());	
			int leftcount = BossRushMgr::Instance()->LeftBossRushCount(pRole);
			roRes.set_leftcount(leftcount);
		}
		break;
	case KKSG::BOSSRUSH_REQ_REFRESH:
		{
			//要判断可以不可以刷新
			//if(0 != BossRushMgr::Instance()->LeftRefreshCount(pRole))
			if(BossRushMgr::Instance()->MakeRefreshCost(pRole))
			{
				BossRushMgr::Instance()->RefreshData(pRole);
				BossRushMgr::Instance()->AddRefreshCount(pRole);
				BossRushMgr::Instance()->FillRoleStatus(pRole, roRes.mutable_data());
				TBossRushRefreshFlow oLog(pRole);
				oLog.m_nCurRefreshCount = BossRushMgr::Instance()->GetCurRefreshCount(pRole);
				oLog.m_iCurBossCount = BossRushMgr::Instance()->GetCurRank(pRole);
				oLog.Do();
			}	
			else
			{
				roRes.set_ret(ERR_LACKCOIN);
			}
		}
		break;
	case KKSG::BOSSRUSH_REQ_APPEARANCE:
		{
			BossRushMgr::Instance()->FillBossAppearance(pRole, roRes.mutable_bossapp());
			int leftcount = BossRushMgr::Instance()->LeftBossRushCount(pRole);
			roRes.set_leftcount(leftcount);
		}
		break;
	case KKSG::BOSSRUSH_REQ_LEFTCOUNT:
		{
			int leftcount = BossRushMgr::Instance()->LeftBossRushCount(pRole);
			roRes.set_leftcount(leftcount);
		}
		break;
	case KKSG::BOSSRUSH_REQ_CONTINUE:
		{
			BossRushMgr::Instance()->OnRoleCountinue(pRole);
		}
		break;
	default:
		break;
	}
}

void RpcC2G_BossRushReq::OnDelayReplyRpc(const BossRushArg &roArg, BossRushRes &roRes, const CUserData &roUserData)
{
}
