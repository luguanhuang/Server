#include "pch.h"
#include "garden/rpcc2m_opengardenfarmland.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/garden_utility.h"
#include "loghelper/tlogr.h"
// generate by ProtoGen at date: 2016/10/26 21:31:37

RPC_SERVER_IMPLEMETION(RpcC2M_OPenGardenFarmland, OpenGardenFarmlandArg, OpenGardenFarmlandRes)

	void RpcC2M_OPenGardenFarmland::OnCall(const OpenGardenFarmlandArg &roArg, OpenGardenFarmlandRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	// 目前只考虑自己的家园
	Garden* garden = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (roArg.farmland_id() > GetGlobalConfig().GardeLandDefault)
	{
		LogWarn(" farmland id err  ,the id is %d",roArg.farmland_id());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}


	//Rcit_const it_end = garden->GetPlantInfoStore().plantinfo_record.end();
	auto idit = garden->GetPlantInfoStore().allow_plantid.find(roArg.farmland_id());
	auto idit_end = garden->GetPlantInfoStore().allow_plantid.end();
	if  ( idit != idit_end)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}


	UINT32 itemId = 0;
	UINT32 itemCount = 0;
	UINT32 index = 0;
	int openland_range = GetGlobalConfig().GardeLandDefault - GetGlobalConfig().BreakFarmlandLevel.size();	
	int break_count = GetGlobalConfig().BreakFarmlandLevel.size();

	// 第几次开垦
	for ( int i =1; i < openland_range; ++i)
	{
		int farmland_id = i + break_count;
		if (farmland_id == roArg.farmland_id())
		{
			continue;
		}
		auto id_it = garden->GetPlantInfoStore().allow_plantid.find(farmland_id);
		if (id_it != idit_end)
		{
			index += 1;
		}
	}
		
	auto& itmeInfo   = GetGlobalConfig().BreakNewFarmlandCost[index];
	itemId = itmeInfo.seq[0];
	itemCount = itmeInfo.seq[1];
	if ( itemId > 0 &&  itemCount > 0)
	{
		// 扣除费用
		RpcM2G_MSConsumeItems* msg = RpcM2G_MSConsumeItems::CreateRpc();
		msg->m_oArg.set_rpc_id(DelayRpc());
		msg->m_oArg.set_role_id(role->GetID());
		msg->m_oArg.set_reason(ItemFlow_Garden);
		msg->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);
		KKSG::ItemBrief* item = msg->m_oArg.add_items();
		item->set_itemid(itemId); 
		item->set_itemcount(itemCount);

		GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);
	}
}

void RpcC2M_OPenGardenFarmland::OnDelayReplyRpc(const OpenGardenFarmlandArg &roArg, OpenGardenFarmlandRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if ( KKSG::ERR_SUCCESS  !=  (KKSG::ErrorCode)roUserData.m_dwUserData)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}

	// 目前只考虑自己的家园
	Garden* garden = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	garden->AddFarmland(roArg.farmland_id());

	roRes.set_result(KKSG::ERR_SUCCESS);
	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_uGardenID = garden->GetGardenID();
	oLog.m_iOp = Tx_Garden_OpenFarmland;
	oLog.Do();
}
