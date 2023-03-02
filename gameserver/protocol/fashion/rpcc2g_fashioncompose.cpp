#include "pch.h"
#include "fashion/rpcc2g_fashioncompose.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/fashionmgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/ibshopmgr.h"
#include "gamelogic/bagtransition.h"
#include "util/gametime.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/reportdatamgr.h"

// generate by ProtoGen at date: 2015/6/29 19:31:30

RPC_SERVER_IMPLEMETION(RpcC2G_FashionCompose, FashionComposeArg, FashionComposeRes)

	void RpcC2G_FashionCompose::OnCall(const FashionComposeArg &roArg, FashionComposeRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	///> system open
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_FASHION_COMPOSE))
	{
		SSInfo<<"system close"<<END;
		roRes.set_result(ERR_SYS_NOTOPEN);
		return;
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		LogError("cross server FashionCompose  roleid=%llu", pRole->GetID());
		roRes.set_result(ERR_FAILED);
		return;
	}

	UINT32 suit_id  =  FashionMgr::Instance()->GetFashionSuitID(roArg.fashion_id());
	if (suit_id == 0)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	auto suit_cfg = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
	if (suit_cfg == NULL)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if ( pRole->GetLevel() < suit_cfg->ShowLevel)
	{
		SSWarn<<" level to low to see  this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_FASHIONCOMPOSE_LEVEL_REQ);
		return;
	}	

	auto config_info =  FashionMgr::Instance()->GetFashinSynthesisinfo(roArg.fashion_id());
	if (config_info == NULL)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	RoleFashion* record = pRole->Get<RoleFashion>();	

	UINT32  FashionComposeTimeCD = GetGlobalConfig().GetInt("FashionComposeTimeCD" , 2);
	UINT32 last_time = pRole->Get<RoleFashion>()->GetFashionComposeTime();
	UINT32 now_time = (UINT32)GameTime::GetTime();
	if (now_time - last_time <  FashionComposeTimeCD)
	{
		SSWarn<<" too quick "<<END;
		roRes.set_result(ERR_WEDDING_OPER_CD);
		return;
	}

	// check
	UINT64 uid1 = convert<UINT64>(roArg.uid1());
	UINT64 uid2 = convert<UINT64>(roArg.uid2());
	if (uid1 == uid2)
	{
		SSWarn<<" same fahion  ,uid1 :"<<uid1 <<END;
		roRes.set_result(ERR_FASHION_NOT_EXIST);
		return;
	}	
	FashionInfo* fashion1 = record->FindFashionByUId(uid1);
	FashionInfo* fashion2 = record->FindFashionByUId(uid2);
	if (NULL == fashion1 || NULL == fashion2)
	{
		SSWarn<<"uid1:"<<uid1<<" "<<fashion1<<" uid2:"<<uid2<<" "<<fashion2<<END;
		roRes.set_result(ERR_FASHIONCOMPOSE_FAILED);
		return;
	}

	// pos
	FashionConf *pConf1 = ItemConfig::Instance()->GetFashionConf(fashion1->itemID);
	FashionConf *pConf2 = ItemConfig::Instance()->GetFashionConf(fashion2->itemID);
	if (NULL == pConf1 || NULL == pConf1 || pConf1->EquipPos != pConf2->EquipPos)
	{
		SSWarn<<"pos invalid"<<END;
		roRes.set_result(ERR_FASHIONCOMPOSE_POS);
		return;
	}

	// quality
	ItemConf* conf1 = ItemConfig::Instance()->GetItemConf(fashion1->itemID);
	ItemConf* conf2 = ItemConfig::Instance()->GetItemConf(fashion2->itemID);
	//ItemConf* conf_fashion = ItemConfig::Instance()->GetItemConf(roArg.fashion_id());
	if (NULL == conf1 || NULL == conf2 || conf1->ItemQuality   != suit_cfg->CraftedItemQuality || conf2->ItemQuality  != suit_cfg->CraftedItemQuality)
	{
		SSWarn<<"quality invalid"<<END;
		roRes.set_result(ERR_FASHIONCOMPOSE_QUALITY);
		return;
	}
	// time
	if (0 != conf1->TimeLimit || 0 != conf2->TimeLimit)
	{
		SSWarn<<"time limit error"<<END;
		roRes.set_result(ERR_FASHIONCOMPOSE_TIMELIMIT);
		return;
	}

	bool succes = false;
	UINT32 success_rate =  config_info->SuccessRate + record->GetFashionSynthesisFaildCount(roArg.fashion_id()) * config_info->FashinSynthesisAddSucessRate;
	UINT32 random = XRandom::randInt(1,101);
	if (random <= success_rate)
	{
		succes = true;		
	}

	SSInfo<<"role id:"<<pRole->GetRoleID() << " need random :"<<success_rate <<"and get rand is : " << random <<END;

	std::string strbill = TTag::Instance()->GetTagString();
	TFashionComopseFlow oLog(pRole);
	oLog.m_FashionId = roArg.fashion_id();
	oLog.m_FashionId1 = fashion1->itemID;
	oLog.m_FashionId2 = fashion2->itemID;
	oLog.m_TotalSuccessRate = success_rate;
	oLog.m_FailRate = record->GetFashionSynthesisFaildCount(roArg.fashion_id()) * config_info->FashinSynthesisAddSucessRate;
	oLog.m_Result = succes?0:1;
	oLog.m_FashionUid1 = uid1;
	oLog.m_FashionUid2 = uid2;
	oLog.Do();

	if (succes)
	{	
		// item consume reason
		// consume
		if (!record->DelFashion(uid1, ItemFlow_Fashion, ItemFlow_Fashion_Compose, strbill))
		{
			SSWarn<<"delete uid1 error"<<END;
			roRes.set_result(ERR_FASHIONCOMPOSE_FAILED);
			return;
		}

		if (!record->DelFashion(uid2, ItemFlow_Fashion, ItemFlow_Fashion_Compose, strbill))
		{
			SSWarn<<"delete uid2 error"<<END;
			roRes.set_result(ERR_FASHIONCOMPOSE_FAILED);
			return;
		}

		// give
		BagGiveItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Fashion, ItemFlow_Fashion_Compose);
		trans.SetTransTag(strbill);
		trans.GiveItem(roArg.fashion_id(),1);
		trans.NotifyClient();	

		record->ClearFashionSynthesisByID(roArg.fashion_id());
		roRes.set_result(ERR_SUCCESS);	

		record->AddComposeSuccessNum(suit_cfg->SuitQuality);
	}
	else
	{		
		if (!record->DelFashion(fashion2->uid, ItemFlow_Fashion, ItemFlow_Fashion_Compose, strbill))
		{
			SSWarn<<"delete uid1 error"<<END;
			roRes.set_result(ERR_FASHIONCOMPOSE_FAILED);
			return;
		}

		roRes.set_result(ERR_FASHIONCOMPOSE_FAILED);
		record->AddFashionSynthesisByID(roArg.fashion_id());	

		BagGiveItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Fashion, ItemFlow_Fashion_Compose);
		trans.SetTransTag(strbill);
		for (auto it = config_info->ReturnItems.begin(); it != config_info->ReturnItems.end(); ++it)
		{
			trans.GiveItem(it->seq[0],it->seq[1]);
		}
		trans.NotifyClient();		
		record->AddComposeFailedNum();
	}

	ReportDataMgr::Instance()->FashionCompose(pRole, succes, suit_cfg->SuitQuality);

	pRole->Get<RoleFashion>()->SetFashionComposeTime(now_time);
}

void RpcC2G_FashionCompose::OnDelayReplyRpc(const FashionComposeArg &roArg, FashionComposeRes &roRes, const CUserData &roUserData)
{
}
