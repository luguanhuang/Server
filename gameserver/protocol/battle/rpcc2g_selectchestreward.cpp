#include "pch.h"
#include "battle/rpcc2g_selectchestreward.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/stagereward.h"
#include "gamelogic/teamreward.h"

// generate by ProtoGen at date: 2015/3/25 10:38:01

RPC_SERVER_IMPLEMETION(RpcC2G_SelectChestReward, SelectChestArg, SelectChestRes)


void RpcC2G_SelectChestReward::OnCall(const SelectChestArg &roArg, SelectChestRes &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (pRole->GetFlag(RSF_ISSelectChest))
	{
		SSWarn<<"role:"<<pRole->GetID()<<" has selected"<<END;
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	// valid
	if (!TeamSelectReward::ValidIndex(roArg.chestidx()))
	{
		SSWarn<<"role:"<<pRole->GetID()<<" invalid index:"<<roArg.chestidx()<<END;
		roRes.set_errcode(KKSG::ERR_FAILED);
		return;
	}

	int i = 0;
	Scene *pScene = pRole->GetCurrScene();
	if (pScene != NULL && pScene->GetSceneState() == SCENE_WIN)
	{
		pRole->SetFlag(RSF_ISSelectChest, true);

		TeamSelectReward* teamSelectReward = pScene->GetTeamSelectReward();
		if (teamSelectReward && pScene->IsCanDrawBox())
		{
			if (teamSelectReward->Select(pRole->GetID(),roArg.chestidx()))
			{
				roRes.set_errcode(KKSG::ERR_SUCCESS);
				return;
			}
		}
	}
	roRes.set_errcode(KKSG::ERR_FAILED);
}

void RpcC2G_SelectChestReward::OnDelayReplyRpc(const SelectChestArg &roArg, SelectChestRes &roRes, const CUserData &roUserData)
{
}
