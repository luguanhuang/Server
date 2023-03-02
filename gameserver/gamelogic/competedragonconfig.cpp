#include "pch.h"
#include "competedragonconfig.h"
#include "foreach.h"
#include "timeutil.h"
#include "scene/sceneconfig.h"
#include "network/mslink.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "define/globalconfigtodbdef.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "gamelogic/levelsealMgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/competedragonrecord.h"

INSTANCE_SINGLETON(CompeteDragonConfig)

CompeteDragonConfig::CompeteDragonConfig()
{
	TimeSpecific time(-1, -1, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CompeteDragonConfig::OnTimer, this));
}

CompeteDragonConfig::~CompeteDragonConfig()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

bool CompeteDragonConfig::OnTimer()
{
	int now = GameTime::GetTime();
	if (!m_globalCompeteDragonInfo.syncTime)
	{
		return false;
	}
	if (!XCommon::IsDaySame(CLevelSealMgr::Instance()->GetUpdateTime(), now))
	{
		return false;
	}

	if (now - m_globalCompeteDragonInfo.updateTime >= 14 * 86400)
	{
		UpdateGlobalInfo(now);
	}

	return true;
}

bool CompeteDragonConfig::Init()
{
	return LoadFile();
}

void CompeteDragonConfig::Uninit()
{

}

bool CompeteDragonConfig::LoadFile()
{
	m_competeDragonTable.Clear();
	m_competeDragonRankTable.Clear();

	if (!m_competeDragonTable.LoadFile("table/CompeteDragon.txt"))
	{
		LogWarn("Load CompeteDragon.txt failed!");
		return false;
	}

	if (!m_competeDragonRankTable.LoadFile("table/CompeteDragonRankReward.txt"))
	{
		LogWarn("Load CompeteDragonRankReward.txt failed!");
		return false;
	}

	return true;
}

CompeteDragonTable::RowData* CompeteDragonConfig::GetCompeteDragonInfo()
{
	auto data = m_competeDragonTable.GetByLevelSealType(m_globalCompeteDragonInfo.levelSealType);
	return data;
}

UINT32 CompeteDragonConfig::GetExpID()
{
	return m_globalCompeteDragonInfo.curExpID;
}

void CompeteDragonConfig::FillGlobalInfo(const std::string& value)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}

	KKSG::GlobalCompeteDragonInfo info;
	info.ParseFromString(value);

	m_globalCompeteDragonInfo.syncTime = GameTime::GetTime();
	m_globalCompeteDragonInfo.updateTime = info.updatetime();
	m_globalCompeteDragonInfo.curExpID = info.curexpid();
	m_globalCompeteDragonInfo.levelSealType = info.curlevelsealtype();

	LogInfo("competedragon info, %u, levelsealtype:%u, dnexpid:%u", m_globalCompeteDragonInfo.updateTime, m_globalCompeteDragonInfo.levelSealType, m_globalCompeteDragonInfo.curExpID);
}

void CompeteDragonConfig::UpdateGlobalInfo(UINT32 time)
{
	m_globalCompeteDragonInfo.updateTime = XCommon::GetWeekBeginTime(time);
	UINT32 type = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	auto data = m_competeDragonTable.GetByLevelSealType(type);
	if (data && !data->ExpIDs.empty())
	{
		int result = XRandom::randInt(0, data->ExpIDs.size());
		m_globalCompeteDragonInfo.curExpID = data->ExpIDs[result];
	}

	m_globalCompeteDragonInfo.levelSealType = type;

	PtcG2M_UpdateGlobalValue ptc;
	ptc.m_Data.set_id(eCompeteDragonInfo);

	KKSG::GlobalCompeteDragonInfo* info = ptc.m_Data.mutable_competedragoninfo();
	info->set_curexpid(m_globalCompeteDragonInfo.curExpID);
	info->set_updatetime(m_globalCompeteDragonInfo.updateTime);
	info->set_curlevelsealtype(m_globalCompeteDragonInfo.levelSealType);

	MSLink::Instance()->SendTo(ptc);

	for (auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); ++iter)
	{
		(*iter)->Get<CCompeteDragonRecord>()->Update();
	}

	LogInfo("competedragon reset, %u, levelsealtype:%u, dnexpid:%u", m_globalCompeteDragonInfo.updateTime, m_globalCompeteDragonInfo.levelSealType, m_globalCompeteDragonInfo.curExpID);
}

