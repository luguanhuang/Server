#include "pch.h"
#include "spactivity/rpcc2g_tactenterscene.h"
#include "config.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivitybase.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitydatasmall.h"
#include "globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2017/7/24 13:28:07

RPC_SERVER_IMPLEMETION(RpcC2G_TactEnterScene, TactEnterSceneArg, TactEnterSceneRes)

void RpcC2G_TactEnterScene::OnCall(const TactEnterSceneArg &roArg, TactEnterSceneRes &roRes)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}


	SceneInfo *pInfo = SceneConfig::Instance()->FindSceneInfo(roArg.sceneid());
	if (pInfo == NULL || pInfo->m_pConf == NULL)
	{
		SSWarn<<"scene info is NULL, sceneid:"<<roArg.sceneid()<<END;
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	bool isFind = false;
	auto& list = GetGlobalConfig().ThemeActivityScene;
	for (auto it = list.begin(); it != list.end(); it ++)
	{
		if (*it == pInfo->m_pConf->type)
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		SSWarn<<"invalid sceneid=" << roArg.sceneid() << ", scenetype=" << pInfo->m_pConf->type << END;
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	
	if (roArg.actid() == SpActivity_Duck)
	{
		if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_DUCK))
		{
			roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
			return;
		}

		SpActivityDataBase* pData = pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_Duck);
		if (pData == NULL)
		{
			LogWarn("DuckData is NULL, role [%llu]", pRole->GetID());
			roRes.set_result(KKSG::ERR_UNKNOWN);
			return;
		}
		UINT32 itemid = GetGlobalConfig().DuckTickets.seq[0];
		UINT32 itemcount = GetGlobalConfig().DuckTickets.seq[1];
		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_SpActivity, ItemFlow_DuckTickets);
		if (!trans.TakeItem(itemid, itemcount))
		{
			trans.RollBack();
			roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		trans.NotifyClient();
	}
	else
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	bool ret = SceneManager::Instance()->ChangeScene(pRole, roArg.sceneid(), pRole->GetID());
	if (!ret)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
}

void RpcC2G_TactEnterScene::OnDelayReplyRpc(const TactEnterSceneArg &roArg, TactEnterSceneRes &roRes, const CUserData &roUserData)
{
}
