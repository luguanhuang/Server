#include "pch.h"
#include "garden/rpcc2m_gardenbanquet.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "scene/scenemanager.h"
#include "garden/garden_utility.h"
#include "garden/ptcm2c_gardenbanquetntf.h"
#include "loghelper/tlogr.h"
#include "task/roletaskmgr.h"

// generate by ProtoGen at date: 2016/10/13 19:17:33

RPC_SERVER_IMPLEMETION(RpcC2M_GardenBanquet, GardenBanquetArg, GardenBanquetRes)

	void RpcC2M_GardenBanquet::OnCall(const GardenBanquetArg &roArg, GardenBanquetRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	auto cfg = GardenConfig::Instance()->GetBanquet().GetByBanquetID(roArg.banquet_id());
	if (cfg == NULL)
	{
		roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
		SSWarn<<"cfg is  NULL, banquet_id:"<<roArg.banquet_id()<<END;
		return;
	}

	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	if (garden->GetBanquetStatus())
	{
		roRes.set_result(KKSG::ERR_STATE_ERROR);
		SSWarn<<" banquet is running "<<roArg.banquet_id()<<END;
		return ;
	}

	OneOwnerScene* onescene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,role->GetID());
	UINT32 scene_uid = (NULL == onescene) ? 0 : onescene->sceneid;

	RpcM2G_GardenPlayEvent* msg = RpcM2G_GardenPlayEvent::CreateRpc();
	msg->m_oArg.set_rpcid(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_scene_uid(scene_uid);
	msg->m_oArg.set_event_type(KKSG::BANQUET);

	auto it = cfg->Stuffs.begin();
	auto it_end = cfg->Stuffs.end();
	for (; it != it_end; ++it)
	{
		KKSG::ItemBrief* item = msg->m_oArg.add_banquet_items();
		item->set_itemid(it->seq[0]); 
		item->set_itemcount(it->seq[1]);
	}	

	GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);

}

void RpcC2M_GardenBanquet::OnDelayReplyRpc(const GardenBanquetArg &roArg, GardenBanquetRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	GardenPlayEventRes* res = (GardenPlayEventRes*)roUserData.m_pUserPtr;
	if (res == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"GardenPlayEventRes res  is null :"<<m_sessionID<<END;
		return;
	}
	if ( KKSG::ERR_SUCCESS  !=  res->result())
	{
		return ;
	}

	std::vector<UINT64> quests;
	std::vector<UINT64> all;
	for (int i = 0; i < res->guests_size(); ++i)
	{
		all.push_back(res->guests(i));
	}

	UINT32 count = res->guests_size();
	if (count > GetGlobalConfig().GardenBanquetGuests)
	{
		GardenUtility::Instance()->FriendOrderByDegree(role,all,quests);
	}
	else
	{
		quests.assign(all.begin(),all.end());
	}

	GardenUtility::Instance()->NotifyBanquetQuests(role->GetID(),roArg.banquet_id(),quests);
	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	garden->ServeBanquet(roArg.banquet_id(),quests);

	roRes.set_banquet_id(roArg.banquet_id());
	roRes.set_result(KKSG::ERR_SUCCESS);

	for (auto it = quests.begin(); it != quests.end(); ++it)
	{
		if (*it == role->GetID())
		{
			RoleTaskMgr::Instance()->UpdateTask(role->GetID(),KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_Banquet);
		}
		else
		{
			RoleTaskMgr::Instance()->UpdateTask(*it,KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_JoinBanquet);
		}
	}
	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_uGardenID = garden->GetGardenID();
	oLog.m_uBanquetID = roArg.banquet_id();
	oLog.m_iOp = TX_Garden_BanquetPlay;
	oLog.Do();
}
