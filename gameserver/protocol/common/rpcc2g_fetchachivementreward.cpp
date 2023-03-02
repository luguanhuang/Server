#include "pch.h"
#include "common/rpcc2g_fetchachivementreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/AchivementMgr.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2015/2/4 19:49:48

RPC_SERVER_IMPLEMETION(RpcC2G_FetchAchivementReward, FetchAchiveArg, FetchAchiveRes)

void RpcC2G_FetchAchivementReward::OnCall(const FetchAchiveArg &roArg, FetchAchiveRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, sessionid:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SysPPTRewardId)
		|| !pRole->Get<CRoleSystem>()->IsSystemOpened(SysPPTRewardId))
	{
		SSInfo<<"system close"<<END;
		roRes.set_result(ERR_FAILED);
		return;
	}

	//////> system open
	///if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysPPTRewardId) || 
	///	OpenSystemMgr::Instance()->IsIdipCloseSystem(SysLevelRewardId)) 
	///{
	///	SSInfo<<"system close"<<END;
	///	roRes.set_result(KKSG::ERR_FAILED);
	///	return;
	///}

	KKSG::ErrorCode err = (KKSG::ErrorCode)AchivementMgr::Instance()->DoCompleteAchivement(pRole, roArg.achivementid());
	roRes.set_result(err);
}

void RpcC2G_FetchAchivementReward::OnDelayReplyRpc(const FetchAchiveArg &roArg, FetchAchiveRes &roRes, const CUserData &roUserData)
{
}
