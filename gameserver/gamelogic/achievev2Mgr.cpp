#include "pch.h"
#include "item.h"
#include "bag.h"
#include "unit/role.h"
#include "arenarecord.h"
#include "achievev2Mgr.h"
#include "achievev2Record.h"
#include "designation/ptcg2c_coverdesignationntf.h"
#include "define/systemiddef.h"
#include "timeutil.h"
#include "achievement/ptcg2c_reachachieventf.h"
#include "scene/sceneconfig.h"
#include "itemconfig.h"
#include "mail/mailop.h"
#include "define/maildef.h"
#include "flowerrecord.h"
#include "pkrecord.h"
#include "towerrecord.h"
#include "loginrecord.h"
#include "define/eventdef.h"
#include "gamelogic/stagerankmgr.h"
#include "gamelogic/fashionmgr.h"

INSTANCE_SINGLETON(AchieveV2Mgr);

AchieveV2Mgr::AchieveV2Mgr()
{
}

AchieveV2Mgr::~AchieveV2Mgr()
{
}

bool AchieveV2Mgr::Init()
{
	bool bRet =CheckFile();
	if(!bRet)
		return false;
	LoadFile();
	return true;
}



bool AchieveV2Mgr::LoadFile()
{
	bool bRet = CheckFile();
	if (!bRet)
		return false;
	ClearFile();
	if( !m_achieveTable.LoadFile("table/Achievement.txt"))
	{
		SSWarn<<"load file Achievement.txt failed"<<END;
		return false;
	}

	if( !m_rewardTable.LoadFile("table/AchievementPointReward.txt") )
	{
		SSWarn<<"load file AchievementPointReward.txt failed"<<END;
		return false;
	}
	LoadData();
	return true;
}

void AchieveV2Mgr::ClearFile()
{
	m_achieveTable.Clear();
	m_rewardTable.Clear();
}

bool AchieveV2Mgr::CheckFile()
{
	AchievementV2Table		tmpAchievementV2Table;
	AchievementPointRewardTable	tmpAchievementPointRewardTable;
	if( !tmpAchievementV2Table.LoadFile("table/Achievement.txt"))
	{
		SSWarn<<"check file Achievement.txt failed"<<END;
		return false;
	}

	if( !tmpAchievementPointRewardTable.LoadFile("table/AchievementPointReward.txt") )
	{
		SSWarn<<"check file AchievementPointReward.txt failed"<<END;
		return false;
	}
	return true;
}

void AchieveV2Mgr::Uninit()
{
	ClearFile();
}


void AchieveV2Mgr::HintNotify(Role* role)
{
	AchieveV2Record* record = role->Get<AchieveV2Record>();
	if( NULL == record )
	{
		return; 
	}

	bool canReward = false;
	for( auto i=record->m_mapAchieveInfo.begin(); i!=record->m_mapAchieveInfo.end(); i++ )
	{
		if( i->second.dwRewardStatus != 1 )
		{
			continue; 
		}

		canReward = true;
		break; 
	}

	for( auto i=record->m_mapAchievePointRewardInfo.begin(); i != record->m_mapAchievePointRewardInfo.end(); i++ )
	{
		if( i->second.dwRewardStatus != 1 )
		{
			continue; 
		}

		canReward = true; 
		break; 
	}

	if(canReward)
	{
		role->HintNotify(DesignationID_achieve, false);
		///>SSInfo << "Finish Send SC Message, CMD=PtcG2C_HintNtf, SystemID=AchieveID, RoleID=" << role->GetID() << END;
	}
}

///>获得新的成就推送
void AchieveV2Mgr::SendReachAchieveNotify(Role* pRole, UINT32 achieveID)
{
	PtcG2C_ReachAchieveNtf ntf;
	ntf.m_Data.set_achieveid(achieveID);
	pRole->Send(ntf);
	SSInfo << "Finish Send SC Message, CMD=PtcG2C_ReachAchieveNtf, RoleID=" << pRole->GetID() << END;
}

///>获取table
AchievementV2Table* AchieveV2Mgr::GetTable()
{
	return &m_achieveTable;
}

///>获取奖励table
AchievementPointRewardTable* AchieveV2Mgr::GetRewardTable()
{
	return &m_rewardTable;
}

///>刷新获得成就
void AchieveV2Mgr::UpdateAchieve(Role* pRole, int achieveId)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record )
	{
		return; 
	}
	STC_ACHIEVE_V2_INFO stc; 
	stc.dwAchieveID = (UINT32)achieveId;
	stc.dwReachTimestamp = TimeUtil::GetTime();
	stc.dwRewardStatus = 1; 
	record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
	this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
	record->m_ismodify.Set();
}

void AchieveV2Mgr::OnJoinGuild(Role* pRole)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_JOIN_GUILD);
	UINT32 n = m_achieveTable.Table.size();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end())
			continue; 
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert(std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}


void AchieveV2Mgr::OnLevelUp(Role* pRole)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_LEVEL);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}		
		if( pRole->GetLevel() < pData->CompleteValue[0] ){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}


void AchieveV2Mgr::OnDelGetAchievement(Role* pRole, UINT32 aid)
{
	UINT64 roleid = pRole->GetID();
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record )
	{
		SSInfo << "role achievement record null" << " " << "RoleID=" << roleid << END; 
		return; 
	}

	AchievementV2Table::RowData* pData = m_achieveTable.GetByID(aid);
	if( NULL == pData )
	{
		SSInfo << "achievement config not exist, " << " " << "RoleID=" << roleid << ", AchievementID=" << aid << END; 
		return; 
	}

	std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(aid);
	if( itFind != record->m_mapAchieveInfo.end() )
	{
		LogInfo("del ach roleid=%llu aid=%u",roleid,aid);
		record->m_mapAchieveInfo.erase(aid);
		return; 
	}

}

///>GM指令获得称号
void AchieveV2Mgr::OnAddGetAchievement(Role* pRole, UINT32 aid)
{
	UINT64 roleid = pRole->GetID();
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();

	AchievementV2Table::RowData* pData = m_achieveTable.GetByID(aid);
	if( NULL == pData )
	{
		SSInfo << "achievement config not exist, " << " " << "RoleID=" << roleid << ", AchievementID=" << aid << END; 
		return; 
	}
	LogInfo("add ach roleid=%llu aid=%u",roleid,aid);
	std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(aid);
	if( itFind != record->m_mapAchieveInfo.end() )
	{
		SSInfo << "achievement already exist, " << " " << "RoleID=" << roleid << ", AchievementID=" << aid << END; 
		return; 
	}

	STC_ACHIEVE_V2_INFO stc; 
	stc.dwAchieveID = aid;
	stc.dwRewardStatus = 1; 
	stc.dwReachTimestamp = TimeUtil::GetTime();
	LogInfo("add ach roleid=%llu aid=%u",roleid,aid);
	record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc) );

	this->HintNotify(pRole);
	this->SendReachAchieveNotify(pRole, aid);

	record->m_ismodify.Set();
}


void AchieveV2Mgr::OnHaveSuitCount( Role* pRole, UINT32 itemID )
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	UINT32 nSuitID =  ItemConfig::Instance()->GetEquipSuitID(itemID);
	if (nSuitID == (UINT32)-1)
		return;
	///>拥有几件该套装中的装备：equipSuit表中判断
	UINT32 diffEquipCount = ItemConfig::Instance()->GetDiffEquipCountInSuit(pRole,nSuitID);
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_SUIT_COUNT);
	UINT32 n = m_achieveTable.Table.size();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		UINT32 needCount = (UINT32)pData->CompleteValue[0];
		if( needCount > diffEquipCount )
			continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end())
			continue; 
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnJewelrySuitCount( Role* pRole, UINT32 itemID )
{
	UINT32 nSuitID =  ItemConfig::Instance()->GetEquipInJewelrySuit(itemID);
	if(nSuitID == (UINT32)-1)
		return;

	UINT32 nJewelryID =   ItemConfig::Instance()->GetJewelrySuitID(nSuitID);
	if(nJewelryID == (UINT32)-1)
		return;

	UINT32 diffTypeEquipInSuit = ItemConfig::Instance()->GetDiffEquipCountInSuit(pRole,nSuitID);

	SSInfo << "begin to handle jewelry suit count, roleid=" << pRole->GetID() << ", suitid=" << nSuitID << ", diffTypeEquipCount:" << diffTypeEquipInSuit << END;
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_JEWELRY_SUIT_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( (int)diffTypeEquipInSuit < pData->CompleteValue[0] )
			continue;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnEquipLevel(Role* pRole, UINT32 level)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_EQUIP_LEVEL);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( (int)level < pData->CompleteValue[0] ) continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end())
			continue; 
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}


void AchieveV2Mgr::OnSuitLevel(Role* pRole, UINT64 equipUid, UINT32 level)
{
	Bag* pBag = pRole->Get<Bag>();
	if( NULL == pBag )
	{
		return ;
	}
	int pos = 0;
	XItem* pItem  = pBag->FindUniqueEquipOnBody(equipUid, &pos);
	if( NULL == pItem )
	{
		return ; 
	}
	if (XItem::IsJewelry(pos))
	{
		OnJewlerySuitLevel(pRole, equipUid, level);
		return;
	}

	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	SSInfo << "Handle Achievement OnSuitLevel, RoleID=" << pRole->GetID() << ", EquipUID=" << equipUid << ", EnhanceLevel=" << level << END;

	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_SUIT_LEVEL);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 ){continue; }

		int needLevel = pData->CompleteValue[0];
		int needCount =  pData->CompleteValue[1];
		if( 0 == ItemConfig::Instance()->GetEquipSuitLevel(pRole, equipUid, (UINT32)needLevel,(UINT32)needCount) ){continue; }

		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }

		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		SSInfo << "Insert Equip Suit Level OK Achievent, RoleID=" << pRole->GetID() << ", EquipUID=" << equipUid << ", AchieveID=" << stc.dwAchieveID << END;
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnJewlerySuitLevel(Role* pRole, UINT64 equipUid, UINT32 level)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	SSInfo << "Handle Achievement OnSuitLevel, RoleID=" << pRole->GetID() << ", EquipUID=" << equipUid << ", EnhanceLevel=" << level << END;

	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_JEWELRY_SUIT_LEVEL);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 ){continue; }

		int needLevel = pData->CompleteValue[0];
		int neeedCount =  pData->CompleteValue[1];
		if( 0 == ItemConfig::Instance()->GetEquipSuitLevel(pRole, equipUid, (UINT32)needLevel,neeedCount) ){continue; }

		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }

		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		SSInfo << "Insert Jewelry Suit Level OK Achievent, RoleID=" << pRole->GetID() << ", EquipUID=" << equipUid << ", AchieveID=" << stc.dwAchieveID << END;
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnChangeJob(Role* pRole)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_CHANGE_JOB);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	///>如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnFinishChapter(Role* pRole, UINT32 sceneId)
{
	SSInfo << "AchieveV2Mgr OnFinishChapter, RoleID=" << pRole->GetID() << ", SceneID=" << sceneId << END;
	SceneInfo* pInfo = SceneConfig::Instance()->FindSceneInfo(sceneId);
	if( NULL == pInfo ){return; }
	if( !pInfo->m_IsEndofChapter ){return; }
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	int chapId = pInfo->m_pConf->Chapter;
	bool haveNew = false; 
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_FINISH_CHAPTER);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSError << "config have no complete value, " << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( chapId != pData->CompleteValue[0] ){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
    	STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

///>3分钟通过
void AchieveV2Mgr::OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( timespan > 180 )	{return; }
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_LIMIT_TIME_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "Complete Value Param Not Enough" << ", table=Achievement.txt, " << "AchieveID=" << pData->ID << ", RoleID=" << pRole->GetID() << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0]) continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnStarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star)
{
	if( star < 3 ){return; }

	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_STAR_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}

}

void AchieveV2Mgr::OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent)
{
	if( hpPercent != 100 ){return; }
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record ){return; }
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_NO_HURT_FINISH_RAID);
	UINT32 n = m_achieveTable.Table.size();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}

void AchieveV2Mgr::OnOneStarFinishRaid(Role* pRole, UINT32 sceneId)
{
	bool haveNew = false; 
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_ONE_STAR_FINSIH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; 	}
		STC_ACHIEVE_V2_INFO stc; 
		stc.dwAchieveID = pData->ID;
		stc.dwReachTimestamp = TimeUtil::GetTime();
		stc.dwRewardStatus = 1; 
		record->m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc));
		this->SendReachAchieveNotify(pRole, stc.dwAchieveID);
		haveNew = true; 
	}
	//如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		record->m_ismodify.Set();
		this->HintNotify(pRole);
	}
}


void AchieveV2Mgr::OnGiveFlower(Role* pRole, int num)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	UINT32 total = num;
	UINT32 n = m_achieveTable.Table.size();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_GIVE_FLOWER);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() != 2 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)total < pData->CompleteValue[1]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}

}

void AchieveV2Mgr::OnFashionCount(Role* pRole)
{
	UINT32 count = FashionMgr::Instance()->GetFashionCount(pRole);
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_FASHION_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)count < pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end())	{continue; }
		UpdateAchieve(pRole, pData->ID);
	}
}

void AchieveV2Mgr::OnFashionQuality(Role* pRole, UINT32 fashionId)
{
	UINT32 quality = 0; 
	if(!FashionMgr::Instance()->GetFashionQuality(pRole, fashionId, quality))
	{
		SSError << "Get Fashion Quality Failed, RoleID=" << pRole->GetID() << END;
		return; 
	}
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_FASHION_QUALITY);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)quality < pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}
}

void AchieveV2Mgr::OnNoDieFinishRaid(Role* pRole, UINT32 sceneId)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_NO_DIE_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnAloneFinishRaid(Role* pRole, UINT32 sceneId)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_ALONE_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}


void AchieveV2Mgr::On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star)
{
	if( 3 != star ){return; }
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_3S_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)sceneId != pData->CompleteValue[0]){continue; }
		//说明已经完成
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_FOUR_NO_DIE_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)sceneId != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; 	}
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnPKWinTimes(Role* pRole,UINT32 nWin)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_PK_WIN_TIMES);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if(nWin < pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnPKScore(Role* pRole,UINT32 nPoint)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_PK_SCORE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 	
		}
		if( nPoint < pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnArenaRank(Role* pRole, UINT32 rank)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_ARENA_RANK);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)rank > pData->CompleteValue[0]) continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()) continue; 
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnTowerFloor(Role* pRole,UINT32 nFloor)
{
	CTowerRecord* towerrecord = pRole->Get<CTowerRecord>();
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_TOWER_FLOOR);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() < 2 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		int hardLevel = pData->CompleteValue[0];
		int needFloor = pData->CompleteValue[1];
		if( nFloor < needFloor ) continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; 	}
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnTotalLogin(Role* pRole)
{
	MyLoginRecord* loginrecord = pRole->Get<MyLoginRecord>();
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	int totalLoginDay = loginrecord->GetRealLoginDayCount();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_TOTAL_LOGIN);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" <<pData->ID << END; 
			continue; 
		}
		if( totalLoginDay < pData->CompleteValue[0] ) continue; 
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnRechargeAmount(Role* pRole, UINT32 totalPay)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_RECHARGE_AMOUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0)
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)totalPay < pData->CompleteValue[0] ){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnFirstRecharge(Role* pRole)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_FIRST_RECHARGE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue;}
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnAssistFight(Role *pRole,UINT32 nCount)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_TYPE_ASSIST);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		if((int)nCount <pData->CompleteValue[0]){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnSceneFinished(Role* pRole, UINT32 nSceneTemplateID, const StageRankInfo& stageinfo)
{
	if (!pRole){return;}
	UINT32 raidStar = StageRankMgr::Instance()->GetStarByRank(stageinfo.rank);
	OnStarFinishRaid(pRole, nSceneTemplateID, raidStar);
	OnNoHurtFinishRaid(pRole, nSceneTemplateID, stageinfo.hppercent);
	OnLimitTimeFinishRaid(pRole, nSceneTemplateID, stageinfo.timespan);
	OnFinishChapter(pRole, nSceneTemplateID);
	OnOneStarFinishRaid(pRole, nSceneTemplateID);
	On3SFinishRaid(pRole, nSceneTemplateID, raidStar);
}

void AchieveV2Mgr::LoadData()
{
	m_mapConfig.clear();
	for (auto iter = m_achieveTable.Table.begin();iter!=  m_achieveTable.Table.end();iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		UINT32 nType = pData->CompleteType;
		m_mapConfig[nType].push_back(pData);
	}
}

std::vector<AchievementV2Table::RowData *>* AchieveV2Mgr::GetConfigType(UINT32 nType)
{
	return &m_mapConfig[nType];
}

void AchieveV2Mgr::OnPassDragonScene(Role* pRole, UINT32 expID, UINT32 roleSize, UINT32 costTime)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	UINT32 n = m_achieveTable.Table.size();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_DRAGON_SCENE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if (pData->CompleteValue.empty())
		{
			continue;
		}
		if( expID != pData->CompleteValue[0]){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }

		bool bUpdate = false;
		if( pData->CompleteValue.size() == 1 )
		{
			bUpdate = true;
		}
		else if (pData->CompleteValue.size() == 2)
		{
			if (pData->CompleteValue[1] >= roleSize || !pData->CompleteValue[1])
				bUpdate = true;
		}
		else if (pData->CompleteValue.size() == 3)
		{
			if ((pData->CompleteValue[1] >= roleSize || !pData->CompleteValue[1]) && (pData->CompleteValue[2] >= costTime || !pData->CompleteValue[2]))
				bUpdate = true;
		}
		if (bUpdate)
		{
			UpdateAchieve(pRole, pData->ID);
		}
	}
}


void AchieveV2Mgr::OnMentor(Role* pRole, UINT32 nStudent)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_MENTOR);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 1)
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << ", AchieveID=" << pData->ID << END; 
			continue; 
		}
		if( (int)nStudent >= pData->CompleteValue[0] ){continue; }
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnMarry(Role* pRole)
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	vector<AchievementV2Table::RowData *> *pVec = GetConfigType(ACHIEVE_COM_MARRY);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		AchievementV2Table::RowData *pData = *iter;
		std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(pData->ID);
		if(itFind != record->m_mapAchieveInfo.end()){continue; }
		UpdateAchieve(pRole, pData->ID);
	}	
}

void AchieveV2Mgr::OnGarden(Role* pRole, UINT32 index)
{
	LogDebug("AchieveV2Mgr::OnGarden index %d ",index);

	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	auto vec  =  GetConfigType(ACHIEVE_COM_GARDEN);
	auto it = vec->begin();
	auto it_end = vec->end();
	for ( ; it != it_end; ++it)
	{
		if ((*it)->CompleteValue.size() < 1)
		{
			SSInfo << "OnGarden not right, RoleID=" << pRole->GetID() << ", AchieveID=" << (*it)->ID << END; 
			continue;
		}

		if ( index != (*it)->CompleteValue[0])
		{
			continue;
		}

		auto  it_find = record->m_mapAchieveInfo.find((*it)->ID);
		if (it_find != record->m_mapAchieveInfo.end()) 
		{
			continue;
		}

		UpdateAchieve(pRole, (*it)->ID);
	}
}


void AchieveV2Mgr::DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent)
{
	UINT32 nAch = pEvent->neventid() - ACH_EVENT_OFFSET;
	if (nAch == ACHIEVE_COM_TYPE_JOIN_GUILD)
	{
		AchieveV2Mgr::Instance()->OnJoinGuild(pRole);
	}
	else if (nAch == ACHIEVE_COM_TYPE_GIVE_FLOWER)
	{
		AchieveV2Mgr::Instance()->OnGiveFlower(pRole, pEvent->nparam1());
	}
	else if (nAch == ACHIEVE_COM_TYPE_ARENA_RANK)
	{
		AchieveV2Mgr::Instance()->OnArenaRank(pRole, pEvent->nparam1());
	}else if (nAch == ACHIEVE_COM_GARDEN)
	{
		AchieveV2Mgr::Instance()->OnGarden(pRole, pEvent->nparam1());		
	}else if(nAch == ACHIEVE_COM_MENTOR)
	{
		AchieveV2Mgr::Instance()->OnMentor(pRole, pEvent->nparam1());	
	}else if (nAch == ACHIEVE_COM_IDIP_ADD)
	{
		AchieveV2Mgr::Instance()->OnAddGetAchievement(pRole,pEvent->nparam1());
	}else if (nAch == ACHIEVE_COM_IDIP_DEL)
	{
		AchieveV2Mgr::Instance()->OnDelGetAchievement(pRole,pEvent->nparam1());
	}
}