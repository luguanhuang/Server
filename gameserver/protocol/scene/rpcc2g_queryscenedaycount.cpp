#include "pch.h"
#include "scene/rpcc2g_queryscenedaycount.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/stagemgr.h"

// generate by ProtoGen at date: 2015/5/27 19:51:17

RPC_SERVER_IMPLEMETION(RpcC2G_QuerySceneDayCount, QuerySceneDayCountArg, QuerySceneDayCountRes)

void CallbackFun(void *ptr, UINT32 groupid, UINT32 count, UINT32 buycount)
{
	QuerySceneDayCountRes *poRes = (QuerySceneDayCountRes *)ptr;
	poRes->add_sceneid(groupid);
	poRes->add_scenecout(count);
	poRes->add_scenebuycount(buycount);
}

void RpcC2G_QuerySceneDayCount::OnCall(const QuerySceneDayCountArg &roArg, QuerySceneDayCountRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	StageMgr *pStageMgr = pRole->Get<StageMgr>();
	pStageMgr->Update();
	std::set<UINT32> groupids;
	if (0 == roArg.groupid_size())
	{	
		pStageMgr->ForeachSceneCount(CallbackFun, &roRes, groupids);
	}
	else
	{
		for (int i = 0; i < roArg.groupid_size(); ++i)
		{
			groupids.insert(roArg.groupid(i));
		}
		pStageMgr->ForeachSceneCount(CallbackFun, &roRes, groupids);
	}
	
	const std::vector<UINT32> &OpenedChestScene = pStageMgr->GetChestOpenedScene();
	for (UINT32 i = 0; i < OpenedChestScene.size(); ++i)
	{
		roRes.add_chestopenedscene(OpenedChestScene[i]);
	}
}

void RpcC2G_QuerySceneDayCount::OnDelayReplyRpc(const QuerySceneDayCountArg &roArg, QuerySceneDayCountRes &roRes, const CUserData &roUserData)
{
}
