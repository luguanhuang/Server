#include "pch.h"
#include "absparty/rpcc2g_absenterscene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/expeditionconfigmgr.h"
#include "scene/scenemanager.h"
#include "table/AbyssPartyListTable.h"
#include "define/itemdef.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/bag.h"
#include "gamelogic/abysspartymgr.h"


// generate by ProtoGen at date: 2017/5/6 16:51:28

RPC_SERVER_IMPLEMETION(RpcC2G_AbsEnterScene, AbsEnterSceneArg, AbsEnterSceneRes)

void RpcC2G_AbsEnterScene::OnCall(const AbsEnterSceneArg &roArg, AbsEnterSceneRes &roRes)
{
	if(GSConfig::Instance()->IsCrossGS()==true)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_error(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}


	const AbyssPartyListTable::RowData* pRowData = ExpeditionConfigMgr::Instance()->GetAbssParty(roArg.id());
	if (pRowData == NULL)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}


	bool bRet = AbyssPartyMgr::Instance()->CheckLevel(pRole,pRowData->AbyssPartyId);
	if (bRet ==  false)
	{
		roRes.set_error(KKSG::ERR_ABYSS_LEVEL);
		return;
	}

	UINT32 nSceneID =  ExpeditionConfigMgr::Instance()->GetRandomScene(pRowData->RandomID);
	if (nSceneID == 0)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	UINT32 nDiff =  pRole->Get<StageMgr>()->GetAbyssDiff(pRowData->AbyssPartyId);
	if (nDiff<pRowData->Index)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	std::map<UINT32,UINT32> mapNeedItem;
	std::vector<UINT32> vecNeedItem;
	AbyssPartyMgr::Instance()->GetNeedItem(pRowData->ID/100,vecNeedItem);
	UINT32 nNeedSize = pRowData->Cost[1];//需要
	for (size_t i = 0; i  < vecNeedItem.size(); i++)
	{
		UINT32 nHasItem = pRole->Get<Bag>()->CountItemInBag(vecNeedItem[i]);
		if(nHasItem>0)
		{
			if (nHasItem>=nNeedSize)
			{
				mapNeedItem.insert(std::pair<UINT32,UINT32>(vecNeedItem[i],nNeedSize));
				nNeedSize = 0;
				break;
			}else
			{
				nNeedSize -= nHasItem;
				mapNeedItem.insert(std::pair<UINT32,UINT32>(vecNeedItem[i],nHasItem));
			}
			if (nNeedSize==0)
			{
				break;
			}
		}
	}

	if (nNeedSize!=0)
	{
		roRes.set_error(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}

	for (auto iter = mapNeedItem.begin(); iter!=mapNeedItem.end(); iter++)
	{
		BagTakeItemTransition stTakeTransition(pRole);
		stTakeTransition.SetReason(ItemFlow_AbyssParty, ItemFlow_AbyssPartyTick);
		if(false == stTakeTransition.TakeItem(iter->first, iter->second))
		{
			stTakeTransition.RollBack();
			roRes.set_error(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		stTakeTransition.NotifyClient();
	}

	bRet = SceneManager::Instance()->ChangeScene(pRole, nSceneID, pRole->GetID(),pRowData->ID);
	if (bRet == false)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
}

void RpcC2G_AbsEnterScene::OnDelayReplyRpc(const AbsEnterSceneArg &roArg, AbsEnterSceneRes &roRes, const CUserData &roUserData)
{
}
