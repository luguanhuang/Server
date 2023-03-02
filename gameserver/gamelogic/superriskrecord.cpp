#include "pch.h"
#include "superriskrecord.h"
#include <algorithm>
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "unit/role.h"
#include "util/XCommon.h"
#include "gamelogic/itemconfig.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "superrisk/ptcg2c_playdicentf.h"
#include "superrisk/ptcg2c_riskrecoverdicentf.h"
#include "define/systemiddef.h"
#include "config/itemdropconfig.h"
#include "rolesummarymgr.h"
#include "findback/findbackrecord.h"
#include "define/pushtypedef.h"
#include "utility/tlogger.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/rolepushinforecord.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"
#include "reportdatarecord.h"

int AllRiskBoxInfo::AddBox(int itemID, int itemNum)
{
	if (boxInfos.size() >= MAX_RISK_BOX_NUM)
	{
		return -1;
	}
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemID);
	if (!conf)
	{
		return -1;
	}
	int quality = conf->ItemQuality;
	int needTime = 0;
	for (int i = 0; i <  GetGlobalConfig().RiskBoxTime.size(); ++i)
	{
		if ( GetGlobalConfig().RiskBoxTime[i].seq[0] == quality)
		{
			needTime =  GetGlobalConfig().RiskBoxTime[i].seq[1];
		}
	}

	KKSG::RiskBoxInfo info;
	KKSG::ItemBrief* brief = info.mutable_item();
	brief->set_itemid(itemID);
	brief->set_itemcount(itemNum);
	info.set_lefttime(needTime);
	info.set_state(KKSG::RISK_BOX_LOCKED);

	int slot = -1;
	for (int i = 0; i < MAX_RISK_BOX_NUM; ++i)
	{
		if (boxInfos.find(i) == boxInfos.end())
		{
			slot = i;
			break;
		}
	}
	if (slot >= 0)
	{
		info.set_slot(slot);
		boxInfos[slot] = info;
	}
	return slot;
}

UINT32 AllRiskBoxInfo::ChangeBoxState(Role* role, int slot, KKSG::RiskBoxState state, KKSG::ChangeRiskBoxStateRes& roRes)
{
	if(!role)
		return KKSG::ERR_FAILED;
	auto find = boxInfos.find(slot);
	if (find == boxInfos.end())
	{
		return KKSG::ERR_FAILED;
	}
	if (state == KKSG::RISK_BOX_LOCKED)
	{
		return KKSG::ERR_FAILED;
	}
	if (state == KKSG::RISK_BOX_UNLOCKED)
	{
		if(find->second.state() != KKSG::RISK_BOX_LOCKED)
			return KKSG::ERR_FAILED;
		else if (HasBoxOpened())
		{
			return KKSG::ERR_FAILED;
		}
		else
		{
			find->second.set_state(state);

			find->second.set_begintime(time(0));
		}
	}
	if(state == KKSG::RISK_BOX_CANGETREWARD)
	{
		if(find->second.state() != KKSG::RISK_BOX_UNLOCKED || find->second.lefttime() <= 0)
			return KKSG::ERR_FAILED;
		
		if (Accelerate(role, slot))
		{
			find->second.set_state(state);
		}
		else
			return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	if (state == KKSG::RISK_BOX_GETREWARD)
	{
		if(find->second.state() != KKSG::RISK_BOX_CANGETREWARD)
			return KKSG::ERR_FAILED;
		BagGiveItemTransition transition(role);
		transition.SetReason(ItemFlow_SuperRisk, ItemFlow_SuperRiskOpenBox);

		ChestConf *pConf = ItemConfig::Instance()->GetChestConf(find->second.item().itemid(), role->GetProfession());
		if (!pConf)
		{
			return KKSG::ERR_FAILED;
		}

		std::vector<ItemDesc> items;
		ItemConfig::Instance()->GetChestItems(pConf, items);
		for (auto iter = items.begin(); iter != items.end(); ++iter)
		{
			transition.GiveItem(*iter);

			KKSG::ItemBrief* brief = roRes.add_openboxrewards();
			brief->set_itemid(iter->itemID);
			brief->set_itemcount(iter->itemCount);
		}
	
		transition.NotifyClient();

		boxInfos.erase(find);
	}
	if (state == KKSG::RISK_BOX_DELETE)
	{
		boxInfos.erase(find);
	}

	return KKSG::ERR_SUCCESS;
}

int AllRiskBoxInfo::GetLeftOpenTime(int slot)
{
	int now = time(0);
	auto find = boxInfos.find(slot);
	if (find == boxInfos.end())
	{
		return -1;
	}
	if (!find->second.lefttime())
	{
		return 0;
	}
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(find->second.item().itemid());
	if (!conf)
	{
		return -1;
	}
	int needTime = 0;
	for(int i = 0; i <  GetGlobalConfig().RiskBoxTime.size(); ++i)
	{
		if (conf->ItemQuality ==  GetGlobalConfig().RiskBoxTime[i].seq[0])
		{
			needTime =  GetGlobalConfig().RiskBoxTime[i].seq[1];
			break;
		}
	}

	if (!find->second.begintime())
	{
		return needTime;
	}
	if ((now - find->second.begintime()) >= needTime)
	{
		return 0;
	}

	return needTime + find->second.begintime() - now;

}

bool AllRiskBoxInfo::Accelerate(Role* role, int slot)
{
	int leftTime = GetLeftOpenTime(slot);
	if (leftTime <= 0)
	{
		return false;
	}
	auto find = boxInfos.find(slot);
	if (find == boxInfos.end())
	{
		return false;
	}
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(find->second.item().itemid());
	if (!conf)
	{
		return false;
	}

	int needItem = 0;
	int needCount = 0;
	for (auto iter =  GetGlobalConfig().RiskBoxAccelerate.begin(); iter !=  GetGlobalConfig().RiskBoxAccelerate.end(); ++iter)
	{
		if (iter->seq[0] == conf->ItemQuality)
		{
			needItem = iter->seq[2];
			needCount = (leftTime / iter->seq[1] + (leftTime % iter->seq[1] ? 1 : 0)) * iter->seq[3];
		}
	}

	if (needCount && needItem)
	{
		BagTakeItemTransition trasition(role);
		trasition.SetReason(ItemFlow_SuperRisk, ItemFlow_SuperRiskAccelerate);
		if(!trasition.TakeItem(needItem, needCount))
		{
			trasition.RollBack();
			return false;
		}	
		trasition.NotifyClient();
	}
	find->second.set_lefttime(0);

	return true;
}

bool AllRiskBoxInfo::CanGetBoxReward(int& slot)
{
	for (auto iter = boxInfos.begin(); iter != boxInfos.end(); ++iter)
	{
		if (iter->second.lefttime() == 0)
		{
			slot = iter->first;
			return true;
		}
	}

	return false;
}

bool AllRiskBoxInfo::HasBoxOpened()
{
	bool has = false;
	for (auto iter = boxInfos.begin(); iter != boxInfos.end(); ++iter)
	{
		if (iter->second.state() == KKSG::RISK_BOX_UNLOCKED)
		{
			has = true;
			break;
		}
	}

	return has;

}

bool AllRiskBoxInfo::OnTimer()
{
	int now = time(0);
	bool change = false;
	for (auto iter = boxInfos.begin(); iter != boxInfos.end(); ++iter)
	{
		if (iter->second.state() == KKSG::RISK_BOX_CANGETREWARD)
		{
			continue;
		}
		if (iter->second.state() == KKSG::RISK_BOX_UNLOCKED)
		{
			if (GetLeftOpenTime(iter->first) <= 0)
			{
				iter->second.set_lefttime(0);
				iter->second.set_state(KKSG::RISK_BOX_CANGETREWARD);
				change = true;
			}
		}

	}

	return change;
}

const KKSG::RiskBoxInfo* AllRiskBoxInfo::GetSlotInfo(int slot)const
{
	auto find = boxInfos.find(slot);
	if (find != boxInfos.end())
		return &find->second;

	return NULL;
}

CSuperRiskRecord::CSuperRiskRecord(Role* role):m_superRisk(role)
{
	m_pRole = role;
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 2 * 1000, -1, __FILE__, __LINE__);
	Reset();

	//LogInfo("superriskrecord, create, roleid:%llu, time:%llu, timer:%llu", m_pRole->GetID(), time(0), m_timer);
}

CSuperRiskRecord::~CSuperRiskRecord()
{
	m_pRole = NULL;
}

bool CSuperRiskRecord::AutoDestory()
{
	//LogInfo("superriskrecord, delete, roleid:%llu, time:%llu, timer:%llu", m_pRole->GetID(), time(0), m_timer);
	CTimerMgr::Instance()->KillTimer(m_timer);
	return true;
}

void CSuperRiskRecord::Reset()
{
	m_getDiceNum = 0;
	m_getDiceTime = 0;
	m_diceNum = 0;
	m_updateTime = 0;
	m_hasSendPlayNtf = false;
	m_roleMapInfo.clear();
}

void CSuperRiskRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		int now = time(0);
		m_updateTime = now;
		m_diceNum = GetGlobalConfig().RiskOpenAddDiceNum;
		m_getDiceTime = now;
		OnLevelUp();

		Change();
	}
}

bool CSuperRiskRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_riskrecord())
	{
		const KKSG::RiskInfo2DB* record = &poRoleAllInfo->riskrecord();
		m_updateTime = record->updatetime();
		m_getDiceTime = record->getdicetime();
		m_getDiceNum = record->getdicenum();
		if (record->has_infos())
		{
			m_diceNum = record->infos().dicenum();
			for (int j = 0; j < record->infos().boxinfos_size(); ++j)
			{
				KKSG::RiskBoxInfo boxInfo;
				boxInfo.CopyFrom(record->infos().boxinfos(j));
				m_boxInfos.boxInfos[boxInfo.slot()] = boxInfo;
			}
			for (int i = 0; i < record->infos().infos_size(); ++i)
			{
				const KKSG::RiskOneMapInfo& refInfo = record->infos().infos(i);
				RiskMapRoleInfo info;
				info.mapID = refInfo.mapid();
				info.moveDirection = refInfo.movedirection();
				info.pos.x = refInfo.curx();
				info.pos.y = refInfo.cury();

				m_roleMapInfo[info.mapID] = info;

				SuperRiskMapGeneratedInfo geneInfo;
				geneInfo.mapID = refInfo.mapid();
				for (int j = 0; j < refInfo.grids_size(); ++j)
				{
					const KKSG::RiskGridInfo& refGridInfo = refInfo.grids(j);
					Coordinate coor(refGridInfo.x(), refGridInfo.y());
					GridInfo gridInfo;
					gridInfo.pos = coor;
					gridInfo.gridType = refGridInfo.gridtype();
					gridInfo.state = refGridInfo.boxstate();
					if (refGridInfo.has_rewarditem())
					{
						gridInfo.itemID = refGridInfo.rewarditem().itemid();
						gridInfo.itemNum = refGridInfo.rewarditem().itemcount();
					}
					geneInfo.m_grids[coor] = gridInfo;
				}
				m_mapInfos[geneInfo.mapID] = geneInfo;
			}
		}
		if (record->has_riskinfo())
		{
			m_superRisk.Load(&record->riskinfo());
		}

		Update();
	}
	return true;
}

void CSuperRiskRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::RiskInfo2DB* record = poRoleAllInfo->mutable_riskrecord();
		record->Clear();
		record->set_getdicetime(m_getDiceTime);
		record->set_getdicenum(m_getDiceNum);
		record->set_updatetime(m_updateTime);

		KKSG::RiskMapInfos* infos = record->mutable_infos();
		infos->set_dicenum(m_diceNum);
		for (auto iter = m_boxInfos.boxInfos.begin(); iter != m_boxInfos.boxInfos.end(); ++iter)
		{
			KKSG::RiskBoxInfo* info = infos->add_boxinfos();
			info->CopyFrom(iter->second);
		}
		for (auto iter = m_mapInfos.begin(); iter != m_mapInfos.end(); ++iter)
		{
			KKSG::RiskOneMapInfo* oneInfo = infos->add_infos();
			oneInfo->set_mapid(iter->first);
			for (auto i = iter->second.m_grids.begin(); i != iter->second.m_grids.end(); ++i)
			{
				KKSG::RiskGridInfo* gridInfo = oneInfo->add_grids();
				gridInfo->set_x(i->second.pos.x);
				gridInfo->set_y(i->second.pos.y);
				gridInfo->set_gridtype((KKSG::RiskGridType)i->second.gridType);
				gridInfo->set_boxstate(i->second.state);
				KKSG::ItemBrief* brief = gridInfo->mutable_rewarditem();
				brief->set_itemid(i->second.itemID);
				brief->set_itemcount(i->second.itemNum);
			}
			auto find = m_roleMapInfo.find(iter->first);
			if (find != m_roleMapInfo.end())
			{
				oneInfo->set_curx(find->second.pos.x);
				oneInfo->set_cury(find->second.pos.y);
				oneInfo->set_movedirection(find->second.moveDirection);
			}
		}
		m_superRisk.Save(record->mutable_riskinfo());

		roChanged.insert(record);
	}
}

void CSuperRiskRecord::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	int now = time(0);

	OnRecoverDiceNum(now);

	OnTimeOpenBox(now);

	OnTimeCheckSendNtf(now);
}

void CSuperRiskRecord::OnRecoverDiceNum(int time)
{
	int now = time;
	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_SUPERRISK))
	{
		return;
	}
	if (m_getDiceNum >=  GetGlobalConfig().RiskRecoverDiceMaxNum)
	{
		return;
	}
	if (m_diceNum >=  GetGlobalConfig().RiskDiceMaxNum)
	{
		return;
	}

	int recoverInterval =  GetGlobalConfig().RiskRecoverDiceNum.seq[0] * 60;
	if (now - m_getDiceTime >= recoverInterval)
	{
		if (!recoverInterval)
		{
			return;
		}
		int count = (now - m_getDiceTime) / recoverInterval;
		int add =  GetGlobalConfig().RiskRecoverDiceNum.seq[1] * count;
		int realAdd = 0;
		if ((m_getDiceNum + add) >  GetGlobalConfig().RiskRecoverDiceMaxNum)
		{
			realAdd =  GetGlobalConfig().RiskRecoverDiceMaxNum - m_getDiceNum;
		}
		else
			realAdd = add;
		if ((m_diceNum + realAdd) >  GetGlobalConfig().RiskDiceMaxNum)
		{
			realAdd =  GetGlobalConfig().RiskDiceMaxNum - m_diceNum;
		}

		/*int lastCanRecover = 0;
		int laterRecover = 0;
		int todayRecover = 0;
		if (!XCommon::IsDaySame(m_getDiceTime, now))
		{
			lastCanRecover = (XCommon::GetDayBeginTime(m_getDiceTime) + XCommon::GetOneDayTime() - m_getDiceTime) / recoverInterval;
			if (realAdd > lastCanRecover)
			{
				laterRecover = realAdd - lastCanRecover;
				if (XCommon::IsDaySame(XCommon::GetDayBeginTime(m_getDiceTime) + XCommon::GetOneDayTime(), now))
				{
					todayRecover = laterRecover;
				}
				else
				{
				}
			}
		}
		else
		{
			todayRecover = realAdd;
		}*/


		//m_getDiceNum += realAdd;
		//m_getDiceNum += todayRecover;

		m_pRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::DICE_BACK, 1, realAdd, m_getDiceTime, now);

		AddDiceNum(realAdd);
		m_getDiceTime = now;
		Change();

	}
}

void CSuperRiskRecord::OnTimeCheckSendNtf(int time)
{
	int mapID = 0;
	int slot = 0;
	bool full = false;
	if (m_hasSendPlayNtf)
	{
		if (!HasFullDiceOrBoxReward(full, mapID, slot))
		{
			m_hasSendPlayNtf = false;
		}
		return;
	}

	if (HasFullDiceOrBoxReward(full, mapID, slot))
	{
		PtcG2C_PlayDiceNtf sendNtf;
		sendNtf.m_Data.set_isdicefull(full);
		sendNtf.m_Data.set_mapid(mapID);
		sendNtf.m_Data.set_slot(slot);
		m_pRole->Send(sendNtf);
		m_hasSendPlayNtf = true;
	}
}

void CSuperRiskRecord::OnTimeOpenBox(int time)
{
	bool change = false;
	if (m_boxInfos.OnTimer())
	{
		change = true;
	}
	if (change)
	{
		Change();
	}
}

bool CSuperRiskRecord::HasFullDiceOrBoxReward(bool& full, int& mapID, int& slot)
{
	bool send = false;
	if (GetLeftDiceNum() >= GetGlobalConfig().RiskDiceMaxNum)
	{
		full = true;
		send = true;
	}

	if (m_boxInfos.CanGetBoxReward(slot))
	{
		mapID = 1;
		send = true;
	}

	return send;
}

void CSuperRiskRecord::Update()
{
	int now = time(0);
	if (!XCommon::IsDaySame(m_updateTime, now))
	{
		m_updateTime = now;

		m_getDiceNum = 0;

		SuperRiskMapGeneratedInfo info;
		for (auto iter = m_mapInfos.begin(); iter !=m_mapInfos.end(); ++iter)
		{
			info.Clear();
			RefreshMapInfo(iter->first, false);
		}
		Change();
	}

}

void CSuperRiskRecord::Change()
{
	m_switch.Set();
}

void CSuperRiskRecord::OnDayPass()
{
	Update();
	//OnFirstEnterScene();
}

void CSuperRiskRecord::DebugReset()
{
	m_updateTime = time(0) - 86400;
	Update();
	//OnFirstEnterScene();
	Change();
}

void CSuperRiskRecord::DebugResetDice()
{
	m_diceNum = 10;
	Change();
}

void CSuperRiskRecord::OnOpenSystem()
{
	int now = time(0);
	m_updateTime = now;

	AddDiceNum( GetGlobalConfig().RiskOpenAddDiceNum);

	m_getDiceTime = now;

	Change();
}

bool CSuperRiskRecord::IsMapOpen(int mapID)
{
	auto find = m_mapInfos.find(mapID);
	if (find == m_mapInfos.end())
	{
		return false;
	}

	return true;
}

void CSuperRiskRecord::OnLevelUp()
{
	std::vector<int> mapIDs;
	SuperRiskConfig::Instance()->GetNeedOpenMapID(m_pRole->GetLevel(), mapIDs);
	for (auto iter = mapIDs.begin(); iter != mapIDs.end(); ++iter)
	{
		if (m_mapInfos.find(*iter) != m_mapInfos.end())
		{
			continue;
		}

		SuperRiskMapGeneratedInfo info;
		if(SuperRiskMapGenerator::GenerateOneRiskMapInfo(m_pRole, *iter, info))
		{
			m_mapInfos[*iter] = info;
		}
		RiskMapRoleInfo maprole;
		maprole.mapID = *iter;
		SuperRiskConfig::Instance()->GetStartNode(*iter, maprole.pos);
		m_roleMapInfo[*iter] = maprole;

		Change();
	}
}

void CSuperRiskRecord::AddDiceNum(int num, bool canOver)
{
	if (!canOver)
	{
		if (m_diceNum >=  GetGlobalConfig().RiskDiceMaxNum)
		{
			return;
		}
		int maxAdd =  GetGlobalConfig().RiskDiceMaxNum - m_diceNum;
		int add = 0;
		if (num > maxAdd)
		{
			add = maxAdd;
		}
		else
			add = num;

		m_diceNum += add;
	}
	else
	{
		m_diceNum += num;
	}

	Change();

	PtcG2C_RiskRecoverDiceNtf ntf;
	ntf.m_Data.set_dicenum(GetLeftDiceNum());
	ntf.m_Data.set_leftdicetime(GetLeftDiceTime());
	m_pRole->Send(ntf);
}

int CSuperRiskRecord::GetMaxDiceNum()
{
	auto record = m_pRole->Get<CPayV2Record>();
	int total = record->GetSuperRiskCount();

	return GetGlobalConfig().RiskDiceMaxNum + total;
}

int CSuperRiskRecord::GetLeftDiceNum()
{
	auto record = m_pRole->Get<CPayV2Record>();
	int left = record->GetRemainSuperRiskCount();

	return m_diceNum + left;
}

int CSuperRiskRecord::GetLeftDiceTime()
{
	int now = time(0);
	bool getMax = false;
	if (m_getDiceNum >=  GetGlobalConfig().RiskRecoverDiceMaxNum)
	{
		getMax = true;
	}
	if (getMax)
	{
		return -1;
	}
	if (m_diceNum >=  GetGlobalConfig().RiskDiceMaxNum)
	{
		return -1;
	}

	int needTime =  GetGlobalConfig().RiskRecoverDiceNum.seq[0] * 60;
	if (now - m_getDiceTime < needTime)
	{
		return (needTime + m_getDiceTime - now);
	}
	return 0;
}

UINT32 CSuperRiskRecord::GetMapInfo2Client(const KKSG::GetRiskMapInfosArg& roArg, KKSG::GetRiskMapInfosRes& roRes)
{
	int mapID = roArg.mapid();
	if (!mapID)
	{
		return KKSG::ERR_FAILED;
	}
	auto find = m_mapInfos.find(mapID);
	if (find == m_mapInfos.end())
	{
		return KKSG::ERR_FAILED;
	}
	if (m_roleMapInfo.find(mapID) == m_roleMapInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::RiskMapInfos* mapinfo = roRes.mutable_mapinfo();
	mapinfo->set_dicenum(GetLeftDiceNum());
	mapinfo->set_leftdicetime(GetLeftDiceTime());
	
	if (roArg.onlycountinfo())
	{
		return KKSG::ERR_SUCCESS;
	}

	KKSG::RiskOneMapInfo* oneMapInfo = mapinfo->add_infos();
	oneMapInfo->set_curx(m_roleMapInfo[mapID].pos.x);
	oneMapInfo->set_cury(m_roleMapInfo[mapID].pos.y);
	oneMapInfo->set_mapid(mapID);
	oneMapInfo->set_movedirection(m_roleMapInfo[mapID].moveDirection);
	for(auto iter = m_boxInfos.boxInfos.begin(); iter != m_boxInfos.boxInfos.end(); ++iter)
	{
		KKSG::RiskBoxInfo* boxInfo = oneMapInfo->add_boxinfos();
		boxInfo->CopyFrom(iter->second);
		boxInfo->set_lefttime(m_boxInfos.GetLeftOpenTime(iter->first));
	}
	for (auto iter = find->second.m_grids.begin(); iter != find->second.m_grids.end(); ++iter)
	{
		KKSG::RiskGridInfo* gridInfo = oneMapInfo->add_grids();
		iter->second.Convert2KKSGRiskGridInfo(*gridInfo);
	}

	return KKSG::ERR_SUCCESS;
}

const RiskMapRoleInfo* CSuperRiskRecord::GetRoleRiskMapInfo(int mapID)
{
	auto find = m_roleMapInfo.find(mapID);
	if (find == m_roleMapInfo.end())
	{
		return NULL;
	}

	return &find->second;
}

const SuperRiskMapGeneratedInfo* CSuperRiskRecord::GetGeneratedMapInfo(int mapID)
{
	auto find = m_mapInfos.find(mapID);
	if (find == m_mapInfos.end())
	{
		return NULL;
	}

	return &find->second;
}

const AllRiskBoxInfo& CSuperRiskRecord::GetBoxInfos()
{
	return m_boxInfos;
}

void CSuperRiskRecord::EndMove(int mapID, Coordinate& dest, int direction)
{
	auto find = m_roleMapInfo.find(mapID);
	if (find == m_roleMapInfo.end())
	{
		return;
	}
	find->second.moveDirection = direction;
	find->second.pos = dest;

	Change();
	
	TSuperRiskFlow oLog(m_pRole);
	oLog.m_iMapID = mapID;
	oLog.m_iCurx = dest.x;
	oLog.m_iCury = dest.y;
	oLog.m_iDirection = direction;
	oLog.Do();
	m_pRole->Get<CReportDataRecord>()->UpdateSuperRiskCount();
	LogInfo("csuperrisk curx:%d, cury:%d, direction:%d,", dest.x, dest.y, direction);
}

void CSuperRiskRecord::RefreshMapInfo(int mapID, bool isEndRefresh)
{
	auto find = m_mapInfos.find(mapID);
	if (find == m_mapInfos.end())
	{
		return;
	}
	SuperRiskMapGeneratedInfo info;
	if(SuperRiskMapGenerator::GenerateOneRiskMapInfo(m_pRole, mapID, info))
	{
		find->second = info; 
	}

	m_roleMapInfo[mapID].pos.Clear();
	SuperRiskConfig::Instance()->GetStartNode(mapID, m_roleMapInfo[mapID].pos);
	m_roleMapInfo[mapID].moveDirection = Right;

	Change();
}

int CSuperRiskRecord::AddBox(int mapID, int itemID, int itemNum)
{
	auto find = m_roleMapInfo.find(mapID);
	if (find == m_roleMapInfo.end())
	{
		return -1;
	}

	int slot = m_boxInfos.AddBox(itemID, itemNum);
	if (slot >= 0)
	{
		Change();
	}
	return slot;
}

UINT32 CSuperRiskRecord::ChangeBoxState(int mapID, int slot, KKSG::RiskBoxState state, KKSG::ChangeRiskBoxStateRes& roRes)
{
	auto find = m_roleMapInfo.find(mapID);
	if (find == m_roleMapInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 error = m_boxInfos.ChangeBoxState(m_pRole, slot, state, roRes);
	if (error == KKSG::ERR_SUCCESS)
	{
		Change();
	}
	return error;
}

bool CSuperRiskRecord::ReduceDiceNum(int num)
{
	if (GetLeftDiceNum() < num)
	{
		return false;
	}

	auto record = m_pRole->Get<CPayV2Record>();
	int left = record->GetRemainSuperRiskCount();
	if (left >= num)
	{
		record->ReduceSuperRiskCount(num);
	}
	else
	{
		int costOwn = num - left;

		if (costOwn && m_diceNum >=  GetGlobalConfig().RiskDiceMaxNum)
		{
			m_getDiceTime = time(0);
		}

		record->ReduceSuperRiskCount(left);
		m_diceNum -= costOwn;
	}

	Change();

	return true;
}

void CSuperRiskRecord::OnFirstEnterScene()
{
	OnLevelUp();
}

UINT32 CSuperRiskRecord::GetDiceRecoverFullTime()
{
	if (m_diceNum >=  GetGlobalConfig().RiskDiceMaxNum)
	{
		return 0;
	}
	UINT32 now = time(0);
	UINT32 step =  GetGlobalConfig().RiskRecoverDiceNum.seq[0] * 60;
	UINT32 maxNum = std::min<UINT32>( GetGlobalConfig().RiskDiceMaxNum, m_diceNum + ( GetGlobalConfig().RiskRecoverDiceMaxNum - m_getDiceNum));
	// 今天恢复不满了
	if (maxNum <  GetGlobalConfig().RiskDiceMaxNum)
	{
		UINT32 todayFullTime = m_getDiceTime + step * (maxNum - m_diceNum);
		UINT32 tommorrow = XCommon::GetDayBeginTime(now, false) + 86400;
		return std::max<UINT32>(todayFullTime, tommorrow) + step * ( GetGlobalConfig().RiskDiceMaxNum - maxNum);
	}

	return m_getDiceTime + step * (maxNum - m_diceNum);
}

void CSuperRiskRecord::OnLogout()
{
	RolePushInfoRecord* pushRecord = m_pRole->Get<RolePushInfoRecord>();
	UINT32 now = time(0);
	// 骰子满推送
	UINT32 fullTime = GetDiceRecoverFullTime();
	if (fullTime > now)
	{
		pushRecord->AddInfo(Push_SuperRisk, SuperRiskPush_Dice, fullTime);
	}

	// 宝箱开启推送
	{
		std::map<int, KKSG::RiskBoxInfo>& boxInfos = m_boxInfos.boxInfos;
		for (auto it = boxInfos.begin(); it != boxInfos.end(); ++it)
		{
			if (it->second.state() != KKSG::RISK_BOX_UNLOCKED)
			{
				continue;
			}
			int leftTime = m_boxInfos.GetLeftOpenTime(it->first);
			if (leftTime <= 0)
			{
				continue;
			}
			pushRecord->AddInfo(Push_SuperRisk, SuperRiskPush_Box, leftTime + now);
		}
	}
}
