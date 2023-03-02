#include "pch.h"
#include "spactivitymgr.h"
#include "config.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "gamelogic/levelsealMgr.h"
#include "unit/rolemanager.h"
#include "spactivityrecord.h"
#include "unit/role.h"
#include "role/ptcg2c_opensystemntf.h"
#include "table/OpenSystemMgr.h"
#include "spactivity/rpcg2m_syncactivityend2ms.h"
#include "network/mslink.h"
#include "spactivitybase.h"
#include "spactivitydef.h"
#include "spactivitysmall.h"
#include "unit/systeminfo.h"

INSTANCE_SINGLETON(SpActivityMgr)


#define OneWeekSecond (7 * 24 * 3600)


SpActivityBase* GetNewSpActivityBase(UINT32 actid)
{
	switch (actid)
	{
	case SpActivity_OpenServer:
		{
			return new SpActivityOpenServer(actid);
		}
	case SpActivity_PartnerDay:
		{
			return new SpActivityPartnerDay(actid);
		}
	case SpActivity_BackFlow:
		{
			return new SpActivityBackFlow(actid);
		}
	case SpActivity_Argenta:
		{
			return new SpActivityArgenta(actid);
		}
	case SpActivity_AncientTimes:
		{
			return new SpActivityAncient(actid);
		}
	case SpActivity_BioHell:
	case SpActivity_Duck:
		{
			return new SpActivityTheme(actid);
		}
	case SpActivity_WeekEndNest:
		{
			return new SpActivityWeekEndNest(actid);
		}
	case SpActivity_LuckyTurntable:
		{
			return new SpActivityLuckyTurntable(actid);
		}
	default:
		{
			return new SpActivityBase(actid);
		}
	}
}

SpActivityMgr::SpActivityMgr()
{
	m_getServerOpenTimeCnt = 0;
	m_timerHandler = INVALID_HTIMER;
}

SpActivityMgr::~SpActivityMgr()
{
	for (auto i = m_allSpActivity.begin(); i != m_allSpActivity.end(); ++i)
	{
		delete i->second;
	}
}

void SpActivityMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	RpcG2M_SyncActivityEnd2MS* rpc = RpcG2M_SyncActivityEnd2MS::CreateRpc();
	for (auto iter = m_allSpActivity.begin(); iter != m_allSpActivity.end(); ++iter)
	{
		const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(iter->first);
		if (!conf)
		{
			continue;
		}
		KKSG::SpActivityData2MS* data = rpc->m_oArg.add_spactivitydata();
		data->set_actid(iter->first);
		data->set_starttime(iter->second->GetStartTime());
		data->set_actstage(iter->second->GetStage());

		time_t tempTime = iter->second->GetStartTime();
		for (UINT32 i = 0; i < conf->timestage.size(); ++i)
		{
			tempTime += conf->timestage[i][0] * 3600 + conf->timestage[i][1] *60;
		}

		data->set_endtime(tempTime);
		data->set_minlevel(conf->minlevel);
	}
	MSLink::Instance()->SendTo(*rpc);
}

bool SpActivityMgr::Init()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return true; //非主城服务器不允许运行这些逻辑
	}

	const SuperActivityTime& timeTable = SpActivityConfig::Instance()->GetSpAcitivityTimeTable();
	time_t nowTime = GameTime::GetTime();
	for (auto i = timeTable.Table.begin(); i != timeTable.Table.end(); ++i)
	{
		if (m_allSpActivity.find((*i)->actid) != m_allSpActivity.end())
		{
			LogError("actid [%u] in SuperActivityTime.txt is duplicated key");
			return false;
		}
		SpActivityBase* pSpActBase = GetNewSpActivityBase((*i)->actid);
		m_allSpActivity[(*i)->actid] = pSpActBase;

		if (!pSpActBase->Init())
		{
			LogWarn("actid [%u] SpActivityBase init failed", (*i)->actid);
			return false;
		}

		if ((*i)->datetype == SpActDateType_OpenServerDay)
		{
			continue; //等ms发同步时间协议过来	
		}
		else if ((*i)->datetype == SpActDateType_DefiniteDate)
		{
			time_t startTime = _GetStartTime(*i, nowTime);

			_InitSpActivity(nowTime, startTime, *i);
		}
		else if ((*i)->datetype == SpActDateType_EveryWeek)
		{
			time_t startTime = _GetStartTime(*i, nowTime);

			_InitSpActivity(nowTime, startTime, *i);
		}
	}

	TimeSpecific spec(-1, 0, 1);
	CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::onTimer, this));

	m_timerHandler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);

	return true;
}

void SpActivityMgr::Uninit()
{
	if (m_timerHandler)
	{
		CTimerMgr::Instance()->KillTimer(m_timerHandler);
		m_timerHandler = INVALID_HTIMER;
	}
}

bool SpActivityMgr::onTimer()
{
	time_t nowTime = GameTime::GetTime();
	PtcG2C_SpActivityOffsetDayNtf oPtc;
	for (auto i = m_allSpActivity.begin(); i != m_allSpActivity.end(); ++i)
	{		
		if (i->second->m_stage != 0) //开启
		{
			int offsetDay = (int)XCommon::GetDiffDayCount(nowTime, i->second->m_startTime);
			oPtc.m_Data.add_actid(i->first);
			oPtc.m_Data.add_offsetday(offsetDay);
			if (nowTime < i->second->m_startTime) oPtc.m_Data.add_offsettime(0);
			else oPtc.m_Data.add_offsettime((UINT32)nowTime - i->second->m_startTime);
		}
	}
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		pRole->Send(oPtc);
	}
	return true;
}

bool SpActivityMgr::LoadFile(UINT32 args)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return true;
	}
	auto iter = m_allSpActivity.find(args);
	if (iter == m_allSpActivity.end())
	{
		return true;
	}
	LogInfo("SpActivity, LoadFile, args [%u]", args);
	return iter->second->LoadFile();
}

bool SpActivityMgr::OnReloadTimeAndTaskTable()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return true;
	}

	bool ret = true;
	const SuperActivityTime& timeTable = SpActivityConfig::Instance()->GetSpAcitivityTimeTable();
	time_t nowTime = GameTime::GetTime();
	time_t serverOpenTime = GSConfig::Instance()->GetGameServerOpenTimeStamp();
	for (auto i = timeTable.Table.begin(); i != timeTable.Table.end(); ++i)
	{
		SpActivityBase* pSpActBase = GetSpActivity((*i)->actid);
		if (pSpActBase == NULL)
		{
			pSpActBase = GetNewSpActivityBase((*i)->actid);	

			if (!pSpActBase->Init())
			{
				LogError("actid [%u] SpActivityBase init failed", (*i)->actid);
				delete pSpActBase;
				ret = false;
				continue;
			}
			
			m_allSpActivity[(*i)->actid] = pSpActBase;
		}
		else if (pSpActBase->GetStage() == 0)
		{
			CTimeSpecificMgr::Instance()->UnRegisterEvent(pSpActBase->m_handler);
		}
		else
		{
			continue;
		}

		if ((*i)->datetype == SpActDateType_OpenServerDay
			|| (*i)->datetype == SpActDateType_DefiniteDate
			|| (*i)->datetype == SpActDateType_EveryWeek)
		{
			time_t startTime = _GetStartTime(*i, nowTime, serverOpenTime);

			_InitSpActivity(nowTime, startTime, *i);
		}
	}

	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		UpdateSpActivityData(*i, true);
	}

	return ret;
}

void SpActivityMgr::OnGetGameServerOpenTime()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}

	if (m_getServerOpenTimeCnt != 0)
	{
		return;
	}
	m_getServerOpenTimeCnt++;

	const SuperActivityTime& timeTable = SpActivityConfig::Instance()->GetSpAcitivityTimeTable();
	time_t nowTime = GameTime::GetTime();
	time_t serverOpenTime = GSConfig::Instance()->GetGameServerOpenTimeStamp();
	for (auto i = timeTable.Table.begin(); i != timeTable.Table.end(); ++i)
	{
		if ((*i)->datetype == SpActDateType_OpenServerDay) 
		{
			time_t startTime = _GetStartTime(*i, nowTime, serverOpenTime);

			_InitSpActivity(nowTime, startTime, *i);
		}
	}

	// CombineServer Acitivity
	time_t combineServerTime = GSConfig::Instance()->GetCombineServerTime();
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(SpActivity_CombineServer);
	if (conf != NULL)
	{
		time_t startTime = XCommon::GetDayBeginTime(combineServerTime, false) + conf->starthour * 3600;
		_InitSpActivity(nowTime, startTime, conf);
	}
}

void SpActivityMgr::SendActivityOffsetDayPtc(Role* pRole)
{
	if (!GSConfig::Instance()->IsHallGS() || pRole == NULL)
	{
		return;
	}
	time_t nowTime = GameTime::GetTime();
	PtcG2C_SpActivityOffsetDayNtf oPtc;
	for (auto i = m_allSpActivity.begin(); i != m_allSpActivity.end(); ++i)
	{
		if (i->second->m_stage != 0) //开启
		{
			int offsetDay = (int)XCommon::GetDiffDayCount(nowTime, i->second->m_startTime);
			oPtc.m_Data.add_actid(i->first);
			oPtc.m_Data.add_offsetday(offsetDay);
			if (nowTime < i->second->m_startTime) oPtc.m_Data.add_offsettime(0);
			else oPtc.m_Data.add_offsettime((UINT32)nowTime - i->second->m_startTime);
		}
	}
	pRole->Send(oPtc);
}

void SpActivityMgr::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (!GSConfig::Instance()->IsHallGS() || pRole == NULL)
	{
		return;
	}

	for (auto i = m_allSpActivity.begin(); i != m_allSpActivity.end(); ++i)
	{
		i->second->UpdateSpActivityData(pRole, updateTask);
	}

	CloseNotOpenSpActSystem(pRole);
}

void SpActivityMgr::CloseNotOpenSpActSystem(Role* pRole)
{
	if (!GSConfig::Instance()->IsHallGS() || pRole == NULL)
	{
		return;
	}
	///> actid是保证唯一的，但是可能对应的systemid是重复的
	std::set<UINT32> openSystem;
	for (auto i = m_allSpActivity.begin(); i != m_allSpActivity.end(); ++i)
	{
		const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(i->first);
		if (conf == NULL)
		{
			continue;
		}
		if (i->second->CheckOpenCondition(pRole))
		{
			openSystem.insert(conf->systemid);
		}
	}

	std::vector<UINT32> closeids;

	const SuperActivityTime& timeTable = SpActivityConfig::Instance()->GetSpAcitivityTimeTable();
	for (auto i = timeTable.Table.begin(); i != timeTable.Table.end(); ++i)
	{
		///> 系统应该没开，但是人物身上系统开了
		if (openSystem.find((*i)->systemid) == openSystem.end() && pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole((*i)->systemid)) 
		{
			if (!pRole->GetFlag(RSF_ISWaitLogin))
			{
				closeids.push_back((*i)->systemid);
				pRole->Get<CRoleSystem>()->SetSystemOpened((*i)->systemid, false);
			}
		}
	}

	OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &closeids);
}

void SpActivityMgr::_InitSpActivity(time_t nowTime, time_t startTime, const SpActivityTimeConf* conf)
{
	SpActivityBase* pSpActBase = GetSpActivity(conf->actid);
	if (pSpActBase == NULL)
	{
		LogError("actid [%u] SpActivityBase is NULL, code error", conf->actid);
		return;
	}
	if (nowTime <= startTime)
	{
		pSpActBase->m_startTime = startTime;
		pSpActBase->m_stage = 0;

		TimeSpecific spec(startTime);
		pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnStart, this, conf->actid));
	
		return;
	}

	time_t tempTime = startTime;
	for (UINT32 i = 0; i < conf->timestage.size(); ++i)
	{
		tempTime += conf->timestage[i][0] * 3600 + conf->timestage[i][1] *60;
		if (nowTime <= tempTime)
		{
			pSpActBase->m_startTime = startTime;
			pSpActBase->m_stage = i + 1;

			TimeSpecific spec(tempTime);
			if (pSpActBase->m_stage == conf->timestage.size())
			{
				pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnEnd, this, conf->actid));
			}
			else
			{
				pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnNextStage, this, conf->actid));
			}

			return;
		}
	}

	if (conf->datetype == SpActDateType_EveryWeek)
	{
		tempTime = startTime + OneWeekSecond;	
		pSpActBase->m_startTime = tempTime;
		pSpActBase->m_stage = 0;

		TimeSpecific spec(tempTime);
		pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnStart, this, conf->actid));

		return;
	}
}

void SpActivityMgr::_RegisterNextTimer(SpActivityBase* pSpActBase, const SpActivityTimeConf* conf)
{
	if (!IsValidDateType(conf->datetype))
	{
		return;
	}
	if (pSpActBase->m_stage > conf->timestage.size())
	{
		LogError("actid [%u], it shouldn't happen, stage > timestage's size, change table ?", conf->actid);
		_ResetSpActivity(pSpActBase);
		return;
	}
	time_t tempTime = pSpActBase->m_startTime;
	for (UINT32 i = 0; i < pSpActBase->m_stage; ++i)
	{
		tempTime += conf->timestage[i][0] * 3600 + conf->timestage[i][1] * 60;
	}
	if (pSpActBase->m_stage == conf->timestage.size())
	{
		TimeSpecific spec(tempTime - 1); // important! ! ! because of the time sequence problem
		pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnEnd, this, conf->actid));
	}
	else
	{
		TimeSpecific spec(tempTime);
		pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnNextStage, this, conf->actid));
	}
}

bool SpActivityMgr::OnStart(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return false;
	}

	auto iter = m_allSpActivity.find(actid);
	if (iter == m_allSpActivity.end())
	{
		return false;
	}

	SpActivityBase* pSpActBase = iter->second;
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(actid);
	if (conf == NULL)
	{
		LogError("actid [%u] can't find in SuperActivityTime.txt", actid);
		_ResetSpActivity(pSpActBase);
		return false;
	}
	
	if (pSpActBase->m_stage != 0)
	{
		LogError("actid [%u], stage [%u] should be 0(NotOpen)", actid, pSpActBase->m_stage);
		_ResetSpActivity(pSpActBase);
		return false;
	}

	pSpActBase->m_stage = 1;

	pSpActBase->OnStart();

	_RegisterNextTimer(pSpActBase, conf);

	LogInfo("actid [%u] OnStart", actid);

	return true;
}

bool SpActivityMgr::OnNextStage(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return false;
	}

	auto iter = m_allSpActivity.find(actid);
	if (iter == m_allSpActivity.end())
	{
		return false;
	}

	SpActivityBase* pSpActBase = iter->second;
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(actid);
	if (conf == NULL)
	{
		LogError("can't find actid:%u in SuperActivityTime.txt", actid);
		_ResetSpActivity(pSpActBase);
		return false;
	}	

	++pSpActBase->m_stage;

	pSpActBase->OnNextStage();

	_RegisterNextTimer(pSpActBase, conf);

	LogInfo("actid [%u] OnNextStage [%u]", actid, pSpActBase->m_stage);

	return true;
}

bool SpActivityMgr::OnEnd(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return false;
	}

	auto iter = m_allSpActivity.find(actid);
	if (iter == m_allSpActivity.end())
	{
		return false;
	}

	SpActivityBase* pSpActBase = iter->second;
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(actid);
	if (conf == NULL)
	{
		LogError("can't find actid:%u in SuperActivityTime.txt", actid);
		_ResetSpActivity(pSpActBase);
		return false;
	}
	
	if (IsValidDateType(conf->datetype) && pSpActBase->m_stage != conf->timestage.size())
	{
		LogError("actid [%u], stage [%u] should equal to timestage size [%u]", actid, pSpActBase->m_stage, conf->timestage.size());
		_ResetSpActivity(pSpActBase);
		return false;
	}

	pSpActBase->OnEnd();

	_SyncEndToMS(actid); //如果需要这个操作，活动状态是需要存数据库的，现在还没做

	if (conf->datetype == SpActDateType_EveryWeek)
	{
		pSpActBase->m_startTime += OneWeekSecond;
		pSpActBase->m_stage = 0;
		TimeSpecific spec(pSpActBase->m_startTime);
		pSpActBase->m_handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::OnStart, this, conf->actid));
	}
	else
	{
		_ResetSpActivity(pSpActBase);
	}

	LogInfo("actid [%u] OnEnd", actid);

	return true;
}

SpActivityBase* SpActivityMgr::GetSpActivity(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return NULL;
	}
	auto iter = m_allSpActivity.find(actid);
	if (iter == m_allSpActivity.end())
	{
		return NULL;
	}
	return iter->second;
}

bool SpActivityMgr::IsValidDateType(UINT32 type)
{
	return type == SpActDateType_OpenServerDay 
		|| type == SpActDateType_DefiniteDate
		|| type == SpActDateType_EveryWeek
		|| type == SpActDateType_Other;
}

void SpActivityMgr::_ResetSpActivity(SpActivityBase* pSpActBase)
{
	pSpActBase->Reset();
}

time_t SpActivityMgr::_GetStartTime(const SpActivityTimeConf* conf, time_t nowTime, time_t serverOpenTime)
{
	if (conf->datetype == SpActDateType_OpenServerDay)
	{
		return serverOpenTime - HOUR_ONE_DAY_BEGIN * 3600 + conf->starttime * 24 * 3600 + conf->starthour * 3600;
	}
	else if (conf->datetype == SpActDateType_DefiniteDate)
	{
		tm stTime;
		stTime.tm_year = conf->starttime / 10000 - 1900;
		stTime.tm_mon = conf->starttime / 100 % 100 - 1;
		stTime.tm_mday = conf->starttime % 100;
		if (conf->starthour < 0 || conf->starthour >= 24)
		{
			LogWarn("actid [%u], starthour [%u] is invalid", conf->actid, conf->starthour);
			return 0;
		}
		stTime.tm_hour = conf->starthour;
		stTime.tm_min = 0;
		stTime.tm_sec = 0;

		return mktime(&stTime);
	}
	else if (conf->datetype == SpActDateType_EveryWeek)
	{
		if (conf->starttime < 1 || conf->starttime > 7)
		{
			LogWarn("actid [%u], datetype is 3, starttime [%u] is invalid", conf->actid, conf->starttime);
			return 0;
		}
		tm* pTime = localtime(&nowTime);
		int wday = pTime->tm_wday == 0 ? 7 : pTime->tm_wday;
		pTime->tm_hour = conf->starthour;
		pTime->tm_min = 0;
		pTime->tm_sec = 0;
		time_t startTime = mktime(pTime);
		startTime += ((int)conf->starttime - wday) * 24 * 3600;

		return startTime;
	}
	return 0;
}

void SpActivityMgr::_SyncEndToMS(UINT32 actid)
{
	if (GSConfig::Instance()->IsHallGS())
	{
		RpcG2M_SyncActivityEnd2MS* rpc = RpcG2M_SyncActivityEnd2MS::CreateRpc();
		rpc->m_oArg.set_actid(actid);
		MSLink::Instance()->SendTo(*rpc);
	}
}

bool SpActivityMgr::GMStart(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return false;
	}
	auto iter = m_allSpActivity.find(actid);
	if (iter == m_allSpActivity.end())
	{
		return false;
	}
	if (iter->second->m_stage != 0)
	{
		return false;
	}
	CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second->m_handler);
	OnEnd(actid);

	iter->second->Reset();
	iter->second->m_startTime = GameTime::GetTime();

	return OnStart(actid);
}

bool SpActivityMgr::GMEnd(UINT32 actid)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return false;
	}
	auto iter = m_allSpActivity.find(actid);
	if (iter != m_allSpActivity.end() && iter->second->m_stage != 0)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second->m_handler);
		OnEnd(actid);
		return true;
	}
	return false;
}

/*bool SpActivityMgr::GMAddOneDay(UINT32 actid)
{
	auto iter = m_spActivity.find(actid);
	if (iter != m_spActivity.end() && iter->second.state != SpAct_NotOpen)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second.handler);
		const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(actid);

		iter->second.startTime -= 24*3600; //add one day
		if (iter->second.state == SpAct_Open)
		{
			time_t startPrizeTime = iter->second.startTime + conf->duration * 24 * 3600 + conf->durationhour * 3600;
			TimeSpecific spec(startPrizeTime - 1);
			if (0 == conf->rewardtime) //no prize time
			{
				iter->second.handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::_OnEnd, this, actid));
			}
			else
			{
				iter->second.handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::_OnStartPrizeTime, this, actid));
			}
		}
		else if (iter->second.state == SpAct_OpenStageOne)
		{
			time_t endTime = iter->second.startTime + (conf->duration + conf->rewardtime) * 24 * 3600 + conf->durationhour * 3600;
			TimeSpecific spec(endTime - 1);
			iter->second.handler = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&SpActivityMgr::_OnEnd, this, actid));
		}
		OnTimer();
		return true;
	}
	return false;
}*/