#include "pch.h"
#include "towerrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "activity/rpcc2g_gettoweractivitytop.h"
#include "gamelogic/teamhandler.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/team.h"
#include "gamelogic/towerconfig.h"
#include "findback/findbackrecord.h"
#include "utility/tlogger.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "network/mslink.h"
#include "event/eventmgr.h"

bool RoleTowerInfo::InSweep()
{
	return sweepTime > 0;
}

bool RoleTowerInfo::IsSweepOver()
{
	return sweepFloor > 0;
}

void RoleTowerInfo::OnSweepOver()
{
	sweepFloor = reachTopFloor;
	sweepTime = 0;

	curCostTime = bestTime;
	//curFloor = reachTopFloor;
}

CTowerRecord::CTowerRecord(Role* role)
{
	m_pRole = role;

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	m_updateTime = 0;

	m_useCount = 0;
}

CTowerRecord::~CTowerRecord()
{
}

bool CTowerRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_towerrecord())
	{
		const KKSG::TowerRecord2DB* record = &poRoleAllInfo->towerrecord();
		m_useCount = record->useresetcount();
		m_updateTime = record->m_updatetime();

		for (int i = 0; i < record->records_size(); ++i)
		{
			RoleTowerInfo info;
			info.openHardLevel = record->records(i).openhardlevel();
			if (!info.openHardLevel)
			{
				Change();
				continue;
			}
			info.reachTopFloor = record->records(i).reachtopfloor();
			info.bestTime = record->records(i).besttime();
			if (record->records(i).has_sweeptime())
			{
				info.sweepTime = record->records(i).sweeptime();
			}
			if (record->records(i).has_sweepfloor())
			{
				info.sweepFloor = record->records(i).sweepfloor();
			}
			info.curFloor = record->records(i).curfloor();
			info.curCostTime = record->records(i).curcosttime();
			info.refreshCount = record->records(i).refreshcount();
			info.refreshResult = record->records(i).refreshresult();
			for (int j = 0; j < record->records(i).gotfloorfirstpassreward_size(); ++j)
			{
				info.gotFirstPassRewards.push_back(record->records(i).gotfloorfirstpassreward(j));
			}
			/*if (info.curFloor > info.reachTopFloor)
			{
				info.reachTopFloor = info.curFloor;
				info.bestTime = info.curCostTime;
			}*/
			m_towerInfo[info.openHardLevel] = info;
		}

		Update();
	}
	return true;
}

void CTowerRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::TowerRecord2DB* record = poRoleAllInfo->mutable_towerrecord();
		record->Clear();
		record->set_m_updatetime(m_updateTime);
		record->set_useresetcount(m_useCount);

		for (auto iter = m_towerInfo.begin(); iter != m_towerInfo.end(); ++iter)
		{
			KKSG::TowerRecord* oneRecord = record->add_records();
			oneRecord->set_openhardlevel(iter->second.openHardLevel);
			oneRecord->set_reachtopfloor(iter->second.reachTopFloor);
			oneRecord->set_besttime(iter->second.bestTime);
			oneRecord->set_sweeptime(iter->second.sweepTime);
			oneRecord->set_sweepfloor(iter->second.sweepFloor);
			oneRecord->set_curcosttime(iter->second.curCostTime);
			oneRecord->set_curfloor(iter->second.curFloor);
			oneRecord->set_refreshresult(iter->second.refreshResult);
			oneRecord->set_refreshcount(iter->second.refreshCount);

			for (auto i = iter->second.gotFirstPassRewards.begin(); i != iter->second.gotFirstPassRewards.end(); ++i)
			{
				oneRecord->add_gotfloorfirstpassreward(*i);
			}
		}

		roChanged.insert(record);
	}
}

void CTowerRecord::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Update();
}

bool CTowerRecord::AutoDestory()
{
	CTimerMgr::Instance()->KillTimer(m_timer);
	return true;
}

void CTowerRecord::Change()
{
	m_switch.Set();
}

void CTowerRecord::Update()
{
	int now = time(0);
	if (!XCommon::IsDaySame(m_updateTime, now))
	{
		m_updateTime = now;
		m_useCount = 0;

		Change();
	}
	foreach(i in m_towerInfo)
	{
		if(i->second.InSweep())
		{
			if(!GetSweepLeftTime(i->first))
			{
				i->second.OnSweepOver();

				CheckSweepTopestFloor(i->first);

				Change();
			}
		}
	}
}

void CTowerRecord::DebugReset(bool resetInfo)
{
	int now = (int)time(0);

	if (resetInfo)
	{
		m_towerInfo.clear();
	}

	m_useCount = 0;

	Update();

	const TeamTowerRewardTable& data = TowerConfig::Instance()->GetTowerTable();
	for (auto iter = data.Table.begin(); iter != data.Table.end(); ++iter)
	{
		m_pRole->Get<StageMgr>()->DebugResetStageRank((*iter)->SceneID);
	}

	Change();
}

void CTowerRecord::DebugPassTower(UINT32 floor)
{
	m_towerInfo[DEFAULT_HARDLEVEL].ResetTower();
	m_towerInfo[DEFAULT_HARDLEVEL].curFloor = floor;
	m_towerInfo[DEFAULT_HARDLEVEL].openHardLevel = DEFAULT_HARDLEVEL;
	m_towerInfo[DEFAULT_HARDLEVEL].reachTopFloor = floor;
	m_towerInfo[DEFAULT_HARDLEVEL].gotFirstPassRewards.clear();
	Change();
}

bool CTowerRecord::CheckIsBestInfo(RoleTowerInfo& info)
{
	auto find = m_towerInfo.find(info.openHardLevel);
	if (find == m_towerInfo.end())
	{
		return true;
	}
	else/* if (info.openHardLevel == m_towerInfo.openHardLevel)*/
	{
		//if (info.reachTopFloor > find->second.reachTopFloor)
		if (info.curFloor > find->second.reachTopFloor)
		{
			return true;
		}
		else if (info.curFloor == find->second.reachTopFloor)
		{
			if (info.curCostTime < find->second.bestTime)
			{
				return true;
			}

			return false;
		}
		else
			return false;
	}

}

void CTowerRecord::UpdateBestInfo(RoleTowerInfo& info)
{
	auto& curInfo = m_towerInfo[info.openHardLevel];

	EventMgr::Instance()->AddGuildBonusEvent(m_pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_TOWER, curInfo.reachTopFloor, info.curFloor);

	curInfo.openHardLevel = info.openHardLevel;
	curInfo.bestTime = info.curCostTime;
	curInfo.reachTopFloor = info.curFloor;
	m_switch.Set();

	UpdateTowerRank(m_towerInfo[info.openHardLevel]);
}

void CTowerRecord::UpdateTowerRank(RoleTowerInfo& info)
{
	RpcG2M_UpdateMSRankList* updateRpc = RpcG2M_UpdateMSRankList::CreateRpc();
	updateRpc->m_oArg.set_ranktype(KKSG::TowerRank);
	KKSG::RankData* rankData = updateRpc->m_oArg.mutable_info();
	rankData->add_roleids(m_pRole->GetID());
	rankData->set_towerhardlevel(DEFAULT_HARDLEVEL);
	rankData->set_towerfloor(info.reachTopFloor);
	rankData->set_towerthroughtime(info.bestTime);
	rankData->set_time((UINT32)time(0));
	m_pRole->SendToMS(*updateRpc);
}

void CTowerRecord::UpdateCurInfo(RoleTowerInfo& info)
{
	m_towerInfo[info.openHardLevel].openHardLevel = info.openHardLevel;
	m_towerInfo[info.openHardLevel].curCostTime = info.curCostTime;
	m_towerInfo[info.openHardLevel].curFloor = info.curFloor;
	Change();

	m_pRole->OnThroughTower(info.curFloor);
}

int CTowerRecord::GetStartFloor(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return 1;
	}

	return find->second.curFloor + 1;
}

void CTowerRecord::OnLogin()
{
	for (auto iter = m_towerInfo.begin(); iter != m_towerInfo.end(); ++iter)
	{
		RoleTowerInfo tempInfo;
		tempInfo.openHardLevel = iter->first;
		tempInfo.curCostTime = iter->second.curCostTime;
		tempInfo.curFloor = iter->second.curFloor;
		if (CheckIsBestInfo(tempInfo))
		{
			UpdateBestInfo(tempInfo);
			Change();
		}
	}
}

const RoleTowerInfo* CTowerRecord::GetTowerInfo(int hardLevel)
{
	if (m_towerInfo.find(hardLevel) == m_towerInfo.end())
	{
		return NULL;
	}

	return &m_towerInfo.find(hardLevel)->second;
}

void CTowerRecord::FillTopInfo2Client(KKSG::GetTowerActivityTopRes& roRes)
{
	for (auto iter = m_towerInfo.begin(); iter != m_towerInfo.end(); ++iter)
	{
		KKSG::TowerRecord* info = roRes.add_infos();
		info->set_openhardlevel(iter->second.openHardLevel);
		info->set_reachtopfloor(iter->second.reachTopFloor);
		info->set_besttime(iter->second.bestTime);
		info->set_sweepfloor(GetSweepAlreadyFloor(iter->second.openHardLevel));
		info->set_sweeptime(GetSweepLeftTime(iter->second.openHardLevel));
		info->set_curfloor(iter->second.curFloor);
		info->set_curcosttime(iter->second.curCostTime);
		info->set_refreshcount( GetGlobalConfig().RefreshSweepCost.size() - iter->second.refreshCount);
		info->set_refreshresult(iter->second.refreshResult);
		for (auto i = iter->second.gotFirstPassRewards.begin(); i != iter->second.gotFirstPassRewards.end(); ++i)
		{
			info->add_gotfloorfirstpassreward(*i);
		}
	}
	roRes.set_leftresetcount( GetGlobalConfig().GetInt("TowerTeamDayCount", 2) - m_useCount);
}

UINT32 CTowerRecord::CheckEnterScene(int hardLevel, int floor)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		if (floor == 1)
		{
			return KKSG::ERR_SUCCESS;
		}
		else
			return KKSG::ERR_FAILED;
	}
	if (find->second.InSweep() || find->second.IsSweepOver())
	{
		return KKSG::ERR_FAILED;
	}

	if (floor == (find->second.curFloor + 1))
	{
		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_FAILED;
}

UINT32 CTowerRecord::SweepTower(UINT32 hardLevel, UINT32 itemID, UINT32 count, UINT32& leftTime)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.IsSweepOver())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.InSweep() && !itemID)
	{
		return KKSG::ERR_FAILED;
	}

	//V2
	int floor = find->second.reachTopFloor;

	if (find->second.curFloor >= floor)
	{
		return KKSG::ERR_FAILED;
	}

	if (itemID)
	{
		int need = 0;
		bool bItemIDValid = false;
		foreach(i in  GetGlobalConfig().SweepTowerCost)
		{
			if ((*i).seq[0] == hardLevel && (*i).seq[1] == itemID)
			{
				need = (*i).seq[2];
				bItemIDValid = true;
				break;
			}
		}
		if(!bItemIDValid)
			return KKSG::ERR_FAILED;
		if (need)
		{
			BagTakeItemTransition transition(m_pRole);
			transition.SetReason(ItemFlow_Tower, ItemFlow_Tower_Sweep);
			if (!transition.TakeItem(itemID, need))
			{
				transition.RollBack();
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}	
			transition.NotifyClient();
		}

		find->second.OnSweepOver();

		CheckSweepTopestFloor(hardLevel);
	}
	else
	{
		find->second.sweepTime = time(0);
	}

	leftTime = GetSweepLeftTime(hardLevel);

	Change();

	return KKSG::ERR_SUCCESS;
}

void CTowerRecord::CheckSweepTopestFloor(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return;
	}

	EventMgr::Instance()->AddEvent(m_pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_TowerFloor, 0, find->second.reachTopFloor);

}

void CTowerRecord::GiveSweepReward(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return;
	}
	if (find->second.InSweep())
	{
		return;
	}
	int refreshResult = find->second.refreshResult;
	if (!refreshResult)
	{
		refreshResult = 100;
	}
	std::map<int, int> rewards;
	TowerConfig::Instance()->GetTowerReward(hardLevel, find->second.curFloor, find->second.sweepFloor, rewards);
	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_Tower, ItemFlow_Tower_Sweep);
	foreach(i in rewards)
	{
		int temp = i->second * refreshResult;
		int tempNum = temp / 100;
		if(temp % 100 >= 50)
			++tempNum;

		transition.GiveItem(i->first, tempNum);
	}	
	transition.NotifyClient();

	find->second.sweepFloor = 0;
	find->second.curFloor = find->second.reachTopFloor;
	Change();

	//扫荡成功记录一条记录;
	TRoundFlowLog oLog(m_pRole);
	oLog.m_nBattleType = KKSG::SCENE_TOWER;
	oLog.m_nResult = TX_BATTLE_SWEEP;
	oLog.Do();
}

UINT32 CTowerRecord::GetSweepAlreadyFloor(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end() || !find->second.reachTopFloor)
	{
		return 0;
	}

	if (find->second.sweepFloor)
	{
		return find->second.sweepFloor;
	}

	if (!find->second.sweepTime)
	{
		return 0;
	}

	int now = time(0);
	int eachTime = TowerConfig::Instance()->GetSweepEachLevelTime(hardLevel);
	if (eachTime)
	{
		return ((now - find->second.sweepTime) % eachTime + 1);
	}

	return 0;
}

UINT32 CTowerRecord::GetSweepLeftTime(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return -1;
	}

	if (find->second.sweepFloor)
	{
		return 0;
	}

	if (!find->second.sweepTime)
	{
		return 0;
	}

	int needTime = 0;
	int floor = find->second.reachTopFloor;
	int start = find->second.curFloor;
	if (start > floor)
	{
		return 0;
	}
	int eachTime = TowerConfig::Instance()->GetSweepEachLevelTime(hardLevel);
	if (eachTime)
	{
		needTime = eachTime * (floor - start);
	}

	int now = time(0);

	return (find->second.sweepTime + needTime > now) ? (find->second.sweepTime + needTime - now) : 0;
}

UINT32 CTowerRecord::ResetTower(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.sweepTime)
	{
		return KKSG::ERR_FAILED;
	}

	int total =  GetGlobalConfig().GetInt("TowerTeamDayCount", 2);
	if (m_useCount >= total)
	{
		return KKSG::ERR_TEAM_TOWER_DAYCOUNT;
	}

	++m_useCount;

	find->second.ResetTower();

	Change();

	m_pRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::TOWER, 1, 1);

	return KKSG::ERR_SUCCESS;
}

UINT32 CTowerRecord::AutoResetTower(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	find->second.ResetTower();

	Change();

	return KKSG::ERR_SUCCESS;
}

UINT32 CTowerRecord::GetFirstPassReward(int hardLevel, int floor)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		LogError("tower, role:%llu, invalid hardlevel, hardlevel:%d,floor:%d.", m_pRole->GetID(), hardLevel, floor);
		return KKSG::ERR_FAILED;
	}

	if (floor > find->second.reachTopFloor)
	{
		LogError("tower, role:%llu, invalid floor, hardlevel:%d,floor:%d, topfloor: %d", m_pRole->GetID(), hardLevel, floor, find->second.reachTopFloor);
		return KKSG::ERR_FAILED;
	}

	TeamTowerRewardTable::RowData* data = NULL;
	std::vector<TeamTowerRewardTable::RowData*> vec;
	vec = TowerConfig::Instance()->GetTowerFloorHasFirstPassReward(hardLevel);
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		if ((*iter)->TowerFloor == floor)
		{
			data = *iter;
			break;
		}
	}
	if (!data)
	{
		LogError("tower, role:%llu, invalid floor in xml, hardlevel:%d,floor:%d.", m_pRole->GetID(), hardLevel, floor);
		return KKSG::ERR_FAILED;
	}
	for (auto iter = find->second.gotFirstPassRewards.begin(); iter != find->second.gotFirstPassRewards.end(); ++iter)
	{
		if (*iter == floor)
		{
			LogError("tower, role:%llu, already got, hardlevel:%d,floor:%d.", m_pRole->GetID(), hardLevel, floor);
			return KKSG::ERR_FAILED;
		}
	}

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_Tower, ItemFlow_Tower_FirstPass);
	for (auto iter = data->FirstPassReward.begin(); iter != data->FirstPassReward.end(); ++iter)
	{
		transition.GiveItem(iter->seq[0], iter->seq[1]);
	}	
	transition.NotifyClient();

	find->second.gotFirstPassRewards.push_back(floor);

	Change();

	LogInfo("tower, role:%llu,get reward, hardlevel:%d,floor:%d.", m_pRole->GetID(), hardLevel, floor);
	return KKSG::ERR_SUCCESS;
}

UINT32 CTowerRecord::GetSweepFloor(int hardLevel)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return 0;
	}

	return find->second.sweepFloor;
}

UINT32 CTowerRecord::RefreshSweepReward(int hardLevel, int& result)
{
	auto find = m_towerInfo.find(hardLevel);
	if (find == m_towerInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.refreshCount >=  GetGlobalConfig().RefreshSweepCost.size())
	{
		return KKSG::ERR_TEAM_TOWER_DAYCOUNT;
	}

	int costItem =  GetGlobalConfig().RefreshSweepCost[find->second.refreshCount].seq[0];
	int costNum =  GetGlobalConfig().RefreshSweepCost[find->second.refreshCount].seq[1];

	if (costItem)
	{
		BagTakeItemTransition takeTransiton(m_pRole);
		takeTransiton.SetReason(ItemFlow_Tower, ItemFlow_Tower_RefreshSweep);
		if(!takeTransiton.TakeItem(costItem, costNum))
		{
			takeTransiton.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}	

		takeTransiton.NotifyClient();
	}

	++find->second.refreshCount;

	result = find->second.refreshResult = GetRefreshSweepRandValue();

	Change();

	return KKSG::ERR_SUCCESS;
}

UINT32 CTowerRecord::GetRefreshSweepRandValue()
{
	int total = 0;
	for (auto iter =  GetGlobalConfig().RefreshSweepRandValue.begin(); iter !=  GetGlobalConfig().RefreshSweepRandValue.end(); ++iter)
	{
		total += iter->seq[1];
	}

	int value = 0;
	int rand = XRandom::randInt(0, total + 1);
	for (auto iter =  GetGlobalConfig().RefreshSweepRandValue.begin(); iter !=  GetGlobalConfig().RefreshSweepRandValue.end(); ++iter)
	{
		value += iter->seq[1];
		if (rand <= value)
		{
			return iter->seq[0];
		}
	}
	
	return 0;
}

