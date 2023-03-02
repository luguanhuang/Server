#include "pch.h"
#include "unit/role.h"
#include "spactivitysmall.h"
#include "spactivityrecord.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivitydef.h"
#include "table/OpenSystemMgr.h"
#include "util/gametime.h"
#include "globalconfig.h"
#include "pb/project.pb.h"
#include "spactivityconfig.h"
#include "gamelogic/levelsealMgr.h"
#include "define/tlogenum.h"
#include "spactivityhandler.h"
#include "gamelogic/bag.h"
#include "spactivity/ptcg2c_spactivityoffsetdayntf.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/worldlevelmgr.h"
#include "gamelogic/payv2Mgr.h"


SpActivityOpenServer::SpActivityOpenServer(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_OpenServer);
}

SpActivityOpenServer::~SpActivityOpenServer()
{

}

bool SpActivityOpenServer::Init()
{
	return LoadFile();
}

bool SpActivityOpenServer::CheckFile()
{
	OpenServerActivity tmpTable;
	if (!tmpTable.LoadFile("table/OpenServerActivity.txt"))
	{
		LogWarn("Load OpenServerActivity.txt failed");
		return false;
	}
	return true;
}

bool SpActivityOpenServer::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();

	m_oTable.LoadFile("table/OpenServerActivity.txt");

	return true;
}

void SpActivityOpenServer::ClearFile()
{
	m_oTable.Clear();
}

void SpActivityOpenServer::OnStart()
{
	UINT32 level = 0;
	if (GSConfig::Instance()->IsBackFlowServer())
	{
		level = GSConfig::Instance()->GetBackFlowLevel();
	}
	OpenServerActivity::RowData* conf = m_oTable.GetByServerLevel(level);
	if (conf == NULL)
	{
		LogWarn("ServerLevel [%u] not find in OpenServerActivity.txt", level);
		return;
	}
	std::vector<SpActivityTaskConf*> vec;
	SpActivityConfig::Instance()->GetTaskConfsByTaskIDs(conf->TaskIDs, vec);
	StartOnlinePlayer(vec);
}

void SpActivityOpenServer::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;
	///> 未开启
	if (m_stage == 0)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	
	bool isOpenNow = false;
	///> startTime 与当前不一样 重置任务
	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}
	if (pDataBase != NULL && 
		(pDataBase->GetStartTime() != m_startTime))
	{
		isOpenNow = true;
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
	}

	///> 开服狂欢比较特殊，任务可以先开始完成，但是system先不开(egg pain!!!!!)
	bool canOpen = CheckOpenCondition(pRole);
	if (canOpen)
	{
		OpenSpActivitySystem(pRole, conf->systemid);
	}

	///> 未开启活动 or 登录 才会更新任务
	if (isOpenNow || pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		UINT32 level = 0;
		if (GSConfig::Instance()->IsBackFlowServer())
		{
			level = GSConfig::Instance()->GetBackFlowLevel();
		}
		OpenServerActivity::RowData* conf = m_oTable.GetByServerLevel(level);
		if (conf == NULL)
		{
			LogWarn("ServerLevel [%u] not find in OpenServerActivity.txt", level);
			std::vector<SpActivityTaskConf*> vec;
			pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &vec);
		}
		else
		{
			std::vector<SpActivityTaskConf*> vec;
			SpActivityConfig::Instance()->GetTaskConfsByTaskIDs(conf->TaskIDs, vec);
			pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &vec);
		}
	}

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityPartnerDay::SpActivityPartnerDay(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_PartnerDay);
}

SpActivityPartnerDay::~SpActivityPartnerDay()
{

}

void SpActivityPartnerDay::OnNextStage()
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		pRole->Get<SpActivityRecord>()->AddSpActStage(m_actid);
		pRole->Get<SpActivityRecord>()->ResetSpActTask(m_actid);
	}
}

KKSG::ErrorCode SpActivityPartnerDay::GetReward(Role* pRole, UINT32 taskid)
{
	const SpActivityTaskConf* taskConf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (taskConf == NULL)
	{
		LogWarn("can't find SpActivityTaskConf, taskid:%u", taskid);
		return KKSG::ERR_UNKNOWN;
	}
	return pRole->Get<SpActivityRecord>()->GetReward(m_actid, taskid, taskConf->items, ItemFlow_SpActivityPartner_GetReward);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityBackFlow::SpActivityBackFlow(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_BackFlow);
}

SpActivityBackFlow::~SpActivityBackFlow()
{

}

bool SpActivityBackFlow::Init()
{
	return LoadFile(false);
}

bool SpActivityBackFlow::CheckFile()
{
	BackflowShop tmpBackFlowShop;
	if (!tmpBackFlowShop.LoadFile("table/BackFlowShop.txt"))
	{
		LogWarn("Load BackflowShop.txt failed");
		return false;
	}
	for (auto i = tmpBackFlowShop.Table.begin(); i != tmpBackFlowShop.Table.end(); ++i)
	{
		BackflowShopConf* conf = *i;
		if (conf->LevelRange[1] < conf->LevelRange[0] 
			|| conf->LostDayRange[1] < conf->LostDayRange[0]
			|| conf->PayRange[1] < conf->PayRange[0])
		{
			LogWarn("GoodID [%u] Range error", conf->GoodID);
			return false;
		}
	}

	return true;
}

bool SpActivityBackFlow::LoadFile(bool isReload)
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oBackFlowShop.LoadFile("table/BackFlowShop.txt");

	return true;
}

void SpActivityBackFlow::ClearFile()
{
	m_oBackFlowShop.Clear();
}

void SpActivityBackFlow::OnStart()
{

}

void SpActivityBackFlow::OnEnd()
{

}

bool SpActivityBackFlow::CheckOpenCondition(Role* pRole)
{
	if (pRole == NULL) return false;

	UINT32 lastBackFlowStartTime = pRole->Get<SpActivityRecord>()->GetLastBackFlowStartTime();
	return lastBackFlowStartTime + GetGlobalConfig().BackFlowActivityDayTime * 24 * 3600 >= GameTime::GetTime();
}

void SpActivityBackFlow::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;

	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	bool isOpenNow = false;
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	UINT32 lastOfflineTime = pRole->GetRoleAllInfo().brief().offlinetime();
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
	
		UINT32 lastBackFlowStartTime = pRole->Get<SpActivityRecord>()->GetLastBackFlowStartTime();	
		if (!pRole->GetFlag(RSF_ISWaitLogin)	// 不是登录
			|| m_stage == 0						// 未开启活动
			|| lastBackFlowStartTime + GetGlobalConfig().BackFlowStartAgainMinDayTime * 24 * 3600 > nowTime
			|| !pRole->Get<SpActivityRecord>()->CheckBackFlowOpen()
			|| pRole->GetLevel() < conf->minlevel)
		{
			return;
		}
		LogInfo("start BackFlow, role [%llu], lastOfflineTime [%u]", pRole->GetID(), lastOfflineTime);
		pRole->Get<SpActivityRecord>()->SetLastBackFlowStartTime(nowTime);
		isOpenNow = true;
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}

	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(pRole->Get<SpActivityRecord>()->AddAndGetSpActData(m_actid, m_startTime, m_stage));
	if (isOpenNow)
	{
		backflow->SetInitData(WorldLevelMgr::Instance()->GetLevel(), (nowTime - lastOfflineTime) / (24 * 3600));
	}

	///> 未开启活动 or 登录 才会更新任务
	if (isOpenNow || pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		std::vector<SpActivityTaskConf*> vec;
		FillBackFlowActivityTask(backflow->GetWorldLevel(), vec);
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &vec);
	}	

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

KKSG::ErrorCode SpActivityBackFlow::GetReward(Role* pRole, UINT32 taskid)
{
	const SpActivityTaskConf* taskConf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (taskConf == NULL)
	{
		LogWarn("can't find SpActivityTaskConf, taskid:%u", taskid);
		return KKSG::ERR_UNKNOWN;
	}
	return pRole->Get<SpActivityRecord>()->GetReward(m_actid, taskid, taskConf->items, ItemFlow_SpActivityBackFlow_GetReward);
}

void SpActivityBackFlow::FillBackFlowActivityTask(UINT32 worldLevel, std::vector<SpActivityTaskConf*>& tasks)
{
	const BackflowActivity& backflowActivity = SpActivityConfig::Instance()->GetBackflowActivityTable();
	std::vector<UINT32> taskids;
	for (auto i = backflowActivity.Table.begin();i != backflowActivity.Table.end(); ++i)
	{
		if (worldLevel >= (*i)->WorldLevel[0] && worldLevel <= (*i)->WorldLevel[1])
		{
			taskids.push_back((*i)->TaskId);
		}
	}
	SpActivityConfig::Instance()->GetTaskConfsByTaskIDs(taskids, tasks);
}

const BackflowShopConf* SpActivityBackFlow::GetBackflowShopConf(UINT32 goodid)
{
	return m_oBackFlowShop.GetByGoodID(goodid);
}

void SpActivityBackFlow::GetNewShopGoodsList(Role* pRole, UINT32 lostDay, std::vector<UINT32>& newGoods)
{
	if (pRole == NULL) return;
	UINT32 roleLevel = pRole->GetLevel();
	UINT32 totalAmt = pRole->Get<CPayV2Record>()->GetSaveAmt();
	std::unordered_set<BackflowShopConf*> vecShopGoods;
	UINT32 totalWeight = 0;
	for (auto i = m_oBackFlowShop.Table.begin(); i != m_oBackFlowShop.Table.end(); ++i)
	{
		BackflowShopConf* conf = *i;
		if (conf->LevelRange[0] <= roleLevel && conf->LevelRange[1] >= roleLevel
			&& conf->LostDayRange[0] <= lostDay && conf->LostDayRange[1] >= lostDay
			&& conf->PayRange[0] <= totalAmt && conf->PayRange[1] >= totalAmt
			&& conf->Weight != 0)
		{
			vecShopGoods.insert(conf);
			totalWeight += conf->Weight;
		}
	}
	UINT32 needCount = GetGlobalConfig().BackFlowShopGoodCount;
	while (needCount--)
	{
		if (totalWeight == 0 || vecShopGoods.empty())
		{
			return;
		}
		UINT32 random = XRandom::randInt(1, totalWeight + 1), prob = 0;
		for (auto i = vecShopGoods.begin(); i != vecShopGoods.end(); ++i)
		{
			prob += (*i)->Weight;
			if (prob >= random)
			{
				newGoods.push_back((*i)->GoodID);
				totalWeight -= (*i)->Weight;
				vecShopGoods.erase(i);
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityArgenta::SpActivityArgenta(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_Argenta);
}

SpActivityArgenta::~SpActivityArgenta()
{

}

bool SpActivityArgenta::Init()
{
	return LoadFile();
}

bool SpActivityArgenta::CheckFile()
{
	ArgentaDaily tmpDaily;
	if (!tmpDaily.LoadFile("table/ArgentaDaily.txt"))
	{
		LogWarn("Load ArgentaDaily.txt failed");
		return false;
	}

	ArgentaTask tmpTask;
	if (!tmpTask.LoadFile("table/ArgentaTask.txt"))
	{
		LogWarn("Load ArgentaTask.txt failed");
		return false;
	}
	for (auto i = tmpTask.Table.begin(); i != tmpTask.Table.end(); i++)
	{
		if ((*i)->LevelRange[0] > (*i)->LevelRange[1])
		{
			LogWarn("ArgentaTask.txt error, taskid [%u], LevelRange Wrong", (*i)->TaskID);
			return false;
		}
	}

	return true;
}

bool SpActivityArgenta::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oDailyTable.LoadFile("table/ArgentaDaily.txt");
	m_oTaskTable.LoadFile("table/ArgentaTask.txt");

	return true;
}

void SpActivityArgenta::ClearFile()
{
	m_oDailyTable.Clear();
	m_oTaskTable.Clear();
}

void SpActivityArgenta::OnStart()
{

}

void SpActivityArgenta::OnEnd()
{

}

bool SpActivityArgenta::CheckOpenCondition(Role* pRole)
{
	if (pRole == NULL) return false;

	SpActArgentaData* pArgentData = static_cast<SpActArgentaData*>(pRole->Get<SpActivityRecord>()->GetSpActData(m_actid));
	if (pArgentData == NULL)
	{
		return false;
	}
	return pArgentData->GetArgentaStartTime() + GetGlobalConfig().ArgentaLastTime * 3600 >= GameTime::GetTime();
}

void SpActivityArgenta::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;

	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}

	bool isOpenNow = false;
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);

		UINT32 nowTime = (UINT32)GameTime::GetTime();
		UINT32 lastArgentaStartTime = pRole->Get<SpActivityRecord>()->GetLastArgentaStartTime();
		if (!pRole->GetFlag(RSF_ISWaitLogin)			// 不是登录
			|| m_stage == 0
			|| lastArgentaStartTime + (GetGlobalConfig().ArgentaLastTime + GetGlobalConfig().ArgentaEndToStartLimit) * 3600 > nowTime
			|| pRole->GetLevel() < conf->minlevel
			|| !pRole->Get<SpActivityRecord>()->CheckArgentaOpen())
		{
			return;
		}
		LogInfo("start Argenta, role [%llu], lastArgentaStartTime [%u]", pRole->GetID(), lastArgentaStartTime);
		pRole->Get<SpActivityRecord>()->SetLastArgentaStartTime(nowTime);
		isOpenNow = true;
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->AddAndGetSpActData(m_actid, m_startTime, m_stage);
	SpActArgentaData* pArgentData = static_cast<SpActArgentaData*>(pDataBase);

	if (pArgentData->GetLevel() == 0)
	{
		pArgentData->SetInitialData();
	}

	///> 登录才会触发 or 更新任务
	if (pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		std::vector<UINT32> taskid;
		for (auto i = m_oTaskTable.Table.begin(); i != m_oTaskTable.Table.end(); i++)
		{
			if ((*i)->LevelRange[0] <= pArgentData->GetLevel() 
				&& (*i)->LevelRange[1] >= pArgentData->GetLevel())
			{
				taskid.push_back((*i)->TaskID);
			}
		}
		std::vector<SpActivityTaskConf*> vec;
		SpActivityConfig::Instance()->GetTaskConfsByTaskIDs(taskid, vec);
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &vec);
	}	

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

KKSG::ErrorCode SpActivityArgenta::GetReward(Role* pRole, UINT32 taskid)
{
	SpActArgentaData* pArgentData = static_cast<SpActArgentaData*>(pRole->Get<SpActivityRecord>()->GetSpActData(m_actid));
	if (pArgentData == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	for (auto i = m_oTaskTable.Table.begin(); i != m_oTaskTable.Table.end(); i++)
	{
		if ((*i)->TaskID == taskid
			&& (*i)->LevelRange[0] <= pArgentData->GetLevel()
			&& (*i)->LevelRange[1] >= pArgentData->GetLevel())
		{
			return pArgentData->GetReward(taskid, (*i)->Reward, ItemFlow_SpActivityArgenta_GetReward);
		}
	}
	return KKSG::ERR_UNKNOWN;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityAncient::SpActivityAncient(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_AncientTimes);
}

SpActivityAncient::~SpActivityAncient()
{

}

bool SpActivityAncient::Init()
{
	return LoadFile();
}

bool SpActivityAncient::CheckFile()
{
	AncientTimesTable tmpAncientTimes;
	if(!tmpAncientTimes.LoadFile("table/AncientTimesTable.txt"))
	{
		LogError("Load AncientTimes.txt failed!");
		return false;
	}
    return true;
}

bool SpActivityAncient::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oAncientTimes.LoadFile("table/AncientTimesTable.txt");

	return true;
}

void SpActivityAncient::ClearFile()
{
	m_oAncientTimes.Clear();
}

void SpActivityAncient::OnActivityOpen(Role* pRole)
{
	if (pRole == NULL) return;
	std::set<UINT32> itemids;
	for (auto i = m_oAncientTimes.Table.begin(); i != m_oAncientTimes.Table.end(); ++i)
	{
		itemids.insert((*i)->nPoints[0]);
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_SpActivity, ItemFlow_AncientTimesTake);
	for (auto i = itemids.begin(); i != itemids.end(); ++i)
	{
		take.TakeItem(*i, pRole->Get<Bag>()->CountItemTotal(*i));
	}
	take.NotifyClient();
}

UINT32 SpActivityAncient::GetAcientTimesTableSize()
{
	return m_oAncientTimes.Table.size();
}

std::vector<Sequence<uint, 2>> SpActivityAncient::GetAcientTimesAward(Role* pRole, UINT32 nPos)
{
	std::vector<Sequence<uint, 2>> awarditems;
	if (pRole==NULL||nPos > m_oAncientTimes.Table.size())
		return awarditems;
	UINT32 itemCount = pRole->Get<Bag>()->CountVirtualItem(m_oAncientTimes.Table[nPos-1]->nPoints[0]);
	UINT32 needCount = m_oAncientTimes.Table[nPos-1]->nPoints[1];
	if (itemCount>=needCount)
	{
		awarditems = m_oAncientTimes.Table[nPos-1]->Items;
	}
	return awarditems;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityTheme::SpActivityTheme(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_BioHell || actid == SpActivity_Duck);
}

SpActivityTheme::~SpActivityTheme()
{

}

void SpActivityTheme::OnNextStage()
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		pRole->Get<SpActivityRecord>()->AddSpActStage(m_actid);
		SpActThemeData* themeData = static_cast<SpActThemeData*>(pRole->Get<SpActivityRecord>()->GetSpActData(m_actid));
		if (themeData != NULL)
		{
			themeData->SetHint(true);
		}
	}
}

void SpActivityTheme::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;
	///> 未开启
	if (m_stage == 0)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	bool isOpenNow = false;
	///> startTime 与当前不一样 重置任务
	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}
	else
	{
		if (pDataBase->GetStartTime() != m_startTime)
		{
			isOpenNow = true;
			pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		}
		else if (pDataBase->GetActStage() != m_stage)
		{
			SpActThemeData* pThemeData = static_cast<SpActThemeData*>(pDataBase);
			pThemeData->SetHint(true);
		}
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	///> 未开启活动 or 登录 才会更新任务
	if (isOpenNow || pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, NULL);
	}

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActivityWeekEndNest::SpActivityWeekEndNest(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_WeekEndNest);
}

SpActivityWeekEndNest::~SpActivityWeekEndNest()
{
}

bool SpActivityWeekEndNest::Init()
{
	return LoadFile();
}

bool SpActivityWeekEndNest::CheckFile()
{
	WeekEndNestActivity tmpWeekEndNestActivity;
	if(!tmpWeekEndNestActivity.LoadFile("table/WeekEndNestActivity.txt"))
	{
		LogError("Load WeekEndNestActivity.txt failed!");
		return false;
	}
	return true;
}

bool SpActivityWeekEndNest::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oWeekEndNest.LoadFile("table/WeekEndNestActivity.txt");

	return true;
}

void SpActivityWeekEndNest::ClearFile()
{
	m_oWeekEndNest.Clear();
}

void SpActivityWeekEndNest::OnStart()
{
	std::vector<SpActivityTaskConf*> allTask;
	GetAllTask(allTask);
	StartOnlinePlayer(allTask);
}

KKSG::ErrorCode SpActivityWeekEndNest::GetReward(Role* pRole, UINT32 taskid)
{
	const SpActivityTaskConf* taskConf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (taskConf == NULL)
	{
		LogWarn("can't find SpActivityTaskConf, taskid:%u", taskid);
		return KKSG::ERR_UNKNOWN;
	}
	return pRole->Get<SpActivityRecord>()->GetReward(m_actid, taskid, taskConf->items, ItemFlow_SpActivity_WeekNest_GetReward);
}

void SpActivityWeekEndNest::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;
	///> 未开启
	if (m_stage == 0)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	bool isOpenNow = false;
	///> startTime or stage与当前不一样 重置任务
	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}
	if (pDataBase != NULL)
	{
		if (pDataBase->GetStartTime() != m_startTime)
		{
			isOpenNow = true;
			pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		}
		else if (pDataBase->GetActStage() != m_stage)
		{
			pDataBase->ResetTask();
		}
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	///> 未开启活动 or 登录 才会更新任务
	if (isOpenNow || pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		std::vector<SpActivityTaskConf*> allTask;
		GetAllTask(allTask);
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &allTask);
	}	

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

void SpActivityWeekEndNest::GetAllTask(std::vector<SpActivityTaskConf*>& allTask)
{
	UINT32 openServerTime = (UINT32)GSConfig::Instance()->GetGameServerOpenTimeStamp();
	UINT32 date = m_startTime > openServerTime ? ((m_startTime - openServerTime) / (24*3600)) : 0;
	for (auto i = m_oWeekEndNest.Table.begin(); i != m_oWeekEndNest.Table.end(); ++i)
	{
		if (date >= (*i)->OpenSvrDay[0] && date <= (*i)->OpenSvrDay[1])
		{
			SpActivityTaskConf* conf = SpActivityConfig::Instance()->GetTaskConf((*i)->ParentTaskId);
			if (conf != NULL)
			{
				allTask.push_back(conf);
				for (auto j = conf->taskson.begin(); j != conf->taskson.end(); ++j)
				{
					SpActivityTaskConf* tmp = SpActivityConfig::Instance()->GetTaskConf(*j);
					if (tmp != NULL)
					{
						allTask.push_back(tmp);
					}
				}
			}
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct AwardItem
{
	UINT32 prop;
	UINT32 confprop;
	UINT32 itemID;
	UINT32 itemCount;

	AwardItem()
	{
		prop = 0;
		confprop = 0;
		itemID = 0;
		itemCount = 0;
	}
};

struct AwardInfo
{
	UINT32 end;
	std::vector<AwardItem> items;

	AwardInfo()
	{
		end = 0;
	}
};

SpActivityLuckyTurntable::SpActivityLuckyTurntable(UINT32 actid):SpActivityBase(actid)
{
	assert(actid == SpActivity_LuckyTurntable);
}

SpActivityLuckyTurntable::~SpActivityLuckyTurntable()
{
}

void SpActivityLuckyTurntable::GetItemRecord(std::vector<KKSG::ItemRecord>& all)
{
	for (auto it = GetGlobalConfig().LotteryDrawItems.begin(); it != GetGlobalConfig().LotteryDrawItems.end(); it++)
	{
		KKSG::ItemRecord itemrecord;
		itemrecord.set_itemid(it->seq[0]);
		itemrecord.set_itemcount(it->seq[1]);
		itemrecord.set_isreceive(false);
		all.push_back(itemrecord);
	}
}

void SpActivityLuckyTurntable::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL)
		return;
	///> 未开启
	if (m_stage == 0)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	bool isOpenNow = false;
	///> startTime or stage与当前不一样 重置任务
	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}
	if (pDataBase != NULL)
	{
		if (pDataBase->GetStartTime() != m_startTime)
		{
			isOpenNow = true;
			pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		}
		else if (pDataBase->GetActStage() != m_stage)
		{
			pDataBase->ResetTask();
		}
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	bool Config_IsModify = pRole->Get<RoleMisc>()->IsConfigModify();
	///> 未开启活动 or 登录 才会更新任务
	if (pDataBase == NULL || pRole->GetFlag(RSF_ISWaitLogin) || updateTask || Config_IsModify)
	{
		std::vector<KKSG::ItemRecord> all;
		GetItemRecord(all);
		pRole->Get<RoleMisc>()->InitLuckyActivity(all);
		std::vector<SpActivityTaskConf*> allTask;
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &allTask);
	}

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

void SpActivityLuckyTurntable::FillConsume(KKSG::GetLuckyActivityInfoRes& res, UINT32 state)
{
	res.set_errorcode(KKSG::ERR_SUCCESS);
	auto it = GetGlobalConfig().LotteryDrawConsume.find(state);
	if (it != GetGlobalConfig().LotteryDrawConsume.end())
	{
		res.set_currencytype(it->second.seq[1]);
		res.set_price(it->second.seq[0]);
	}
	else
	{
		res.set_currencytype(0);
		res.set_price(0);
	}
}

KKSG::ErrorCode SpActivityLuckyTurntable::GetPriceAndConsume(UINT32 state, UINT32 &price, UINT32 &consumeitemid)
{
	auto it = GetGlobalConfig().LotteryDrawConsume.find(state);
	if (it == GetGlobalConfig().LotteryDrawConsume.end())
	{
		return KKSG::ERR_BUY_LIMIT;
	}
	price = it->second.seq[0];
	consumeitemid = it->second.seq[1];
	return KKSG::ERR_SUCCESS;
}

void SpActivityLuckyTurntable::LotterDraw(KKSG::LotteryDrawRes& res, Role *pRole)
{
	if (pRole->Get<RoleMisc>()->GetIsPay() == false)
	{
		res.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	std::vector<KKSG::ItemRecord> itemList;
	pRole->Get<RoleMisc>()->GetRoleItemRecord(itemList);
	AwardInfo info;
	for (int i = 0;i < itemList.size();i++)
	{
		AwardItem item;
		if (!itemList[i].isreceive())
		{
			item.confprop = GetGlobalConfig().LotteryDrawItems[i].seq[2];
		}
		info.end += item.confprop;
		item.prop = info.end;
		item.itemID = GetGlobalConfig().LotteryDrawItems[i].seq[0];
		item.itemCount = GetGlobalConfig().LotteryDrawItems[i].seq[1];
		info.items.push_back(item);
	}
	UINT32 random = XRandom::randInt(1, info.end + 1);
	for (UINT32 i = 0;i < info.items.size();i++)
	{
		if (random <= info.items[i].prop)
		{
			if (!itemList[i].isreceive())
			{
				res.set_errorcode(KKSG::ERR_SUCCESS);
				res.set_index(i);
				pRole->Get<RoleMisc>()->UpdateAfterLottery(i);
				KKSG::LuckyActivity turntable = pRole->Get<RoleMisc>()->GetLuckyActicityInfo();
				auto it = GetGlobalConfig().LotteryDrawConsume.find(turntable.state());
				if (it != GetGlobalConfig().LotteryDrawConsume.end())
				{
					res.set_price(it->second.seq[0]);
					res.set_currencytype(it->second.seq[1]);
				}
				else
				{
					res.set_price(0);
					res.set_currencytype(0);
				}
				res.mutable_itemrecord()->CopyFrom(turntable.itemrecord());
				BagGiveItemTransition stTransition(pRole);
				stTransition.SetReason(ItemFlow_Lottory, ItemFlow_Lottory_Draw, false);
				stTransition.GiveItem(info.items[i].itemID, info.items[i].itemCount);
				stTransition.NotifyClient();
				return;
			}
		}
	}
	res.set_errorcode(KKSG::ERR_BUY_LIMIT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////