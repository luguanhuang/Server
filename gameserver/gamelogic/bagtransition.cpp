#include "pch.h"
#include "bagtransition.h"
#include "unit/role.h"
#include "timeutil.h"
#include "scene/scene.h"
#include "fashionmgr.h"
#include "rolefashion.h"
#include "utility/loghelper.h"
#include "guild/guildmgr.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "util/XRandom.h"
#include "table/ProfessionMgr.h"
#include "stagemgr.h"
#include "team.h"
#include "achievev2Mgr.h"
#include "designationMgr.h"
#include "utility/uidhelper.h"
#include "LogInit.h"
#include "utility/tlogger.h"
#include "unit/role.h"
#include "unit/pet.h"
#include "config/itemdropconfig.h"
#include "spritetransition.h"
#include "taskrecord.h"
#include "idip/idiprecord.h"
#include "buff/buffrecord.h"
#include "common/ptcg2c_itemcircledrawresult.h"
#include "pandoraconfig.h"
#include "lotteryrecord.h"
#include "levelsealRecord.h"
#include "noticemgr.h"
#include "levelsealMgr.h"
#include "worldlevelmgr.h"
#include "unit/pet.h"
#include "unit/petconfigmgr.h"
#include "teamrecord.h"
#include "gamelogic/superriskrecord.h"
#include "herobattlerecord.h"
#include "designationRecord.h"
#include "servermgr/servercrossdatamgr.h"
#include "gamelogic/platformshare_record.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"
#include "event/eventmgr.h"
#include "buff/XBuffTemplateManager.h"

BagTransition::BagTransition(Role *pRole)
:m_pBag(pRole->Get<Bag>())
,m_pFashion(pRole->Get<RoleFashion>())
,m_pRole(pRole)
{
	m_pFashion->SetBagListener(&m_Record);
	m_actionType = INVALID_ACTION;
	m_isUsedNotify = true;
}

BagTransition::~BagTransition()
{
	m_pFashion->SetBagListener(NULL);
}

void BagTransition::NotifyClient()
{
	m_isUsedNotify = true;
	BeforeNotifyClient();
	m_Record.NotifyClient(m_pRole);
	AfterNotifyClient();
}

BagTakeItemTransition::BagTakeItemTransition(Role *pRole)
:BagTransition(pRole)
,m_nReason(-1)
,m_nSubReason(-1)
,m_nParam(0)
{
}

BagTakeItemTransition::~BagTakeItemTransition()
{
	for (auto it = m_ItemChangeCount.begin(); it != m_ItemChangeCount.end(); ++it)
	{
		m_pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Del, KKSG::TaskConn_ItemID, it->first, it->second);
	}

	if (!m_isUsedNotify)
	{
		LogError("Reason:%d, SubReason:%d, use TakeItemTransition, but not use NotifyClient or RollBack in the end", m_nReason, m_nSubReason);
		assert(false);
	}
}

bool BagTakeItemTransition::TakeItem(UINT32 itemID, int Count, ItemBind filter)
{
	if(Count == 0) 
	{
		return true;
	}
	if(Count < 0)
	{
		LogError("take bag item error, count < 0, itemID:%u, Count:%d", itemID, Count);
		return false;
	}
	//钻石托管在midas，存储的虚拟货币钻石只用来显示和简单预判使用,扣除钻石必须用特定的接口发往midas处理;
	if (itemID == DIAMOND)
	{
		LogError("take bag item error, itemID:%u, Count:%d", itemID, Count);
		return false;
	}

	m_isUsedNotify = false;
	if (XItem::IsVirtualItem(itemID))
	{
		return TakeVirtualItem(itemID, Count);
	}
	else
	{
		std::vector<XItem*> items;
		m_pBag->FindItemsInBag(itemID, items, filter);
		int sum = 0;
		for (auto i = items.begin(); i != items.end(); ++i)
		{
			sum += (*i)->itemCount;
			if(sum >= Count) break;
		}
		if (sum < Count)
		{
			return false;
		}
		Bag::SortBind(items);
		for (auto i = items.begin(); i != items.end(); ++i)
		{
			int tempcount = (*i)->itemCount;
			int subcount = (tempcount >= Count) ? Count : tempcount;
			int istake = TakeBagItem(*i, subcount);
			if (!istake)
			{
				return false;
			}
			Count -= subcount;
			if(Count <= 0)
			{
				break;
			}
		}
		return (Count <= 0);
	}
}

bool BagTakeItemTransition::TakeUniqueItem(UINT64 uid, int Count)
{
	m_isUsedNotify = false;
	XItem *pItem = m_pBag->FindUniqueItemInBag(uid);
	if(NULL == pItem)
	{
		LogWarn("The itemid:%lld not in bag", uid);
		return false;
	}
	return TakeBagItem(pItem, Count);
}

void BagTakeItemTransition::RollBack()
{
	m_isUsedNotify = true;
	foreach(j in m_ItemOldCount)
	{
		XItem *pItem = j->first;
		pItem->itemCount = j->second;
	}

	foreach(i in m_VirtualItemOldCount)
	{
		m_pBag->SetVirtualItem(i->first, i->second, true, NULL);
		//m_pBag->AddAnyItem(i->first, i->second, false);//刷金币bug
	}

	m_ItemOldCount.clear();
	m_VirtualItemOldCount.clear();
	m_ItemChangeCount.clear();
}

bool BagTakeItemTransition::TakeVirtualItem(UINT32 itemID, int Count)
{
	if (m_pBag->CountVirtualItem(itemID) < Count)
	{
		return false;
	}

	if (m_VirtualItemOldCount.count(itemID) == 0)
	{
		m_VirtualItemOldCount[itemID] = m_pBag->CountVirtualItem(itemID);
	}

	Record(itemID, Count);
	m_pBag->DecVirtualItem(itemID, Count, &m_Record);
	return true;
}

bool BagTakeItemTransition::TakeBagItem(XItem* pItem, int Count)
{
	if (Count == 0)
	{
		return true;
	}
	if (Count < 0)
	{
		LogError("take bag item error, take count:%d < 0, uid:%llu, itemid:%u", Count, pItem->uid, pItem->itemID);
		return true;
	}
	if (pItem->itemCount < Count)
	{
		return false;
	}
	// record
	if (m_ItemOldCount.count(pItem) == 0)
	{
		m_ItemOldCount[pItem] = pItem->itemCount;
	}
	else
	{
		m_ItemOldCount[pItem] += pItem->itemCount;
	}
	// remove
	if (pItem->itemCount == Count)
	{
		pItem->itemCount = 0; // to be deleted
	}
	else
	{
		m_pBag->ChangeItemCount(pItem, -Count, 0, &m_Record);
	}

	Record(pItem->itemID, Count);	
	return true;
}

void BagTakeItemTransition::Record(UINT32 dwItemID, INT32 nCount)
{
	INT32& rnValue = m_ItemChangeCount[dwItemID];
	rnValue += nCount;
}

INT32 BagTakeItemTransition::GetRecordNum(UINT32 dwItemID) const
{
	std::map<UINT32, int>::const_iterator it = m_ItemChangeCount.find(dwItemID);
	return it == m_ItemChangeCount.end() ? 0 : it->second;
}

void BagTakeItemTransition::DoTXLog(UINT32 dwItemID, INT32 nType, INT64 llCount)
{
	if(dwItemID == GOLD || dwItemID == DIAMOND || dwItemID == DRAGON_COIN)
	{
		TMoneyFlowLog oLog(m_pRole);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nAddOrReduce = TX_REDUCE;
		oLog.m_nMoneyChange = (INT32)llCount;
		oLog.m_nMoneyType = GetTLogMoneyType(dwItemID);
		oLog.SetTransTag(m_strTrans);
		oLog.Do();

		if (dwItemID == DRAGON_COIN && m_nSubReason != ItemFlow_Auction_Take)
		{
			m_pRole->Get<PlatformShareRecord>()->AddDragonCoinsConsume(llCount);
		}
	}
	else if (dwItemID == FATIGUE)
	{
		TFatigueLog oLog(m_pRole);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nAddOrReduce = TX_REDUCE;
		oLog.m_nFatigueChange = (INT32)llCount;
		oLog.m_nSceneType = m_nParam;
		oLog.SetTransTag(m_strTrans);
		oLog.Do();
	}
	else
	{
		TItemFlowLog oLog(m_pRole);
		oLog.m_nItemID = dwItemID;
		oLog.m_nType = nType;
		oLog.m_nCount = (INT32)llCount;
		oLog.m_nAfterCount = dwItemID < VIRTUAL_ITEM_MAX ?m_pBag->CountVirtualItem(dwItemID):m_pBag->CountItemInBag(dwItemID);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nAddOrReduce = TX_REDUCE;
		oLog.SetTransTag(m_strTrans);
		oLog.Do();
	} 
}

void BagTakeItemTransition::BeforeNotifyClient()
{
	foreach(j in m_ItemOldCount)
	{
		XItem *pItem = j->first;
		DoTXLog(pItem->itemID, pItem->type, GetRecordNum(pItem->itemID));
		m_pRole->OnTakeItemCountAndReason(pItem->itemID, GetRecordNum(pItem->itemID), m_nReason, m_nSubReason);

		if (pItem->itemCount == 0)
		{
			m_pBag->RemoveItemUnique(pItem->uid, &m_Record);
		}
		else if (pItem->itemCount < 0)
		{
			LogWarn("take item %d count < 0", pItem->itemID);
			m_pBag->RemoveItemUnique(pItem->uid, &m_Record);
		}
	}

	foreach(j in m_VirtualItemOldCount)
	{
		DoTXLog(j->first, 0, GetRecordNum(j->first));
		m_pRole->OnTakeItemCountAndReason(j->first, GetRecordNum(j->first), m_nReason, m_nSubReason);
	}

	m_pBag->ApplyVirtualItems(m_pRole);
}

BagGiveItemTransition::BagGiveItemTransition(Role *pRole) 
:BagTransition(pRole)
,m_spriteTransition(pRole) 
{
	m_dolog = true;
	m_nReason = -1;
	m_nSubReason = -1;
	m_nMoneyCost = 0;
	m_nMoneyType = TX_MONEY;
	m_nOldLevel = 0;
	m_bIsCheckZeroProfit = true;
	m_isRealExp = false;
	m_level = 0;
}

BagGiveItemTransition::~BagGiveItemTransition()
{
	if (!m_isUsedNotify && m_nReason != -1 && m_nSubReason != -1)
	{
		LogError("Reason:%d, SubReason:%d, use GiveItemTransition, but not use NotifyClient", m_nReason, m_nSubReason);
		assert(false);
	}
}

void BagGiveItemTransition::SetReason(INT32 nReason, INT32 nSubReason /* = -1 */, bool bIsCheckZeroProfit /* = true */)
{
	m_nReason = nReason; m_nSubReason = nSubReason; m_bIsCheckZeroProfit = bIsCheckZeroProfit;
	m_spriteTransition.SetReason(nReason, nSubReason, bIsCheckZeroProfit);
	m_Record.SetSendMailSubReason(nSubReason);
	LogDebug("givetransition reason:%d, subreason:%d", nReason, nSubReason);
}

void BagGiveItemTransition::GiveItem(const ItemDesc& _desc)
{
	m_isUsedNotify = false;

	ItemDesc desc(_desc);
	UINT32 itemID = desc.itemID; 
	UINT32 itemCount = desc.itemCount;
	bool isbind = desc.isbind;

	if ((int)desc.itemCount < 0)
	{
		LogError("add too many item, it's very dangerous, roleid:%llu, itemid:%u, count:%u", m_pRole->GetID(), desc.itemID, desc.itemCount);
		return;
	}

	if (GSConfig::Instance()->IsCrossGS() && _desc.itemID == EXP && NULL == ServerCrossDataMgr::Instance()->GetDataByRoleID(m_pRole->GetID()))
	{
		LogError("Account %s RoleId %llu try add exp %u in cross_gs,no crossdata, reason %d, subreason %d", m_pRole->GetAccount().c_str(), m_pRole->GetID(), desc.itemCount, m_nReason, m_nSubReason);
		return;
	}

	//需要检查零收益且玩家是零收益状态，无奖励;
	if (m_bIsCheckZeroProfit && m_pRole->Get<CIdipRecord>()->CheckZeroProFit())
	{
		SSWarn << "roleid:"<<m_pRole->GetID() <<" is zero profit now, can't give itemid="<<itemID <<",count="<<itemCount<<END;
		return ;
	}

	//钻石托管在midas，存储的虚拟货币钻石只用来显示和简单预判使用,添加钻石必须用特定的接口发往midas处理;
	if (itemID == DIAMOND)
	{
		LogError("can't use this function give diamond, itemID:%u, Count:%u", itemID, itemCount);
		return;
	}
	if (itemID == DICE)
	{
		LogError("can't use this function give dice, itemID:%u, Count:%u", itemID, itemCount);
		return;
	}

	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf == NULL)
	{
		SSWarn<<"roleid:"<<m_pRole->GetID()<<" itemid:"<<itemID<<" not find "<<" reason:"<<m_nReason<<" subreason:"<<m_nSubReason<<END;
		return;
	}
	isbind = ItemConfig::Instance()->GetBind(pConf, isbind);

	if (ItemConfig::Instance()->IsFashion(itemID))
	{
		for (UINT32 i = 0; i < itemCount; ++i)
		{
			m_pRole->Get<RoleFashion>()->AddNewFashion(itemID, false);
			DesignationMgr::Instance()->OnGetFashion(m_pRole, itemID);
			DoTXLog(itemID, pConf->ItemType, 1);
		}
		return;
	}

	switch (pConf->ItemType)
	{
	case SPRITE: ///> 精灵
		{
			for(UINT32 i = 0; i < itemCount; i++)
			{
				m_spriteTransition.GiveSprite(itemID);
			}
			return;
		}
	case EQUIP_PROTO: ///> 装备原型，把装备ID转换成对应职业的装备
		{
			UINT32 profItemID = EquipProtoCalc::ConvertToProfessionItemID(itemID, m_pRole->GetProfession());
			pConf = ItemConfig::Instance()->GetItemConf(profItemID);
			if (NULL == pConf)
			{
				LogError("equip item %u not found! convert from equip_proto %u", profItemID, itemID);
				return;
			}
			itemID = profItemID;	
			m_pRole->Get<RoleFashion>()->AddEquipItem(itemID);
			break;
		}
	case EQUIP:
		{
			m_pRole->Get<RoleFashion>()->AddEquipItem(pConf->ItemID);
			break;
		}
	default:
		{
			break;
		}
	}
	
	switch (itemID)
	{
	case FATIGUE:
		{
			INT64 llFatigue = m_pRole->Get<Bag>()->CountVirtualItem(FATIGUE);
			if(llFatigue >= GetGlobalConfig().MaxFatigue) 
			{
				LogInfo("roleid:%llu fatigue is max, can't add in", m_pRole->GetID());
				return;
			}
			if(llFatigue + itemCount > GetGlobalConfig().MaxFatigue) 
			{
				itemCount = GetGlobalConfig().MaxFatigue - llFatigue;
			}
			break;
		}
	default:
		{
			break;
		}
	}

	UINT32 insertBagNum = m_pBag->AddAnyItem(ItemDesc(itemID, itemCount, isbind, desc.cooldown), &m_Record);

	Record(itemID, pConf->ItemType, itemCount);

	/// 只能调用真正进背包物品的事件
	if (insertBagNum)
	{
		m_pRole->OnGetItemCountAndReason(itemID, insertBagNum, m_nReason, m_nSubReason);
	}

	///> 原来因为包裹满了发到邮箱，现在取出来，获得物品的事件不应该再次被调用
	if (m_nReason == ItemFlow_MailBagFull)
	{
		return;
	}
	if (GOLD == itemID)
	{
		DesignationMgr::Instance()->OnHaveGoldCount(m_pRole);
	}
	DesignationMgr::Instance()->OnHaveJewelryCount(m_pRole, itemID);
	DesignationMgr::Instance()->OnHaveSuitEquipCount(m_pRole, itemID);
	AchieveV2Mgr::Instance()->OnHaveSuitCount(m_pRole, itemID);
	AchieveV2Mgr::Instance()->OnJewelrySuitCount(m_pRole, itemID);

	m_pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_ItemID, itemID, itemCount);	
}

void BagGiveItemTransition::GiveItem(UINT32 itemID, UINT32 itemCount, bool isbind)
{
	GiveItem(ItemDesc(itemID, itemCount, isbind));
}

void BagGiveItemTransition::GiveItem(XItem* pItem)
{
	m_isUsedNotify = false;

	//需要检查零收益且玩家是零收益状态，无奖励;
	if (m_bIsCheckZeroProfit && m_pRole->Get<CIdipRecord>()->CheckZeroProFit())
	{
		SSWarn << "roleid:"<<m_pRole->GetID() <<" is zero profit now, can't give itemid="<<pItem->itemID <<",count="<<pItem->itemCount<<END;
		return ;
	}
	
	if (pItem->type == FASHION)
	{
		for (UINT32 i = 0; i < pItem->itemCount; ++i)
		{		
			m_pRole->Get<RoleFashion>()->AddNewFashion(pItem->itemID, false);
			DoTXLog(pItem->itemID, pItem->type, 1);
		}

		DesignationMgr::Instance()->OnGetFashion(m_pRole, pItem->itemID);

		DesignationMgr::Instance()->OnHaveJewelryCount(m_pRole, pItem->itemID);
		DesignationMgr::Instance()->OnHaveSuitEquipCount(m_pRole, pItem->itemID);
		AchieveV2Mgr::Instance()->OnHaveSuitCount( m_pRole, pItem->itemID );
		AchieveV2Mgr::Instance()->OnJewelrySuitCount( m_pRole, pItem->itemID );
		m_pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_ItemID, pItem->itemID, pItem->itemCount);
		
		return;
	}
	if (pItem->type == EQUIP)
	{
		m_pRole->Get<RoleFashion>()->AddEquipItem(pItem->itemID);		
	}
	if (pItem->type == EQUIP_PROTO)
	{
		SSWarn<<"give equip proto item, itemid;"<<pItem->itemID<<END;
		return;
	}

	//钻石托管在midas，存储的虚拟货币钻石只用来显示和简单预判使用,添加钻石必须用特定的接口发往midas处理;
	if (pItem->itemID == DIAMOND)
	{
		LogError("can't use this function give diamond, Count:%u", pItem->itemCount);
		return;
	}

	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (NULL == pConf)
	{
		SSWarn<<"item id:"<<pItem->itemID<<" not find"<<END;
		return;
	}
	pItem->isbind = ItemConfig::Instance()->GetBind(pConf, pItem->isbind);

	UINT32 insertBagNum = m_pBag->AddAnyItem(pItem, &m_Record);

	Record(pItem->itemID, pItem->type, pItem->itemCount);
	
	///> 只能调用真正进背包物品的事件
	if (insertBagNum)
	{
		m_pRole->OnGetItemCountAndReason(pItem->itemID, insertBagNum, m_nReason, m_nSubReason);
	}

	///> 原来因为包裹满了发到邮箱，现在取出来，获得物品的事件不应该再次被调用
	if (m_nReason == ItemFlow_MailBagFull)
	{
		return;
	}
	DesignationMgr::Instance()->OnHaveJewelryCount(m_pRole, pItem->itemID);
	DesignationMgr::Instance()->OnHaveSuitEquipCount(m_pRole, pItem->itemID);
	AchieveV2Mgr::Instance()->OnHaveSuitCount(m_pRole, pItem->itemID);
	AchieveV2Mgr::Instance()->OnJewelrySuitCount(m_pRole, pItem->itemID);
	m_pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_ItemID, pItem->itemID, pItem->itemCount);	
}

void BagGiveItemTransition::SetIsRealExp(bool isRealExp)
{
	m_isRealExp = isRealExp;
}

void BagGiveItemTransition::BeforeNotifyClient()
{
	m_nOldLevel = m_pRole->GetLevel();

	UINT64 nExp = m_pBag->GetVirtualItemsExp();
	double worldlevelbuff = WorldLevelMgr::Instance()->GetExpBuff(m_pRole->GetCalcAddExpLevel(), m_pRole->GetID());
	float levelsealbuff = CLevelSealMgr::Instance()->GetPlayerExtraBuff(m_pRole->GetCalcAddExpLevel(), m_pRole->GetID()) + 1.0;
	INT32 nPrePowerPoint = INT32(m_pRole->GetAttr(TOTAL_POWERPOINT));
	m_pBag->ApplyVirtualItems(m_pRole, m_isRealExp);
	if (nExp)
	{
		TPlayerExpFlowLog oLog(m_pRole);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nExpChange = m_pRole->GetRealExp();
		oLog.m_nBeforeLevel = m_nOldLevel;
		oLog.m_nAfterLevel = m_pRole->GetLevel();
		oLog.m_WorldLevelBuff = worldlevelbuff;
		oLog.m_LevelSealBuff = levelsealbuff;
		oLog.m_CurExp = (INT32)m_pRole->GetExp();
		if(m_pRole->GetLevel() != m_nOldLevel)
		{
			UINT32 dwCurr = TimeUtil::GetTime();
			UINT32 dwLast = m_pRole->GetRoleAllInfoPtr()->extrainfo().lastleveluptime();
			oLog.m_nTime  = (dwCurr > dwLast ? dwCurr - dwLast : 0);
			m_pRole->GetRoleAllInfoPtr()->mutable_extrainfo()->set_lastleveluptime(dwCurr);
		}
		oLog.m_PrePowerPoint = nPrePowerPoint;
		oLog.Do();
	}

	for(std::map<UINT32, ItemAddRecord>::iterator it = m_oAddMap.begin(); it != m_oAddMap.end(); ++it)
	{
		DoTXLog(it->first, it->second.m_nType, it->second.m_llCount);
	}

	m_spriteTransition.NotifyClient();
}

void BagGiveItemTransition::AfterNotifyClient()
{
}

void BagGiveItemTransition::DoTXLog(UINT32 dwItemID, INT32 nType, INT64 llCount)
{
	if (dwItemID == EXP)
	{
		return;
	}
	if(dwItemID == GOLD || dwItemID == DIAMOND || dwItemID == DRAGON_COIN)
	{
		TMoneyFlowLog oLog(m_pRole);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nAddOrReduce = TX_ADD;
		oLog.m_nMoneyChange = (INT32)llCount;
		oLog.m_nMoneyType = GetTLogMoneyType(dwItemID);
		oLog.SetTransTag(m_strTrans);
		oLog.Do();
	}
	else if(dwItemID == FATIGUE)
	{
		TFatigueLog oLog(m_pRole);
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nAddOrReduce = TX_ADD;
		oLog.m_nFatigueChange = (INT32)llCount;
		oLog.SetTransTag(m_strTrans);
		oLog.Do();
	}
	else
	{
		TItemFlowLog oLog(m_pRole);
		oLog.m_nItemID = dwItemID;
		oLog.m_nType = nType;
		oLog.m_nCount = (INT32)llCount;
		if (nType == FASHION)
		{
			oLog.m_nAfterCount = m_pRole->Get<RoleFashion>()->GetFashionNumByItemId(dwItemID);
		}
		else if (dwItemID < VIRTUAL_ITEM_MAX)
		{
			oLog.m_nAfterCount = m_pBag->CountVirtualItem(dwItemID);
		}
		else
		{
			oLog.m_nAfterCount = m_pBag->CountItemInBag(dwItemID);
		}
		
		oLog.m_nReason = m_nReason;
		oLog.m_nSubReason = m_nSubReason;
		oLog.m_nMoneyCost = m_nMoneyCost;
		oLog.m_nMoneyType = m_nMoneyType;
		oLog.m_nAddOrReduce = TX_ADD;
		oLog.SetTransTag(m_strTrans);
		oLog.Do();
	}
}

void BagGiveItemTransition::Record(UINT32 dwItemID, INT32 nType, INT64 llCount)
{
	if(llCount == 0) return;

	std::map<UINT32, ItemAddRecord>::iterator it = m_oAddMap.find(dwItemID);
	if(it == m_oAddMap.end())
	{
		m_oAddMap.insert(std::make_pair(dwItemID, ItemAddRecord(nType, llCount)));
	}
	else
	{
		it->second.m_llCount += llCount;
	}
}

BagWearItemTransition::BagWearItemTransition(Role *pRole) : BagTransition(pRole)
{
}

BagWearItemTransition::~BagWearItemTransition()
{
}

void BagWearItemTransition::SetSubReason(int subReason)
{
	m_Record.SetSendMailSubReason(subReason);
}

KKSG::ErrorCode BagWearItemTransition::WearItem(XItem* pItem, bool isSwap)
{
	return m_pBag->WearEquip(pItem, isSwap, &m_Record);
}

KKSG::ErrorCode BagWearItemTransition::WearEmblem(XItem* pItem)
{
	return m_pBag->WearEmblem(pItem, &m_Record);
}

KKSG::ErrorCode BagWearItemTransition::WearArtifact(XItem* pItem)
{
	return m_pBag->WearArtifact(pItem, &m_Record);
}

KKSG::ErrorCode BagWearItemTransition::TakeoffItem(XItem* pItem, bool isFullSendMail)
{
	if (isFullSendMail)
	{
		assert(m_Record.GetSendMailSubReason() != -1);
	}
	return m_pBag->TakeoffItem(pItem, isFullSendMail, &m_Record);
}

void BagWearItemTransition::BeforeNotifyClient()
{
	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_WearItem);
}

BagUseItemTransition::BagUseItemTransition(Role *pRole) : BagTransition(pRole)
{
}

BagUseItemTransition::~BagUseItemTransition()
{
}

KKSG::ErrorCode BagUseItemTransition::UseItem(UINT64 uid, std::vector<UINT64>& uids, int Count, UINT32 dwOpType, KKSG::UseItemRes &roRes, UINT64 petId)
{
	XItem *pItem =  NULL;
	switch (dwOpType)
	{
	case KKSG::BagFind:
		{
			///> 默认在包裹里找
			pItem = m_pBag->FindUniqueItemInBag(uid);
			break;
		}
	case KKSG::BodyFind:
		{
			///> 卸装备的时候在装备栏里找
			pItem = m_pBag->FindUniqueItemOnBody(uid);
			break;
		}
	case KKSG::FashionWear:
		{
			RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
			return pFashion->Wear(uid);
		}
	case KKSG::FashionOff:
		{
			RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
			return pFashion->Takeoff(uid);
		}
	case KKSG::Composite:
		{
			BagCompositeTransition transition(m_pRole);
			return transition.Composite((UINT32)uid, uids, roRes);
		}
	case KKSG::ItemBuffAdd:
		{
			pItem = m_pBag->FindUniqueItemInBag(uid);
			return UseItemBuff(pItem);
		}
	case KKSG::FashionDisplayWear :
		{
			RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
			return pFashion->DisplayFashionWear(uid);
		}
	case KKSG::FashionDisplayOff :
		{
			RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
			return pFashion->DisplayFashionTakeOff(uid);
		}
	default:
		{
			break;
		}
	}

	if (pItem == NULL)
	{
		LogWarn("role [%llu] can't find item, uid [%llu]", m_pRole->GetID(), uid);
		return KKSG::ERR_ITEM_NOTEXIST;
	}

	if (!((pItem->type == EQUIP || pItem->type == EMBLEM || pItem->type == ARTIFACT) && dwOpType == 1))
	{
		///> 非脱下物品的。才需要检查职业
		if (!XItem::CheckProfession(pItem->itemID, m_pRole->GetProfession()))
		{
			return KKSG::ERR_ITEM_WRONG_PROFESSION;
		}
	}
	//LogInfo("use item id:%d optype:%d count:%d", pItem->itemID, dwOpType, Count);

	switch (pItem->type)
	{
	case EQUIP: 
		return UseEquip(pItem, Count > 0, dwOpType);
	case PECK: 
		return UsePeck(pItem, Count, roRes);
	case EMBLEM: 
		return UseEmblem(pItem, dwOpType);
	case CIRCLE_DWAW:
		return UseCircleDraw(pItem);
	case PET_BORN:
		return UsePetBorn(pItem);
	case SCENE_COUNT_TICKET:
		return UseSceneCountTicket(pItem);
	case PET_SKILL_BOOK:
		return UsePetSkillBook(pItem, petId);
	case DESIGNATION_ITEM:
		return UseDesignation(pItem);
	case HERO_EXPERIENCE_TICKET:
		return UseHeroTicket(pItem->itemID);
	case ARTIFACT:
		return UseArtifact(pItem, dwOpType);
	case BagExpand:
		return UseBagExpand(pItem->itemID, roRes);
	case Tarja:
		return UseTarja(pItem);
	case GUILDBONUS:
		return UseGuildBonus(pItem, Count);
	}
	return KKSG::ERR_UNKNOWN;
}

KKSG::ErrorCode BagUseItemTransition::UseItem(std::vector<UINT64>& uids, UINT32 dwOpType)
{
	KKSG::ErrorCode error = KKSG::ERR_SUCCESS;
	if (dwOpType == KKSG::FashionSuitWear)
	{
		RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
		error = pFashion->Wear(uids);
	}
	else if (dwOpType == KKSG::FashionSuitOff)
	{
		RoleFashion *pFashion = m_pRole->Get<RoleFashion>();
		error = pFashion->Takeoff(uids);
	}
	
	return error;
}

KKSG::ErrorCode BagUseItemTransition::DisplayFashion(const UINT32 item_id,const UINT32 operator_type)
{
	RoleFashion *fashion = m_pRole->Get<RoleFashion>();
	if (operator_type == KKSG::FashionDisplayWear)
	{
		return fashion->DisplayFashionWear(item_id);
	}
	if (operator_type == KKSG::FashionDisplayOff)
	{
		return fashion->DisplayFashionTakeOff(item_id);
	}

	return  KKSG::ERR_UNKNOWN;
}
KKSG::ErrorCode BagUseItemTransition::DisplaySuitFashion(const UINT32 suit_id,const UINT32 operator_type)
{
	RoleFashion *fashion = m_pRole->Get<RoleFashion>();
	if (operator_type == KKSG::FashionSuitDisplayWear)
	{
		return fashion->DisplaySuitFashionWear(suit_id);
	}
	if (operator_type == KKSG::FashionSuitDisplayOff)
	{
		return fashion->DisplaySuitFashionTakeOff(suit_id);
	}

	return  KKSG::ERR_UNKNOWN;
}

KKSG::ErrorCode BagUseItemTransition::ActivationFashion(UINT64 uid)
{
	RoleFashion *fashion = m_pRole->Get<RoleFashion>();
	return fashion->ActivationFashion(uid);
}

KKSG::ErrorCode BagUseItemTransition::UseItemBuff(XItem *pItem)
{
	if (pItem == NULL)
	{
		LogWarn("role [%llu], UseItemBuff, item is NULL", m_pRole->GetID());
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	bool istransbuff = false;
	// normal buff
	ItemBuffConf* conf = ItemConfig::Instance()->GetBuffConf(pItem->itemID);
	// transform buff
	ItemTransformConf* transconf = ItemConfig::Instance()->GetItemTransformConf(pItem->itemID);
	if (NULL == conf && NULL == transconf)
	{
		LogInfo("role [%llu], item [%u] not in ItemBuff.txt and ItemTransBuff.txt", m_pRole->GetID(), pItem->itemID);
		return KKSG::ERR_FAILED;
	}
	if (NULL != conf)
	{
		BagTakeItemTransition taketransition(m_pRole);
		taketransition.SetReason(ItemFlow_UseItem, ItemFlow_UsePeck);
		if (!taketransition.TakeItem(pItem->itemID, 1))
		{
			taketransition.RollBack();
			return KKSG::ERR_UNKNOWN;
		}
		taketransition.NotifyClient();
		// add buff
		for (auto i = conf->Buffs.begin(); i != conf->Buffs.end(); ++i)
		{
			m_pRole->Get<BuffRecord>()->AddBuff(conf->Type, i->seq[0], i->seq[1]);
		}
	}
	else if (NULL != transconf)
	{
		///> 假如是变身buff，需要判断条件
		BuffDesc buff = ItemConfig::Instance()->GetTransBuff(transconf);
		if (0 == buff.buffID)
		{
			SSError<<"roleid:"<<m_pRole->GetID()<<" itemid:"<<pItem->itemID<<" random trans buff error"<<END;
			return KKSG::ERR_FAILED;
		}
		int mask = XBuffTemplateManager::Instance()->IsScaleBuff(buff.buffID) ? 2 : 1;
		UINT32 ret = m_pRole->Get<BuffRecord>()->CanTrans(mask);
		if (KKSG::ERR_SUCCESS != ret)
		{
			return (KKSG::ErrorCode)ret;
		}

		BagTakeItemTransition taketransition(m_pRole);
		taketransition.SetReason(ItemFlow_UseItem, ItemFlow_UsePeck);
		if (!taketransition.TakeItem(pItem->itemID, 1))
		{
			taketransition.RollBack();
			return KKSG::ERR_UNKNOWN;
		}
		taketransition.NotifyClient();

		m_pRole->Get<BuffRecord>()->AddTransBuff(buff.buffID, buff.BuffLevel, m_pRole->GetID());
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode BagUseItemTransition::UseEquip(XItem *pItem, bool isSwap, UINT32 dwOpType)
{	
	KKSG::ErrorCode dwErrorCode = KKSG::ERR_UNKNOWN;

	BagWearItemTransition transition(m_pRole);
	if (0 == dwOpType)
	{
		dwErrorCode = transition.WearItem(pItem, isSwap);
		transition.NotifyClient();
	}
	else if (1 == dwOpType)
	{
		dwErrorCode = transition.TakeoffItem(pItem);
		transition.NotifyClient();
	}
	else
	{
		LogWarn("OpType:%u is not 0 or 1", dwOpType);
	}
	return dwErrorCode;
}

KKSG::ErrorCode BagUseItemTransition::UseEmblem(XItem *pItem, UINT32 dwOpType)
{
	KKSG::ErrorCode dwErrorCode = KKSG::ERR_UNKNOWN;

	BagWearItemTransition transition(m_pRole);
	if (0 == dwOpType)
	{
		dwErrorCode = transition.WearEmblem(pItem);
		transition.NotifyClient();
	}
	else if (1 == dwOpType)
	{
		dwErrorCode = transition.TakeoffItem(pItem);
		transition.NotifyClient();
	}
	else
	{
		LogWarn("OpType:%u is not 0 or 1", dwOpType);
	}
	return dwErrorCode;
}

KKSG::ErrorCode BagUseItemTransition::UseArtifact(XItem* pItem, UINT32 dwOpType)
{
	KKSG::ErrorCode dwErrorCode = KKSG::ERR_UNKNOWN;

	BagWearItemTransition transition(m_pRole);
	if (0 == dwOpType)
	{
		dwErrorCode = transition.WearArtifact(pItem);
		transition.NotifyClient();
	}
	else if (1 == dwOpType)
	{
		dwErrorCode = transition.TakeoffItem(pItem);
		transition.NotifyClient();
	}
	else
	{
		LogWarn("OpType:%u is not 0 or 1", dwOpType);
	}
	return dwErrorCode;
}

KKSG::ErrorCode BagUseItemTransition::UsePeck(XItem *pItem, int Count, KKSG::UseItemRes &roRes)
{
	ChestConf *pConf = ItemConfig::Instance()->GetChestConf(pItem->itemID, m_pRole->GetProfession());
	if (pConf == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	bool isbind = pItem->IsBind();
	int cnt = Count == 1 ? 1 : pItem->itemCount;
	if(!TakePeckItem(pItem->itemID, cnt))
	{
		return KKSG::ERR_FAILED;
	}
	std::vector<ItemDesc> giveItem;
	for(int i = 0; i < cnt; i++)
	{
		ItemConfig::Instance()->GetChestItems(pConf, giveItem, m_pRole->GetLevel());
	}
	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_UseItem, ItemFlow_UsePeck);

	INT32 iDiamond = 0;
	for(auto i = giveItem.begin(); i != giveItem.end(); i++)
	{
		if ((*i).itemID == DIAMOND)
		{
			iDiamond += (*i).itemCount;
			continue;
		}
		if ((*i).itemID == DICE)
		{
			m_pRole->Get<CSuperRiskRecord>()->AddDiceNum((*i).itemCount, true);
			continue;
		}
		transition.GiveItem(*i);

		KKSG::ItemBrief *pBrief = roRes.add_resultitems();
		pBrief->set_itemid((*i).itemID);
		pBrief->set_itemcount((*i).itemCount);
	}
	transition.NotifyClient();
	if (iDiamond)
	{
		m_pRole->AddDiamond(iDiamond, ItemFlow_UseItem, ItemFlow_UsePeck);
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode BagUseItemTransition::UseCircleDraw(XItem *pItem)
{
	const std::vector<Sequence<uint,3>> &info = pItem->circleDrawInfo;
	if (info.empty())
	{
		return KKSG::ERR_FAILED;
	}	

	UINT32 allPro = 0;
	for (size_t i = 0; i < info.size(); ++i)
	{
		allPro += info[i][2];
	}

	UINT32 random = XRandom::randInt(1, allPro + 1), prob = 0;
	for (size_t i = 0; i < info.size(); ++i)
	{
		prob += info[i][2];
		if (prob >= random)
		{
			BagGiveItemTransition give(m_pRole);
			give.SetReason(ItemFlow_UseItem, ItemFlow_CircleDraw);
			give.GiveItem(info[i][0],info[i][1]);
			give.NotifyClient();

			PtcG2C_ItemCircleDrawResult send;
			send.m_Data.set_itemid(info[i][0]);
			send.m_Data.set_index(i);
			m_pRole->Send(send);

			UINT32 noticeID = ItemConfig::Instance()->GetCircleDrawNotice(pItem->itemID, info[i][0], info[i][1], info[i][2], i + 1);
			if (noticeID && GuildSimpleMgr::Instance()->GetGuildByRoleId(m_pRole->GetID()) != NULL)
			{
				Notice notice(noticeID);
				notice.Add(m_pRole);
				notice.Replace("$C", ItemConfig::Instance()->GetItemName(pItem->itemID, m_pRole->GetID()));
				notice.Add(ItemDesc(info[i][0], info[i][1]));
				notice.SendGuild(m_pRole->getGuildId());
			}
			break;
		}
	}

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_UseItem, ItemFlow_CircleDraw);
	if (!take.TakeUniqueItem(pItem->uid,1))
	{
		take.RollBack();
		return KKSG::ERR_FAILED;
	}
	take.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

bool BagUseItemTransition::TakePeckItem(UINT32 itemID, int Count)
{
	BagTakeItemTransition taketransition(m_pRole);
	taketransition.SetReason(ItemFlow_UseItem, ItemFlow_UsePeck);
	if (!taketransition.TakeItem(itemID, Count))
	{
		taketransition.RollBack();
		return false;
	}
	taketransition.NotifyClient();
	return true;
}

KKSG::ErrorCode BagUseItemTransition::UseTarja(XItem *pItem)
{
	// 检查活动是否开启 
	if (GetGlobalConfig().TarjaSwitch==0)
	{
		return KKSG::ERR_UNOPEN_FUNCTION;
	}

	if (pItem == NULL)
	{
		LogWarn("role [%llu], UseTarja, item is NULL", m_pRole->GetID());
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	UINT32 nItemID = GetGlobalConfig().TarjaItem[0];
	UINT32 nItemCount = GetGlobalConfig().TarjaItem[1];
	if(nItemCount < 1)
	{
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}

	BagTakeItemTransition taketransition(m_pRole);
	taketransition.SetReason(ItemFlow_UseItem, ItemFlow_Traja);
	if (taketransition.TakeItem(nItemID, nItemCount))
	{
		StageMgr *pStage =  m_pRole->Get<StageMgr>();
		pStage->AddTarjaTime();
		taketransition.NotifyClient();
		return KKSG::ERR_SUCCESS;

	}
	taketransition.RollBack();
	return KKSG::ERR_FAILED;
}



KKSG::ErrorCode BagUseItemTransition::UseGuildBonus(XItem *pItem, UINT32 count)
{
	if (pItem == NULL)
	{
		LogWarn("role [%llu], UseGuildBonus, item is NULL", m_pRole->GetID());
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (!m_pRole->getGuildId())
	{
		return KKSG::ERR_NOTGUILD;
	}

	BagTakeItemTransition taketransition(m_pRole);
	taketransition.SetReason(ItemFlow_UseItem, ItemFlow_GuildBonus);
	if (!taketransition.TakeItem(pItem->itemID, count))
	{
		taketransition.RollBack();
		return KKSG::ERR_FAILED;
	}
	taketransition.NotifyClient();

	for (int i = 0; i < count; ++i)
	{
		EventMgr::Instance()->AddGuildBonusEvent(m_pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_USEITEM, pItem->itemID);
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode BagUseItemTransition::UseDesignation(XItem *pItem)
{
	KKSG::ErrorCode dwErrorCode = KKSG::ERR_UNKNOWN;
	DesignationItem::RowData* pRowData = ItemConfig::Instance()->GetDesignation(pItem->itemID);
	if (pRowData==NULL)
	{
		return dwErrorCode;
	}
	UINT32 hasNum = m_pBag->CountItemInBag(pRowData->itemid);
	if(hasNum < pRowData->itemcount)
	{
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	DesignationRecord* record = m_pRole->Get<DesignationRecord>();
	if (record->HasDes(pRowData->desid))
		return KKSG::ERR_ARENA_ERROR;

	BagTakeItemTransition trans(m_pRole);
	trans.SetReason(ItemFlow_UseItem, ItemFlow_Desig);
	if (trans.TakeItem(pRowData->itemid , pRowData->itemcount))
	{
		trans.NotifyClient();
		DesignationMgr::Instance()->OnUseItem(m_pRole,pRowData->desid);
		return KKSG::ERR_SUCCESS;
	}
	trans.RollBack();
	return KKSG::ERR_FAILED;
}

KKSG::ErrorCode BagUseItemTransition::UseHeroTicket(UINT32 itemID)
{
	return m_pRole->Get<CHeroBattleRecord>()->UseExperienceHeroTicket(itemID);
}

KKSG::ErrorCode BagUseItemTransition::UsePetSkillBook(XItem *pItem, UINT64 petId)
{
	PetSys* pPetSys = m_pRole->Get<PetSys>();
	if (pPetSys == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	Pet* pPet = pPetSys->GetPet(petId);
	if (pPet == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	PetSkillBook::RowData* pRowData = PetConfigMgr::Instance()->GetPetBookData(pItem->itemID);
	if (pRowData == NULL || !pPet->CanAddSkill(pRowData->skillid))
	{
		return KKSG::ERR_CAN_NOT_USE_PET_SKILL_BOOK;
	}

	UINT32 bookId = pItem->itemID; 
	BagTakeItemTransition trans(m_pRole);
	trans.SetReason(ItemFlow_UseItem, ItemFlow_PetSkillBook);
	if (trans.TakeUniqueItem(pItem->uid, 1))
	{
		trans.NotifyClient();
		pPet->AddSkill(m_pRole, bookId);
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		trans.RollBack();
		return KKSG::ERR_UNKNOWN;
	}
}

KKSG::ErrorCode BagUseItemTransition::UsePetBorn(XItem *pItem)
{
	PetItemConf* conf = ItemConfig::Instance()->GetPetItemConf(pItem->itemID);
	if (conf == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	PetSys* pPetSys = m_pRole->Get<PetSys>();

	BagTakeItemTransition trans(m_pRole);
	trans.SetReason(ItemFlow_UseItem, ItemFlow_UsePetBorn);
	if (!trans.TakeUniqueItem(pItem->uid, 1))
	{
		trans.RollBack();
		return KKSG::ERR_SLOTATTR_MONEYLIMIT;
	}
	KKSG::ErrorCode errorCode = pPetSys->AddPet(conf->petid); 
	if (errorCode != KKSG::ERR_SUCCESS)
	{
		trans.RollBack();
		return errorCode;
	}
	trans.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode BagUseItemTransition::UseSceneCountTicket( XItem *pItem )
{
	BagTakeItemTransition trans(m_pRole);
	trans.SetReason(ItemFlow_UseItem, ItemFlow_UseSceneCountTicket);
	trans.TakeUniqueItem(pItem->uid, 1);
	trans.NotifyClient();
	//目前只添加巢穴 如果有其他类型添加 再根据ItemID判断
	m_pRole->Get<CTeamRecord>()->AddTeamExtraAddCount(NEST_TEAM_SCENE);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode BagUseItemTransition::UseBagExpand(UINT32 itemID, KKSG::UseItemRes& roRes)
{
	const BagExpandConf* conf = ItemConfig::Instance()->GetBagExpandConf(itemID);
	if (conf == NULL)
	{
		LogWarn("itemID [%u] 's type is BagExpand, but not in BagExpandItemList.txt", itemID);
		return KKSG::ERR_FAILED;
	}
	std::pair<UINT32, UINT32> data = m_pBag->GetBagExpandData((KKSG::BagType)conf->Type);
	if (data.second >= conf->NeedAndOpen.size())
	{
		return KKSG::ERR_BAGEXPAND_MAXCOUNT;
	}
	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_UseItem, ItemFlow_UseItem_BagExpand);
	if (!take.TakeItem(itemID, conf->NeedAndOpen[data.second][0]))
	{
		take.RollBack();
		return KKSG::ERR_BAGEXPAND_LESSITEM;
	}
	take.NotifyClient();

	KKSG::ErrorCode code = m_pBag->AddBagExpand((KKSG::BagType)conf->Type, conf->NeedAndOpen[data.second][1]);
	if (code != KKSG::ERR_SUCCESS)
	{
		return code;
	}
	KKSG::BagExpandData* expandData = roRes.mutable_expand();
	expandData->set_type((KKSG::BagType)conf->Type);
	data = m_pBag->GetBagExpandData((KKSG::BagType)conf->Type);
	expandData->set_num(data.first);
	expandData->set_count(data.second);

	return KKSG::ERR_SUCCESS;
}

BagChangeItemAttrTransition::BagChangeItemAttrTransition(Role* pRole) :BagTransition(pRole)
{
}

BagChangeItemAttrTransition::~BagChangeItemAttrTransition()
{
}

void BagChangeItemAttrTransition::ChangeItemAttr(XItem *pItem, const ItemChangeAttr& stChangeAttr)
{
	if (NULL == pItem || NULL == m_pBag)
	{
		return;
	}
	m_pBag->ChangeItemAttr(pItem, stChangeAttr, &m_Record);
}

void BagChangeItemAttrTransition::ChangeItemEnhanceAttr(XItem *pItem, std::vector<ItemChangeAttr>& attrs)
{
	if (NULL == pItem || NULL == m_pBag)
	{
		return;
	}
	m_pBag->ChangeItemEnhanceAttr(pItem, attrs, &m_Record);
}

void BagChangeItemAttrTransition::ChangeItemJade(XItem* pItem, const JadeSingle& stJadeSingle, bool bIsDetach)
{
	if (NULL == pItem || NULL == m_pBag)
	{
		return;
	}
	m_pBag->ChangeItemJade(pItem, stJadeSingle, bIsDetach, &m_Record);
}

BagUpdateItemTransition::BagUpdateItemTransition(Role* pRole):BagTransition(pRole)
{

}

BagUpdateItemTransition::~BagUpdateItemTransition()
{

}

void BagUpdateItemTransition::AddChangedItem(XItem *pItem)
{
	m_pBag->MarkItemChanged(pItem, &m_Record);
}

BagCompositeTransition::BagCompositeTransition(Role* pRole):BagTransition(pRole)
{

}

BagCompositeTransition::~BagCompositeTransition()
{

}

bool BagCompositeTransition::Take(BagTakeItemTransition& trans, UINT32 itemid, int count)
{
	if (itemid != 0)
	{
		if (!trans.TakeItem(itemid, count))
		{
			trans.RollBack();
			return false;
		}
	}
	return true;
}

KKSG::ErrorCode BagCompositeTransition::Composite(UINT32 compID, std::vector<UINT64>& uids, KKSG::UseItemRes& roRes)
{
	ItemComposeTableConf* Row = ItemConfig::Instance()->GetComposeConf(compID);
	if (Row == NULL)
	{
		SSWarn<<"composeid:"<<compID<<"can not find"<<END;
		return KKSG::ERR_FAILED;
	}
	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(Row->ItemID);
	if (pConf == NULL)
	{
		SSWarn<<"item id:"<<Row->ItemID<<" not find in composeid:"<<compID<<END;
		return KKSG::ERR_FAILED;
	}
	if (Row->Type != 1 && Row->Type != 3)
	{
		return KKSG::ERR_FAILED;
	}
	if (Row->Type == 1)
	{
		if (!m_pBag->CanAddThisTypeItem(EQUIP))
		{
			return KKSG::ERR_BAG_FULL_CANT_COMPOSE;
		}
	}
	if (Row->Type == 3)
	{
		if (!m_pBag->CanAddThisTypeItem(EMBLEM))
		{
			return KKSG::ERR_BAG_FULL_CANT_COMPOSE;
		}
	}
	// take item
	bool isBind = false; //纹章制作-->绑定非绑定看金属板
	{
		BagTakeItemTransition transition(m_pRole);
		if(1 == Row->Type)
		{
			transition.SetReason(ItemFlow_Compose, ItemFlow_Equip_Compose);
			if (!Take(transition, Row->SrcItem1[0], Row->SrcItem1[1]))
			{
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
		}
		else if(3 == Row->Type)
		{
			transition.SetReason(ItemFlow_Compose, ItemFlow_Emblem_Compose);
			if(uids.size() < 1) 
			{
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
			Bag* pBag = m_pRole->Get<Bag>();
			XItem* item = pBag->FindUniqueItemInBag(uids[0]);
			if(NULL == item || (NULL != item && item->itemID != Row->SrcItem1[0]))
			{
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
			isBind = item->IsBind();
			if(!transition.TakeUniqueItem(uids[0], Row->SrcItem1[1]))
			{
				transition.RollBack();
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
		}
		if (!Take(transition, Row->SrcItem2[0], Row->SrcItem2[1]))
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		if (!Take(transition, Row->SrcItem3[0], Row->SrcItem3[1]))
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		if (!Take(transition, Row->SrcItem4[0], Row->SrcItem4[1]))
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		if (!Take(transition, GOLD, Row->Coin))
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		transition.SetAction(COST_TAKECOMPOSECOST);
		transition.NotifyClient();
	}
	// give	
	{
		if(1 == Row->Type) //合成装备
		{
			UINT64 uid = UIDHelper::CreateUID(UID_Item);
			XItem pItem(uid, Row->ItemID, 1, EQUIP);
			pItem.isbind = Row->IsBind;

			BagGiveItemTransition give(m_pRole);
			give.SetReason(ItemFlow_Compose, ItemFlow_Equip_Compose);
			give.GiveItem(&pItem);
			give.NotifyClient();
			roRes.set_uid(uid);
		}
		else if(3 == Row->Type) //合成纹章
		{
			UINT64 uid = UIDHelper::CreateUID(UID_Item);
			XItem pItem(uid, Row->ItemID, 1, EMBLEM);
			pItem.isbind = isBind;

			BagGiveItemTransition give(m_pRole);
			give.SetReason(ItemFlow_Compose, ItemFlow_Emblem_Compose);
			give.GiveItem(&pItem);
			give.NotifyClient();
			roRes.set_uid(uid);
		}
	}

	m_pRole->OnCompose();

	return KKSG::ERR_SUCCESS;
}

