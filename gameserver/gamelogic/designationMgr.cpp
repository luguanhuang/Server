#include "pch.h"
#include "pb/project.pb.h"
#include "item.h"
#include "unit/role.h"
#include "arenarecord.h"
#include "designationMgr.h"
#include "designationRecord.h"
#include "designation/ptcg2c_coverdesignationntf.h"
#include "designation/ptcg2c_reachdesignationntf.h"
#include "define/systemiddef.h"
#include "timeutil.h"
#include "util/gametime.h"
#include "bag.h"
#include "scene/sceneconfig.h"
#include "table/EquipSuitTable.h"
#include "itemconfig.h"
#include "rolefashion.h"
#include "table/PowerPointMgr.h"
#include "unit/calcattribute.h"
#include "flowerrecord.h"
#include "pkrecord.h"
#include "pkmgr.h"
#include "towerrecord.h"
#include "table/TaskMgr.h"
#include "loginrecord.h"
#include "unit/rolemanager.h"
#include "rolesummarymgr.h"
#include "rolesummary.h"
#include "unit/roleoutlook.h"
#include "mail/mailsender.h"
#include "noticemgr.h"
#include "network/dblink.h"
#include "gamelogic/stagerankmgr.h"
#include "event/eventmgr.h"


INSTANCE_SINGLETON(DesignationMgr);


///>检测称号失效的时间间隔：5秒
static UINT32 check_designation_expire_interval = 10; 

DesignationMgr::DesignationMgr()
{
	m_dwLastUpTime = 0; 
}

DesignationMgr::~DesignationMgr()
{
	 
}

bool DesignationMgr::Init()
{
	bool bRet = LoadFile();
	return bRet;
}

void DesignationMgr::Uninit()
{
	ClearFile();
}

void DesignationMgr::ClearFile()
{
	m_designationTable.Clear();

}

bool DesignationMgr::CheckFile()
{
	DesignationTable tmpDesignationTable;
	if( !tmpDesignationTable.LoadFile("table/Designation.txt"))
	{
		SSWarn<<"load file Designation.txt failed"<<END;
		return false;
	}
	return true;
}

bool DesignationMgr::LoadFile()
{
	bool bRet = CheckFile();
	if (!bRet)
		return false;
	m_designationTable.Clear();
	if( !m_designationTable.LoadFile("table/Designation.txt"))
	{
		SSWarn<<"load file Designation.txt failed"<<END;
		return false;
	}
	LoadData();
	return true;
}

void DesignationMgr::HintNotify(Role* role)
{
	role->HintNotify(DesignationID_design, false);
}

///>首次进入场景红点
void DesignationMgr::FirstEnterSceneNotify(Role* pRole)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		return; 
	}
	if( record->m_mapDesignationInfo.size() == 0 )
	{
		return; 
	}
	
	if (record->IsCurAbilityDesignationMax())
	{
		return; 
	}

	///>SSInfo << "Current Ability Designation Not Max, HintNotify, RoleID=" << pRole->GetID() << END;
	HintNotify(pRole);
}

//获得新的称号推送;
void DesignationMgr::SendReachDesignationNotify(Role* pRole, UINT32 designationID)
{
	PtcG2C_ReachDesignationNtf ntf;
	ntf.m_Data.set_designationid(designationID);
	double ppt = this->GetDesignationPower(pRole->GetProfession(), designationID);
	pRole->Send(ntf);
	SSInfo << "Finish Send SC Message, CMD=PtcG2C_ReachDesignationNtf, " << "RoleID=" << pRole->GetID() << ", DesignationID=" << designationID << ", PPT=" << ppt << END;

	//判断是否最高战力，如果是，则发送红点;
	IsHigherCombat(pRole, designationID);
}



//推送封面称号;
void DesignationMgr::SendCoverDesignationNotify(Role* pRole)
{
	PtcG2C_CoverDesignationNtf ntf;
	
	UINT32 coverid = 0; 
	std::string covername = "";
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL != record )
	{
		coverid   = record->GetCoverDesignation();
		covername = record->GetName();
	}

	if( coverid > 0 )
	{
		DesignationTable::RowData* pData = m_designationTable.GetByID( coverid );
		if( NULL == pData )
		{
			record->DelDesignation(coverid);
			SSWarn << "Designation Delete From Table, Update Player Cover Designation To Zero, RoleID:" << pRole->GetID() << ", CoverID:" << coverid << END;
			return;
		}
	}

	ntf.m_Data.set_designationid(coverid);
	ntf.m_Data.set_desname(covername);
	pRole->Send(ntf);
}

//获取table;
DesignationTable* DesignationMgr::GetTable()
{
	return &m_designationTable;
}



void DesignationMgr::OnLevelUp(Role* pRole)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *> *pVec = GetConfigType(DESIGNATION_COM_TYPE_LEVEL);
	for(auto iter = pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0)
		{
			SSInfo << "complete type level have not param for level value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}		
		if(pRole->GetLevel() < pData->CompleteValue[0]){continue;}
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

///>计算战力
void DesignationMgr::ApplyEffect(Role* pRole, CombatAttribute* attr)
{
	if( NULL == attr )
	{
		return; 
	}

	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		SSError << "role designation record null" << " " << "RoleID=" << pRole->GetID() << END; 
		return; 
	}

	DesignationTable::RowData* pData = m_designationTable.GetByID(record->GetAbilityDesignation());
	if( NULL == pData ){return; }

	for( int i=0; i< (int)pData->Attribute.size(); i++ )
	{
		attr->AddAttr((CombatAttrDef)pData->Attribute[i][0], pData->Attribute[i][1]);
	}
}

void DesignationMgr::OnDelDesigantion(Role* pRole, UINT32 did)
{
	if (pRole==NULL)
		return;
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	SSInfo << "idip del role designation record " << " " << "RoleID=" << pRole->GetID() << "did:" << did <<END; 
	record->DelDesignation(did);
}



void DesignationMgr::OnAddGetDesigantion(Role* pRole, UINT32 did,UINT32 nTime)
{
	UINT64 roleid = pRole->GetID();
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		SSInfo << "role designation record null" << " " << "RoleID=" << roleid << END; 
		return; 
	}

	DesignationTable::RowData* pData = m_designationTable.GetByID(did);
	if( NULL == pData )
	{
		SSInfo << "designation config not exist, " << " " << "RoleID=" << roleid << ", DesignationID=" << did << END; 
		return; 
	}

	std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(did);
	if( itFind != record->m_mapDesignationInfo.end() )
	{
		SSInfo << "designation already exist, " << " " << "RoleID=" << roleid << ", DesignationID=" << did << END; 
		return; 
	}
	SSInfo << "add role designation record " << " " << "RoleID=" << pRole->GetID() << "did:" << did <<END; 
	stDesignationInfo stc; 
	stc.bIsNew = true; 
	stc.dwDesignationID = did;
	stc.dwReachTimestamp = nTime;
	record->AddDesignation(stc);
	this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
}

///>任意装备等级达到多少级
void DesignationMgr::OnAnyEquipLevel(Role* pRole, UINT32 equipLevel)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_ANY_EQUIP_LEVEL);
	UINT32 n = m_designationTable.Table.size();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( (int)equipLevel < pData->CompleteValue[0] ){continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

///>多少件装备套装: 拥有一某套装中的N件装备
void DesignationMgr::OnHaveSuitEquipCount(Role* pRole, UINT32 itemId)
{
	UINT32 nSuitID = ItemConfig::Instance()->GetEquipSuitID(itemId);
	if (nSuitID == (UINT32)-1)
		return;
	///>拥有几件该套装中的装备：equipSuit表中判断
	UINT32 diffEquipCount = ItemConfig::Instance()->GetDiffEquipCountInSuit(pRole,nSuitID);

	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_SUIT_EQUIP_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}

		UINT32 needCount = (UINT32)pData->CompleteValue[0];
		if( needCount > diffEquipCount )
			continue; 
		stDesignationInfo stc; 
		stc.dwDesignationID = (UINT32)pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}



///>获得某首饰套装中的N件首饰
void DesignationMgr::OnHaveJewelryCount(Role* pRole, UINT32 itemId)
{
	UINT32 nSuitID = ItemConfig::Instance()->GetEquipSuitID(itemId);
	if(nSuitID == (UINT32)-1)
	{
		return;
	}
	UINT32 nJewerlryID =  ItemConfig::Instance()->GetJewelrySuitID(nSuitID); 
	if(nJewerlryID == (UINT32)-1)
	{
		return;
	}
	UINT32 diffJewCount = ItemConfig::Instance()->GetDiffEquipCountInSuit(pRole, nJewerlryID);
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_JEWELRY_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Achievement.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		UINT32 needNum = (UINT32)pData->CompleteValue[0];
		if( needNum > diffJewCount ){continue; 	}
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

///>完成章节
void DesignationMgr::OnFinishChapter(Role* pRole, UINT32 sceneId)
{
	//取副本配置信息
	SceneInfo* pInfo = SceneConfig::Instance()->FindSceneInfo(sceneId);
	if( NULL == pInfo ){return; }
	//不是该章节最后一个副本
	if( !pInfo->m_IsEndofChapter ){return; }
	UINT32 chapId = pInfo->m_pConf->Chapter;
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FINISH_CHAPTER);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteType != DESIGNATION_COM_TYPE_FINISH_CHAPTER ){continue; }
		if( pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( chapId != pData->CompleteValue[0] ){continue; }
		//说明已经完成
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

//限时通关关卡
void DesignationMgr::OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( timespan > 180 ){return; }
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_LIMIT_TIME_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0] ){continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }

		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
	//如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
}

//3星通关关卡
void DesignationMgr::On3StarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star)
{
	if( star < 3 ){return; }	
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_3_STAR_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0)
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0] ){continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

//无伤通关
void DesignationMgr::OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent)
{
	if( hpPercent != 100 ){return; }
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_NO_HURT_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( sceneId != pData->CompleteValue[0] ){continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}
}

///>拥有多少金币
void DesignationMgr::OnHaveGoldCount(Role* pRole)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	UINT64 gold = pRole->Get<Bag>()->CountVirtualItem(GOLD);
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_HAVE_GOLD_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( gold < (UINT64)pData->CompleteValue[0] ){	continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		SSInfo << "Insert New Designation To Map, RoleID=" << pRole->GetID() << ", Did=" << stc.dwDesignationID << END;
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}

	//如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		//this->HintNotify(pRole);
	}

}

//获得时装
void DesignationMgr::OnGetFashion(Role* pRole, UINT32 fashionId)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_HAVE_FASHION_EQUIP);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for level value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		bool bRet = FashionMgr::Instance()->GetFashionSuit(fashionId,pData->CompleteValue[0]);
		if (!bRet)
			continue;
		if(!FashionMgr::Instance()->HaveEntireFashion(pRole, pData->CompleteValue[0]))
		{
			continue;
		}
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}

	//如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true

}


//1星通关关卡 
void DesignationMgr::OnOneStarFinishRaid(Role* pRole, UINT32 sceneId)
{
	bool haveNew = false; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_ONE_STAR_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete value size not right, RoleID=" << pRole->GetID() << "DesignationID=" <<pData->ID << END; 
			continue; 
		}
		if( sceneId != (UINT32)pData->CompleteValue[0] ){continue; }
		//说明已经完成
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		stDesignationInfo stc; 
		stc.dwDesignationID = pData->ID;
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.bIsNew = true; 
		record->AddDesignation(stc);
		SSInfo << "Insert New Designation To Map, RoleID=" << pRole->GetID() << ", Did=" << stc.dwDesignationID << END;
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);
		haveNew = true; 
	}

	//如果有新完成的称号，则推送红点 && 将该部分数据是否发生变更的标示设置为true
	if(haveNew)
	{
		//this->HintNotify(pRole);
	}
}

//是否获得更高战力的称号
void DesignationMgr::IsHigherCombat(Role* pRole, UINT32 designationId)
{
	SSInfo << "Begin Judge Higher Combat, RoleID=" << pRole->GetID() << ", DesignationID=" << designationId << END; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		return; 
	}
	if (record->GetMaxDesignationID() == designationId)
	{
		SSInfo << "Get Higher Combat Designation, RoleID=" << pRole->GetID() << ", DesignationID=" << designationId << END;
		HintNotify(pRole);
	}
}

UINT32 DesignationMgr::GetDesignationPower( UINT32 prof, UINT32 designationId )
{
	DesignationTable::RowData* pData = m_designationTable.GetByID(designationId);
	if( NULL == pData )
	{
		///>SSWarn << "Invalid Designation ID, Did=" << designationId << END;
		return 0; 
	}

	if (!IsValidDesignationByType(pData->Type))
	{
		return 0;
	}

	CombatAttribute combatAttribute;
	for( int i=0; i< (int)pData->Attribute.size(); i++ )
	{
		combatAttribute.AddAttr((CombatAttrDef)pData->Attribute[i][0], pData->Attribute[i][1]);
	}

	CalcRoleAttribute roleAttribute;
	roleAttribute.CalcAttributeConvert(&combatAttribute, prof);
	double ppt = PowerPointMgr::Instance()->CalcPowerPoint(&combatAttribute, prof);
	return (UINT32)ppt;
}

///>获取最高战力
UINT32 DesignationMgr::GetMaxPPT(Role* pRole)
{
	UINT32 maxDesignationID = pRole->Get<DesignationRecord>()->GetMaxDesignationID();
	UINT32 maxPPt = GetDesignationPower(pRole->GetProfession(), maxDesignationID);
	return maxPPt;
}

///>获取大类红点
void DesignationMgr::GetBigTypeRedPoint(Role* pRole, std::vector<bool>& vecRedPoint)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		for( UINT32 i = DESIGNAION_BIG_TYPE_COMMON; i < DESIGNAION_BIG_TYPE_MAX; i++ )
		{
			vecRedPoint.push_back(false);
		}
		return; 
	}
	bool isCurMax = record->IsCurAbilityDesignationMax();
	if (isCurMax)
	{
		for( UINT32 i = DESIGNAION_BIG_TYPE_COMMON; i < DESIGNAION_BIG_TYPE_MAX; i++ )
		{
			vecRedPoint.push_back(false);
		}
		return;
	}
	else
	{
		UINT32 maxID = record->GetMaxDesignationID();
		DesignationTable::RowData* pData = m_designationTable.GetByID(maxID);
		if( NULL == pData )
		{
			LogError("reload data, need relogin, role:%llu, designationid:%u", pRole->GetID(), maxID);
			return; 
		}
		if (!IsValidDesignationByType(pData->Type))
		{
			LogError("reload data, need relogin, role:%llu, designationid:%u", pRole->GetID(), maxID);
			return; 
		}

		for( UINT32 i = DESIGNAION_BIG_TYPE_COMMON; i < DESIGNAION_BIG_TYPE_MAX; i++ )
		{
			if (pData->Type == i)
				vecRedPoint.push_back(true);
			else
				vecRedPoint.push_back(false);
		}
	}
}

///>刷新获得的称号
void DesignationMgr::UpdateDesignation(Role* pRole, UINT32 designationId,std::string &name,UINT32 oper,UINT32 nParam,UINT32 time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	DesignationTable::RowData* pData = m_designationTable.GetByID(designationId);
	if( NULL == pData )
		return; 
	stDesignationInfo stc; 
	stc.dwDesignationID = (UINT32)pData->ID;
	if (oper == GlobalEvent::EVENT_OP_ADD)
	{
		stc.dwReachTimestamp = GameTime::GetTime();
		stc.szName = name;
		if(time >0&&pData->Pragmaticality>0)
		{
			if(GameTime::GetTime()>time+pData->Pragmaticality)
				return;
			stc.dwReachTimestamp = time;
		}
		stc.bIsNew = true; 
		stc.dwType = nParam;
		record->AddDesignation(stc);
		this->SendReachDesignationNotify(pRole, stc.dwDesignationID);

	}else if (oper == GlobalEvent::EVENT_OP_DEL)
	{
		record->DelDesignation(designationId);
	}else if (oper == GlobalEvent::EVENT_OP_MODIFY)
	{
		record->SetName(designationId,name);
	}
}



///>刷新获得的称号
void DesignationMgr::UpdateDesignation(Role* pRole, UINT32 designationId, UINT32 time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	DesignationTable::RowData* pData = m_designationTable.GetByID(designationId);
	if( NULL == pData )
	{
		SSWarn << "Invalid DesignationID, RoleID=" << pRole->GetID() << ", DesignationID=" << designationId << END;
		return; 
	}

	stDesignationInfo stc; 
	stc.dwDesignationID = (UINT32)pData->ID;
	stc.dwReachTimestamp = GameTime::GetTime();
	if(time >0&&pData->Pragmaticality>0)
	{
		if (time+pData->Pragmaticality<GameTime::GetTime())
		{
			return;
		}
		stc.dwReachTimestamp = time;
	}
	stc.bIsNew = true; 
	record->AddDesignation(stc);
	SSInfo << "Insert New Designation To Map, RoleID=" << pRole->GetID() << ", Did=" << stc.dwDesignationID << END;
	this->SendReachDesignationNotify(pRole, stc.dwDesignationID);

	///>如果是昨日鲜花称号则发送邮件86400
	if( pData->CompleteType == DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY )
	{
		MailSender sender;
		sender.SetContent(Mail_System,  GetGlobalConfig().DesignationFlowerMailTitle,  GetGlobalConfig().DesignationSendFlowerMailContent);
		sender.Send(pRole->GetID());
		SSInfo << "Send Mail To Client, MailType=LostFlowerDesignationMail, " << "RoleID=" << pRole->GetID() << END; 		
	}
}

///>钻石达到数量
void DesignationMgr::OnHaveDiamondCount(Role* pRole)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	UINT64 diamond = pRole->Get<Bag>()->CountVirtualItem(DIAMOND);
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_DIAMOND_COUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0)
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( diamond < (UINT64)pData->CompleteValue[0] ){continue; }
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//加入公会
void DesignationMgr::OnJoinGuild(Role* pRole)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_JOIN_GUILD);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(pRole, pData->ID);
	}
}

//双人无死亡通关
void DesignationMgr::OnTwoNoDieFinishRaid(Role* pRole, UINT32 sceneId, UINT32 noDieNum)
{
	if( noDieNum < 2 ){return; }
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_TWO_NO_DIE_FINISH_RAID);
	for (auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if((int)sceneId != pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//独自通关
void DesignationMgr::OnAloneFinishRaid(Role* pRole, UINT32 sceneId, UINT32 roleCount)
{
	if( roleCount != 1 ){return; }
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_ALONE_FINISH_RAID);
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	for (auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if( pData->CompleteValue.size() == 0 ){continue; }
		if( (int)sceneId != pData->CompleteValue[0] ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}

//四人无死亡通关
void DesignationMgr::OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FOUR_NO_DIE_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue;}
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( (int)sceneId != pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//3S高评分通关
void DesignationMgr::On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star)
{
	if( star != 3 ){return; }
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_3S_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue;}
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( (int)sceneId != pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//通关输出最高
void DesignationMgr::OnMostHurtFinishRaid(Role* pRole, UINT32 sceneId)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_MOST_HURT_FINISH_RAID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue;}
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( (int)sceneId != pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}


///>天梯赛获胜场次
void DesignationMgr::OnSkyLadderWinTimes(Role* pRole,UINT32 nWin)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_SKY_LADDER_WIN_TIMES);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find( pData->ID );
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( nWin < pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

///>天梯赛积分
void DesignationMgr::OnSkyLadderScore(Role* pRole,UINT32 nPoint)
{
	if (!pRole||nPoint<2500) return;
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_SKY_LADDER_SCORE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( nPoint < pData->CompleteValue[0] ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}


///>天梯赛积分
void DesignationMgr::On2V2SkyLadderScore(Role* pRole,UINT32 nPoint)
{
	if (!pRole||nPoint<2500) return;
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_2V2SKY_LADDER_SCORE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( nPoint < pData->CompleteValue[0] ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}


///>到达黑暗神殿层数
void DesignationMgr::OnTowerFloor(Role* pRole,UINT32 nFloor)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	CTowerRecord* towerrecord = pRole->Get<CTowerRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_TOWER_FLOOR);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() < 2 ){continue; }
		if(pData->CompleteValue[1] > nFloor ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}

//完成任务获得
void DesignationMgr::OnFinishTask(Role* pRole, UINT32 taskId )
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FINISH_TASK);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if(taskId != pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//连续登陆天数
void DesignationMgr::OnTotalLogin(Role* pRole)
{
	MyLoginRecord* loginrecord = pRole->Get<MyLoginRecord>();
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_TOTAL_LOGIN);
	int totalLoginDay = loginrecord->GetRealLoginDayCount();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0){continue; }
		if( (int)totalLoginDay < pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//充值数额
void DesignationMgr::OnRechargeAmount(Role* pRole, UINT32 totalPay)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_RECHARGE_AMOUNT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( (int)totalPay < pData->CompleteValue[0] ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}

///>抽次充值获得
void DesignationMgr::OnFirstRecharge(Role* pRole)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FIRST_RECHARGE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID );
		if( itFind != record->m_mapDesignationInfo.end() ){continue; }
		UpdateDesignation(pRole, pData->ID);
	}
}

//昨日榜;
void DesignationMgr::OnFlowerRank(Role *pRole,UINT32 nRank, int time)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )continue;
		if( nRank != pData->CompleteValue[1])continue; 
		UpdateDesignation(pRole, pData->ID, XCommon::GetDayBeginTime(time));
	}
}

//历史榜;
void DesignationMgr::OnFlowerHistoryRank(Role *pRole,UINT32 nRank, int time)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0){continue; }
		if( nRank != pData->CompleteValue[1] ){continue; }
		UpdateDesignation(pRole, pData->ID, XCommon::GetDayBeginTime(time));
	}
}


//历史榜;
void DesignationMgr::OnGuilActivityRank(Role *pRole,UINT32 nRank, int time)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 ){continue; }
		if( nRank !=pData->CompleteValue[0] ){continue; }
		UpdateDesignation(pRole, pData->ID, XCommon::GetDayBeginTime(time));
	}
}



//无死亡通关;
void DesignationMgr::OnNoDieFinishRaid( Role* pRole, UINT32 sceneId )
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_NO_DIE_FINISH_RAID);
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end()){continue;}
		if(pData->CompleteValue.size() == 0 ){continue; }
		if((int)sceneId != pData->CompleteValue[0] ){continue;}
		UpdateDesignation(pRole, pData->ID);
	}
}

//参与首次击杀公会boss;
void DesignationMgr::OnFirstTimeKillGuildBoss( UINT64 roleID, UINT32 bossID )
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 ){continue;}
		if((int)bossID != pData->CompleteValue[0] ){continue; }
		if (pRole)
		{
			DesignationRecord* record = pRole->Get<DesignationRecord>();
			auto itFind = record->m_mapDesignationInfo.find(pData->ID);
			if(itFind != record->m_mapDesignationInfo.end()){continue;}
			UpdateDesignation(pRole, pData->ID);
		}
		else
			//AddDesignationToDB(roleID, pData->ID);
			EventMgr::Instance()->AddEvent(roleID,  DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS, bossID);
		break; 
	}
}

//显示所有获得的称号;
void DesignationMgr::OnShowDesignation(Role* pRole, std::string mark)
{
	///>昨日榜名单
	for( UINT32 i=0; i<m_vecFlowerRank.size(); i++ )
	{
		UINT32 rank = i+1;
		UINT64 roleID = m_vecFlowerRank[i];
		SSWarn << "Flower Yesterday Rank Info, Rank:" << rank << "  RoleID:" << roleID << END;
	}
	///>历史榜名单
	for( UINT32 i=0; i<m_vecHistoryFlowerRank.size(); i++ )
	{
		UINT32 rank = i+1;
		UINT64 roleID = m_vecHistoryFlowerRank[i];
		SSWarn << "Flower History Rank Info, Rank:" << rank << "  RoleID:" << roleID << END;
	}

	///>历史榜名单
	if( NULL == pRole )
	{
		return;
	}
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		return; 
	}
	UINT64 roleID = pRole->GetID();
	UINT32 flowerDid = 0; 
	UINT32 historyFlowerDid = 0; 
	for( auto i=record->m_mapDesignationInfo.begin(); i != record->m_mapDesignationInfo.end(); i++ )
	{
		DesignationTable::RowData* pData= m_designationTable.GetByID( i->first );
		if( NULL == pData )
		{
			continue;
		}
		if( pData->CompleteType == DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY )
		{
			flowerDid = i->first;
			continue;
		}
		if( pData->CompleteType == DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY )
		{
			historyFlowerDid = i->first;
			continue;
		}
	}
	SSDebug << mark <<  ", Designation Info, RoleID:" << pRole->GetID() << ", FlowerDid:" << flowerDid << ", HistoryFlowerDid:" << historyFlowerDid << ", CoverDid:" << record->GetCoverDesignation()<< END;

	///>显示玩家所有的称号
	for( auto i=record->m_mapDesignationInfo.begin(); i!= record->m_mapDesignationInfo.end(); i++ )
	{
		UINT32 did = i->first;
		UINT32 time = i->second.dwReachTimestamp;
		std::string datetime = TimeUtil::GetDateTimeStr( time );
		SSInfo << "Player Designation Info, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", DesignationID:" << did << ", Time:" << time << ", DateTime:" << datetime << END;
	}
}


//登录时更新玩家鲜花称号;
void DesignationMgr::LoginUpdateFlowerDesignation( Role* pRole )
{
	if( NULL == pRole ){return; }
	UpdateExpireDesignation(pRole);
}


//设置封面称号
void DesignationMgr::SetCoverDesignation( Role* pRole, UINT32 coverDid )
{
	if( NULL == pRole ){return; }
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	record->SetCoverDesignation(coverDid);
	SSWarn << "Update Player Cover Designation, RoleID:" << pRole->GetID() << ", CoverID:" << record->GetCoverDesignation()<< END;
}

///>战力改变
void DesignationMgr::OnPPT( Role* pRole )
{
	UINT32 ppt = pRole->GetAttr(TOTAL_POWERPOINT);
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_PPT);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		auto itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end() ){continue; }
		if(pData->CompleteValue.size() == 0 ){continue;}
		UINT32 needPPT = pData->CompleteValue[0];
		if( ppt < needPPT ){continue; }
		UpdateDesignation(pRole,pData->ID);
	}
}

void DesignationMgr::UpdateExpireDesignation(Role* pRole)
{
	UINT32 now = GameTime::GetTime();
	//if( m_dwLastUpTime + check_designation_expire_interval > now ){return; }
	m_dwLastUpTime = now; 
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<UINT32> vecDid;
	vecDid.clear();
	for( auto i=record->m_mapDesignationInfo.begin(); i != record->m_mapDesignationInfo.end(); i++ )
	{
		DesignationTable::RowData* pData = m_designationTable.GetByID( i->first );
		if( NULL == pData ){continue; }
		if( pData->Pragmaticality == 0 ){continue; }
		if( i->second.dwReachTimestamp + pData->Pragmaticality > GameTime::GetTime() ) {continue; }
		SSInfo << "Designation Expire, Need Remove, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", Did:" << i->first << ", ReachTime:" 
			<< i->second.dwReachTimestamp << ", Now:" << GameTime::GetTime() << ", PargmaticallityTime:" << pData->Pragmaticality << ", ExpireSecond:" << 
			GameTime::GetTime() - i->second.dwReachTimestamp - pData->Pragmaticality << END;
		vecDid.push_back( i->first );
	}
	for( UINT32 i=0; i<vecDid.size(); i++ )
	{
		UINT32 did = vecDid[i];
		record->DelDesignation(did);
		SSInfo << "Delete Expire Designation, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", RemoveDid:" << did << END;
	}
}


void DesignationMgr::DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent)
{
	if (pEvent->neventid() == DESIGNATION_COM_TYPE_GUILD_GROUP_CHAMPION)
	{
		DesignationMgr::Instance()->OnGuildArenaChampion(pRole,pEvent->nparam1(), pEvent->ntime());
	}
	else if(pEvent->neventid() == DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY)
	{
		DesignationMgr::Instance()->OnFlowerRank(pRole, pEvent->nparam1(), pEvent->ntime());
	}
	else if(pEvent->neventid() == DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY)
	{
		DesignationMgr::Instance()->OnFlowerHistoryRank(pRole, pEvent->nparam1(), pEvent->ntime());
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_FIRSTPASS)
	{
		DesignationMgr::Instance()->OnFirstPass(pRole,pEvent->nparam1(), pEvent->ntime());
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK)
	{
		DesignationMgr::Instance()->OnGuilActivityRank(pRole,pEvent->nparam1(), pEvent->ntime());
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_PK_WEEK_RANK)
	{
		DesignationMgr::Instance()->OnPkWeekRank(pRole->GetID(), pEvent->nparam1(), pEvent->ntime());
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_ARENA_RANK)
	{
		DesignationMgr::Instance()->OnArenaRank(pRole, pEvent->nparam1(), pEvent->ntime());
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS)
	{
		DesignationMgr::Instance()->OnFirstTimeKillGuildBoss(pRole->GetID(), pEvent->nparam1());
	}				
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_WEEK_ASSIST)
	{
		DesignationMgr::Instance()->AssistWeekDesignation(pRole, pEvent->nparam1(), pEvent->ntime());					
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_GARDEN)
	{
		DesignationMgr::Instance()->OnGarden(pRole, pEvent->nparam1());		
	}else if (pEvent->neventid() == DESIGNATION_COM_NESTWEEK)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_NESTWEEK,pEvent->ntime(),pEvent->nparam1());
	}else if (pEvent->neventid() == DESIGNATION_COM_ORDER)
	{
		DesignationMgr::Instance()->OrderDesignation(pRole,DESIGNATION_COM_ORDER,pEvent->nparam1(),pEvent->ntime());
	}else if(pEvent->neventid() == DESIGNATION_COM_TGACHAMPTION)
	{
		DesignationMgr::Instance()->OrderDesignation(pRole,DESIGNATION_COM_TGACHAMPTION,pEvent->nparam1(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_GUILDTERRITORYBATTLE)
	{
		DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_GUILDTERRITORYBATTLE,pEvent->nparam1(),pEvent->nparam2(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_LEAGUELICHAMPTION)
	{
		DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_LEAGUELICHAMPTION,pEvent->nparam1(),pEvent->nparam2(),pEvent->ntime());
	}else if(pEvent->neventid() == DESIGNATION_COM_MENTOR)
	{
		DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_MENTOR,pEvent->nparam1(),pEvent->nparam2(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_MILITARY)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_MILITARY,pEvent->ntime(),pEvent->nparam1());
	}else if (pEvent->neventid() == DESIGNATION_COM_QQEXCLUSIVE)
	{
		DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_QQEXCLUSIVE,pEvent->nparam1(),pEvent->nparam2(),pEvent->ntime());
	}else if(pEvent->neventid() == DESIGNATION_COM_TYPE_2V2PK_WEEK_RANK)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_TYPE_2V2PK_WEEK_RANK,pEvent->ntime(),pEvent->nparam1());			
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_SKYCITY)
	{
		DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_TYPE_SKYCITY,pEvent->nparam1(),pEvent->nparam2(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_BIGMELEE)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_TYPE_BIGMELEE,pEvent->ntime(),pEvent->nparam1());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_BIOHELLL)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_TYPE_BIOHELLL,pEvent->ntime(),pEvent->nparam1()); 	
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_TOWERRANK)
	{
		DesignationMgr::Instance()->TwoConditionsDesignation(pRole,DESIGNATION_COM_TYPE_TOWERRANK,pEvent->ntime(),pEvent->nparam1()); 	
	}
	else if (pEvent->neventid() == DESIGNATION_COM_TYPE_MARRY)
	{
		DesignationMgr::Instance()->OneMarry(pRole, pEvent->neventid(),pEvent->nparam1(),pEvent->nop(),pEvent->name(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_IDIP_ADD)
	{
		DesignationMgr::Instance()->OnAddGetDesigantion(pRole, pEvent->nparam1(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_IDIP_DEL)
	{
		DesignationMgr::Instance()->OnDelDesigantion(pRole, pEvent->nparam1());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_COMPETEDRAGONRANK)
	{
		DesignationMgr::Instance()->OnAddGetDesigantion(pRole, pEvent->nparam1(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_GOALAWARDS)
	{
		DesignationMgr::Instance()->OnAddGetDesigantion(pRole, pEvent->nparam1(),pEvent->ntime());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_DRAGONGUILD)
	{
		DesignationMgr::Instance()->OnDragonGuild(pRole, DESIGNATION_COM_TYPE_DRAGONGUILD, pEvent->nop(), pEvent->name());
	}else if (pEvent->neventid() == DESIGNATION_COM_TYPE_SURVIVE)
	{
		DesignationMgr::Instance()->OnAddGetDesigantion(pRole, pEvent->nparam1(),pEvent->ntime());
	}
}

///>公会擂台战冠军
void DesignationMgr::OnGuildArenaChampion(Role *pRole,UINT32 nWarType, int time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_GUILD_GROUP_CHAMPION);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( nWarType != (UINT32)pData->CompleteValue[0] ) continue; 
		UpdateDesignation(pRole, pData->ID, time);
	}
}

///>天梯赛排名
void DesignationMgr::OnPkWeekRank( UINT64 roleID, UINT32 rank , int time)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_PK_WEEK_RANK);
	UINT32 nID = 0; 
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() < 2 )
			continue; 
		UINT32 startRank = pData->CompleteValue[0];
		UINT32 endRank   = pData->CompleteValue[1];
		if( rank >= startRank && rank <= endRank )
		{
			nID = pData->ID;
			break; 
		}
	}
	if( 0 == nID ){return; }
	Role* pRole = RoleManager::Instance()->FindByRoleID( roleID );
	if (!pRole){return;}
	UpdateDesignation(pRole, nID, time);
}

void DesignationMgr::OnArenaRank( Role* pRole, UINT32 rank, int time)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_ARENA_RANK);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0)
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( rank > (UINT32)pData->CompleteValue[0] )
			continue; 
		UpdateDesignation(pRole, pData->ID, XCommon::GetDayBeginTime(time));
	}
}

void DesignationMgr::AssistDesignation(Role *pRole, UINT32 nCount)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_ASSIST);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( nCount < (UINT32)pData->CompleteValue[0] )
			continue; 
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(pRole, pData->ID);
	}
}


void DesignationMgr::DargonAssistDesignation(Role *pRole, UINT32 nCount)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_DRAGONASSIST);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( nCount < (UINT32)pData->CompleteValue[0] )
			continue; 
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(pRole, pData->ID);
	}
}

void DesignationMgr::AssistWeekDesignation(Role *pRole, UINT32 nCount, int time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_WEEK_ASSIST);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( nCount < (UINT32)pData->CompleteValue[0] )
			continue; 
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		/*if(itFind != record->m_mapDesignationInfo.end())
			continue; */
		UpdateDesignation(pRole, pData->ID, time);
	}
}

// para1>=a
void DesignationMgr::TwoConditionsDesignation(Role *pRole, UINT32 nType ,int time,UINT32 nRank)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nType);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 2 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( nRank >= (UINT32)pData->CompleteValue[0]&&nRank<=(UINT32)pData->CompleteValue[1])
		{
			UpdateDesignation(pRole, pData->ID, time);
			break;
		}
	}
}

void DesignationMgr::OrderDesignation(Role *pRole,UINT32 nBigType,UINT32 nType,int time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nBigType);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 1)
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if(nType == pData->CompleteValue[0])
		{
			UpdateDesignation(pRole, pData->ID, time);
			break;
		}
	}
}


bool DesignationMgr::IsValidDesignationByType(UINT32 type)
{
	if (type >= DESIGNAION_BIG_TYPE_COMMON && type < DESIGNAION_BIG_TYPE_MAX)
	{
		return true;
	}
	return false;
}

void DesignationMgr::OnFirstPass(Role *pRole, UINT32 nSceneID, int time)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_TYPE_FIRSTPASS);
	for( auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if( nSceneID != (UINT32)pData->CompleteValue[0] )
			continue; 
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(pRole, pData->ID, time);
	}
}


void DesignationMgr::OnSceneFinished(Role* pRole, UINT32 nSceneTemplateID, const StageRankInfo& stageinfo)
{
	if (!pRole){return;}
	UINT32 raidStar = StageRankMgr::Instance()->GetStarByRank(stageinfo.rank);
	On3StarFinishRaid(pRole, nSceneTemplateID, raidStar);
	OnNoHurtFinishRaid(pRole, nSceneTemplateID, stageinfo.hppercent);
	OnLimitTimeFinishRaid(pRole, nSceneTemplateID, stageinfo.timespan);
	OnFinishChapter(pRole, nSceneTemplateID);
	OnOneStarFinishRaid(pRole, nSceneTemplateID);
	On3SFinishRaid(pRole, nSceneTemplateID, raidStar);

}


void DesignationMgr::OnBeginMind(Role *pRole,UINT32 nID)
{
	if (!pRole){return;}
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(nID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(pRole, nID);
	}
}


void DesignationMgr::OnPassDragonScene(Role*  role,UINT32 nExpID)
{
	if (!role){return;}
	DesignationRecord* record = role->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_DRAGON_SCENE);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 1 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" <<pData->ID << END; 
			continue; 
		}
		if( nExpID != (UINT32)pData->CompleteValue[0] )
			continue; 
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(role, pData->ID);
	}
}




void DesignationMgr::OnGay(Role*  role)
{
	if (!role){return;}
	DesignationRecord* record = role->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(DESIGNATION_COM_GAY);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
		if(itFind != record->m_mapDesignationInfo.end())
			continue; 
		UpdateDesignation(role, pData->ID);
	}
}


void DesignationMgr::OnUseItem(Role*  role,UINT32 nDesID)
{
	if (!role)
		return;
	DesignationRecord* record = role->Get<DesignationRecord>();
	DesignationTable::RowData *pRowData = m_designationTable.GetByID(nDesID);
	if (pRowData == NULL)
		return;
	std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pRowData->ID);
	if(itFind == record->m_mapDesignationInfo.end())
		UpdateDesignation(role, pRowData->ID);
}


//param1,time
void DesignationMgr::OnCommonType(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nBigType);
	for( auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() == 0 )
		{
			SSInfo << "complete type level have not param for gold value field" << " table=Designation.txt, " << "ID=" << pData->ID << END; 
			continue; 
		}
		if((pData->CompleteValue.size() == 1&&nParam1 == (UINT32)pData->CompleteValue[0])||	\
			(pData->CompleteValue.size() == 2&&nParam1 == (UINT32)pData->CompleteValue[0]&&nParam2==(UINT32)pData->CompleteValue[1]))
		{
			UpdateDesignation(pRole, pData->ID, nTime);
		}
	}
}




void DesignationMgr::OnGarden(Role * role, UINT32 event_type)
{
	LogDebug("On garden event_type :%d ",event_type);

	DesignationRecord* record = role->Get<DesignationRecord>();
	auto vec  = GetConfigType(DESIGNATION_COM_TYPE_GARDEN);
	auto it = vec->begin();
	auto it_end = vec->end();

	for(; it != it_end; ++it)
	{
		if ((*it)->CompleteValue.size() < 1)
		{
			SSInfo << "OnGarden value field" << " table=Designation.txt, " << "ID=" <<(*it)->ID << END; 
			continue;
		}

		if( event_type   !=  (UINT32)(*it)->CompleteValue[0] )
		{
			continue;
		}
		auto  it_find = record->m_mapDesignationInfo.find((*it)->ID);
		if( it_find != record->m_mapDesignationInfo.end())
		{
			continue;
		}

		UpdateDesignation(role,(*it)->ID);
	}
}


void DesignationMgr::LoadData()
{
	m_mapConfig.clear();
	for (auto iter = m_designationTable.Table.begin();iter!=  m_designationTable.Table.end();iter++)
	{
		DesignationTable::RowData *pData = *iter;
		UINT32 nType = pData->CompleteType;
		m_mapConfig[nType].push_back(pData);
	}
}


std::vector<DesignationTable::RowData *>* DesignationMgr::GetConfigType(UINT32 nType)
{
	return &m_mapConfig[nType];
}


DesignationTable::RowData * DesignationMgr::GetRowData(UINT32 nID)
{
	 return m_designationTable.GetByID(nID);
}


std::string DesignationMgr::GetCoverName(Role *pRole)
{
	DesignationRecord* pRecord = pRole->Get<DesignationRecord>();
	std::string szName = pRecord->GetName();
	return szName;

}

//param1==a, param2>=b
void DesignationMgr::OnEqualBiger(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime,bool bCheck)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nBigType);
	for( auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pRowData = *iter;
		if(pRowData->CompleteValue.size() != 2)
			continue;
		if(nParam1 == (UINT32)pRowData->CompleteValue[0]&&nParam2>=(UINT32)pRowData->CompleteValue[1])
		{
			if (bCheck==true)
			{
				std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pRowData->ID);
				if(itFind == record->m_mapDesignationInfo.end())
				{
					UpdateDesignation(pRole, pRowData->ID, nTime);
				}

			}else
			{
				UpdateDesignation(pRole, pRowData->ID, nTime);
			}

		}
	}
}



//param1,time parm1==a &&param2==b or parm1==a
void DesignationMgr::OnEqualComTime(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime,bool bCheck)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nBigType);
	for( auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pRowData = *iter;
		if(pRowData->CompleteValue.size() == 0 )
			continue; 
		if((pRowData->CompleteValue.size() == 1&&nParam1 == (UINT32)pRowData->CompleteValue[0])||	\
			(pRowData->CompleteValue.size() == 2&&nParam1 == (UINT32)pRowData->CompleteValue[0]&&nParam2==(UINT32)pRowData->CompleteValue[1]))
		{
			if (bCheck==true)
			{
				std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pRowData->ID);
				if(itFind == record->m_mapDesignationInfo.end())
				{
					UpdateDesignation(pRole, pRowData->ID, nTime);
				}

			}else
			{
				UpdateDesignation(pRole, pRowData->ID, nTime);
			}

		}
	}
}


void DesignationMgr::OnEventTypeCom(Role*  role,UINT32 nEventID,UINT32 nTime,bool bCheck)
{
	if (!role){return;}
	DesignationRecord* record = role->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if (bCheck)
		{
			std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(pData->ID);
			if(itFind == record->m_mapDesignationInfo.end())
				UpdateDesignation(role, pData->ID);
		}else
		{
			UpdateDesignation(role, pData->ID);
		}
	}
}


void DesignationMgr::TwoComCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime,bool bCheck)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 2 )
			continue; 
		if(nParam >= (UINT32)pData->CompleteValue[0]&&nParam<=(UINT32)pData->CompleteValue[1])
		{
			UpdateDesignation(pRole, pData->ID,nTime);
			break;
		}
	}
}


// para1>=a
void DesignationMgr::OneBigerCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 1 )
			continue; 
		if(nParam >= (UINT32)pData->CompleteValue[0])
		{
			UpdateDesignation(pRole, pData->ID,nTime);
			break;
		}
	}
}


//param>=a&&param<=b time
void DesignationMgr::OnTwoCompConditionTime(Role *pRole,UINT32 nEventID,UINT32 nParam,UINT32 nTime,bool bCheck)
{
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if(pData->CompleteValue.size() != 2 )
			continue;
		if( nEventID >= pData->CompleteValue[0] && nEventID <=  pData->CompleteValue[1]) 
			UpdateDesignation(pRole, pData->ID, nTime);
	}
}


void DesignationMgr::OneMarry(Role *pRole, UINT32 nEventID,UINT32 nParam ,UINT32 nOp,std::string szName,UINT32 nTime)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for(auto iter=pVec->begin(); iter!=pVec->end(); iter++)
	{
		DesignationTable::RowData *pData = *iter;
		if (pData)
		{
			UpdateDesignation(pRole, pData->ID, szName, nOp,nParam,nTime);
		}

	}
}

void DesignationMgr::OnDragonGuild(Role* pRole, UINT32 nEventID, UINT32 nOp, std::string szName)
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	std::vector<DesignationTable::RowData *>* pVec = GetConfigType(nEventID);
	for (auto iter = pVec->begin(); iter != pVec->end(); iter ++)
	{
		DesignationTable::RowData *pData = *iter;
		if (pData)
		{
			UpdateDesignation(pRole, pData->ID, szName, nOp, 0);
		}
	}
}