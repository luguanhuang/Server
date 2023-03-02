#include "pch.h"
#include "garden/rpcc2m_gardenbanquetaward.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "define/tlogenum.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/10/20 16:54:21

RPC_SERVER_IMPLEMETION(RpcC2M_GardenBanquetAward, BanquetAwardArg, BanquetAwardRes)

	void RpcC2M_GardenBanquetAward::OnCall(const BanquetAwardArg &roArg, BanquetAwardRes &roRes)
{
	//CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	//if (NULL == role)
	//{
	//	roRes.set_result(KKSG::ERR_INVALID_REQUEST);
	//	SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
	//	return;
	//}

	//GardenBanquetCfg::RowData* cfg = GardenConfig::Instance()->GetBanquet().GetByBanquetID(roArg.banquet_id());
	//if (cfg == NULL)
	//{
	//	roRes.set_result(KKSG::ERR_INVALID_REQUEST);
	//	SSWarn<<"cfg is  NULL, banquet_id:"<<roArg.banquet_id()<<END;
	//	return;
	//}

	////不是本人的家园，只有可能是朋友或者工会的
	//std::string garden_name = role->GetName();
	//if (roArg.garden_id() != role->GetID())
	//{
	//	GardenUtility::Instance()->GetGardenName(role->GetID(),roArg.garden_id(),garden_name);
	//}
	//Garden* garden = GardenManager::Instance()->GetGarden(roArg.garden_id(),garden_name);

	//std::set<UINT64> guests;
	//garden->GetBanquetGuests(guests);
	//std::set<UINT64>::iterator it = guests.find(role->GetID());
	//std::set<UINT64>::iterator it_end = guests.end();
	//if (it == it_end)
	//{
	//	//roRes.set_result(KKSG::err_)
	//	return;
	//}

	//// 奖品发给gs
	//PtcM2G_MSGiveItemNtf ntf;
	//ntf.m_Data.set_roleid(role->GetID());
	//ntf.m_Data.set_reason(ItemFlow_Garden);
	//ntf.m_Data.set_subreason(ItemFlow_Garden_Banquet);

	//vector<Sequence<uint, 2>>::iterator itAward = cfg->BanquetAwards.begin();
	//vector<Sequence<uint, 2>>::iterator itAward_end = cfg->BanquetAwards.end();
	//for (; itAward != itAward_end; ++itAward)
	//{
	//	KKSG::ItemBrief* item = ntf.m_Data.add_items();
	//	item->set_itemid(itAward->seq[0]);
	//	item->set_itemcount(itAward->seq[0]);
	//}

	//GSLink::Instance()->SendToLine(role->GetGsLine(), ntf);

	//roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_GardenBanquetAward::OnDelayReplyRpc(const BanquetAwardArg &roArg, BanquetAwardRes &roRes, const CUserData &roUserData)
{
}
