#include "pch.h"
#include "guildterritorybattle.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "guild/guildarena.h"
#include "../share/util/XCommon.h"
#include "table/globalconfig.h"
#include "../share/util/gametime.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "guild/ptcm2c_noticeguildterrwar.h"
#include "guild/ptcm2c_noticeguildterrall.h"
#include "notice/noticemgr.h"
#include "guildcastle/guildcastlemgr.h"
#include "guildauct/guildauctmgr.h"
#include "time.h"
#include <time.h>
#include "guild/ptcm2c_noticeguildterrend.h"
#include "guild/ptcm2c_noticeguildterrbattlewin.h"
#include "guild/ptcm2c_noticeguildterrbigicon.h"
#include "event/eventmgr.h"
#include "foreach.h"


////////////////////////////////////////////////////

INSTANCE_SINGLETON(CGuildTerritoryBattle)

bool TerritoryGuild::operator<(const TerritoryGuild &x) const
{
	Guild* pX = CGuildMgr::Instance()->GetGuild(m_guildId);
	Guild* pY = CGuildMgr::Instance()->GetGuild(x.m_guildId);
	if (pX != NULL && pY == NULL)
	{
		return false;
	}
	else if (pX == NULL)
	{
		return true;
	}

	if (pX->GetPrestige() < pY->GetPrestige())
	{
		return false;
	}
	if (pX->GetPrestige() > pY->GetPrestige())
	{
		return true;
	}

	if (pX->GetLevel() < pY->GetLevel())
	{
		return false;
	}
	if (pX->GetLevel() > pY->GetLevel())
	{
		return true;
	}

	if (pX->GetID() > pY->GetID())
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

/////////////////////////////////////////////////

CGuildTerritoryBattle::CGuildTerritoryBattle()
{
	m_battleBeginTime = 0;
	m_aucBeginTime = 0;
	m_declareEndTime = 0;
	m_warEndTime = 0;
	m_rewardStartTime = 0;
}

CGuildTerritoryBattle::~CGuildTerritoryBattle()
{
}


bool CGuildTerritoryBattle::Init()
{
	if (!m_territoryTable.LoadFile("table/territorybattle.txt"))
	{
		SSWarn<<"load file table/territorybattle.txt failed"<<END;
		return false;
	}

	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	Load();
	return true;
}

void CGuildTerritoryBattle::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

bool CGuildTerritoryBattle::Load()
{
	std::string strData = CGlobalConfigDBBuild::Instance()->GetConfigValue(eGuildTerritory);
	if (strData.empty())
	{
		return true;
	}

	KKSG::GuildTerritoryData data;
	if (!data.ParseFromString(strData))
	{
		SSWarn << "Load CGuildTerritoryBattle Fail !" << END;
		return true;
	}

	m_battleBeginTime = data.battlebegintime();
	m_aucBeginTime = data.aucbegintime();
	m_declareEndTime = data.declareendtime();
	m_warEndTime = data.warendtime();
	m_rewardStartTime = data.rewardstarttime();

	for (UINT32 i = 0; i < data.citydata_size(); ++i)
	{
		CityUnit& cityUnit = m_cityMap[data.citydata(i).id()];
		cityUnit.m_guildId = data.citydata(i).guildid();
		cityUnit.m_time = data.citydata(i).time();

		Guild* pGuild = CGuildMgr::Instance()->GetGuild(cityUnit.m_guildId);
		if (pGuild == NULL)
		{
			continue;
		}
		TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(data.citydata(i).id());
		if (pRowData == NULL)
		{
			continue;
		}
		// 如果本次领地等级高于之前等级，则更新领地等级
		if (pRowData->territorylevel > pGuild->GetMaxTerritoryLevel())
		{
			pGuild->SetMaxTerritoryLevel(pRowData->territorylevel);
		}
	}

	for (UINT32 j = 0; j < data.challlist_size(); ++j)
	{
		const KKSG::GuildTerritory& guildTerr = data.challlist(j);
		std::set<TerritoryGuild>& terrSet = m_challList[guildTerr.id()];
		for (UINT32 k = 0; k < guildTerr.territorydata().territoryunit_size(); ++k)
		{
			TerritoryGuild terrGuild;

			terrGuild.m_guildId = guildTerr.territorydata().territoryunit(k).guildid();
			terrGuild.m_allianceId = guildTerr.territorydata().territoryunit(k).allianceid();

			foreach (y in guildTerr.territorydata().territoryunit(k).reqalliacne())
			{
				terrGuild.m_reqAllianceSet[y->guildid()] = y->time();
			}
			terrSet.insert(terrGuild);
		}
	}
	return true;
}

void CGuildTerritoryBattle::Update()
{
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eGuildTerritory, ToBinary());
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eGuildTerritory);
}

std::string CGuildTerritoryBattle::ToBinary()
{
	KKSG::GuildTerritoryData data;

	data.set_battlebegintime(m_battleBeginTime);
	data.set_aucbegintime(m_aucBeginTime);
	data.set_declareendtime(m_declareEndTime);
	data.set_warendtime(m_warEndTime);
	data.set_rewardstarttime(m_rewardStartTime);

	foreach (i in m_cityMap)
	{
		KKSG::CityData* pCityData = data.add_citydata();
		pCityData->set_id(i->first);
		pCityData->set_guildid(i->second.m_guildId);
		pCityData->set_time(i->second.m_time);
	}

	foreach (j in m_challList)
	{
		KKSG::GuildTerritory* pGuildTerr = data.add_challlist();
		pGuildTerr->set_id(j->first);
		KKSG::TerritoryData* terrData = pGuildTerr->mutable_territorydata();;
		foreach (k in j->second)
		{
			KKSG::TerritoryUnit* pTerrUnit = terrData->add_territoryunit();
			pTerrUnit->set_guildid(k->m_guildId);
			pTerrUnit->set_allianceid(k->m_allianceId);

			foreach (y in k->m_reqAllianceSet)
			{
				KKSG::ReqAllianceData* pAlliData = pTerrUnit->add_reqalliacne(); 
				pAlliData->set_guildid(y->first);
				pAlliData->set_time(y->second);
			}
		}
	}

	return data.SerializeAsString();
}

bool CGuildTerritoryBattle::CanDeclareWar(UINT64 roleId, UINT32 id)
{
	if (!IsDeclareTime())
	{
		return false;
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return false;
	}

	if (!pGuild->isLeader(roleId) && !pGuild->isViceLeader(roleId))
	{
		return false;
	}

	if (_IsDeclare(id, pGuild->GetID())) // 已经对该领地宣战过
	{
		return false;
	}

	// 如果宣战的目标不大于已经占领过得，则全部可以
	UINT32 target_level = _GetCityType(id);
	if (target_level <= pGuild->GetMaxTerritoryLevel())
	{
		return true;
	}
	
	UINT32 cityType = _GetCityType(id);
	UINT32 selfType = _GetGuildTerrType(pGuild->GetID());
	if (!(cityType > selfType && (cityType - selfType) == 1))
	{
		return false;
	}

	return true;
}

bool CGuildTerritoryBattle::DeclareWar(UINT64 roleId, UINT32 id)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return false;
	}

	if (!CanDeclareWar(roleId, id)) // 条件检查
	{
		return false;
	}

	_ClearDeclareWar(pGuild->GetID()); // 清理以前的宣战数据

	std::set<TerritoryGuild>& guildSet = m_challList[id];
	TerritoryGuild unit(pGuild->GetID());
	unit.m_roleId = roleId;
	guildSet.insert(unit);
	SetChange();

	//  公告
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(id);
	if (pRowData != NULL)
	{
		if (pRowData->territorylevel == 1)
		{
			NoticeMgr::Instance()->NoticeGuildTerrDeclareOne(pGuild->GetID(), pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 2)
		{
			NoticeMgr::Instance()->NoticeGuildTerrDeclareTwo(pGuild->GetID(), pRowData->territoryname);
		} 
		else if (pRowData->territorylevel == 3)
		{
			NoticeMgr::Instance()->NoticeGuildTerrDeclareThree(pGuild->GetID(), pRowData->territoryname);
		}  
	}

	SSInfo << "DeclareWar guildId : " << pGuild->GetID() << "roleid : " << roleId <<" id : " << id << END;
	return true;
}

void CGuildTerritoryBattle::_ClearDeclareWar(UINT64 guildId)
{
	foreach (i in m_challList) // 清理以前的报名信息
	{
		for (auto iter = i->second.begin(); iter != i->second.end(); )
		{
			if (iter->m_guildId == guildId)
			{
				i->second.erase(iter++);
				SetChange();
			}
			else
			{
				++iter;
			}
		}
	}

	_ClearAllianceList(guildId); // 清理邀请列表
}

bool CGuildTerritoryBattle::_IsDeclare(UINT32 id, UINT64 guildId)
{
	auto iter = m_challList.find(id);
	if (iter != m_challList.end())
	{
		foreach (i in iter->second)
		{
			if (i->m_guildId == guildId)
			{
				return true;
			}
		}
	}
	return false;
}

UINT32 CGuildTerritoryBattle::RcvAlliance(UINT64 roleId, UINT64 guildId)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pGuild->GetID() == guildId) // 不能盟友自己
	{
		SSWarn << "RcvAlliance can not self guild : " << guildId << END; 
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 cityOne = GetDeclareCityId(pGuild->GetID()); 
	UINT32 cityTwo = GetDeclareCityId(guildId); 
	if (m_cityMap.find(cityOne) == m_cityMap.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (cityTwo != cityOne)
	{
		SSWarn << "RcvAlliance cityid different cityOne : " << cityOne << " cityTwo : " << cityTwo << END; 
		return KKSG::ERR_UNKNOWN;
	}

	if (!pGuild->isLeader(pRole->GetID()) && !pGuild->isViceLeader(pRole->GetID()))
	{
		return KKSG::ERR_UNKNOWN;
	}

	const TerritoryGuild* pSelf = GetUnit(pGuild->GetID());
	const TerritoryGuild* pReq = GetUnit(guildId);
	if (pSelf == NULL || pReq == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pSelf->m_reqAllianceSet.find(guildId) == pSelf->m_reqAllianceSet.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pReq->m_allianceId != 0)
	{
		return KKSG::ERR_OTHER_HAS_ALLIANCE;
	}

	if (pSelf->m_allianceId != 0)
	{
		return KKSG::ERR_SELF_HAS_ALLIANCE;
	}

	_SetAllianceId(pGuild->GetID(), guildId);
	_SetAllianceId(guildId, pGuild->GetID());

	UINT32 cityId = GetDeclareCityId(guildId);
	Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(guildId);
	TerritoryBattle::RowData* pRowdata = m_territoryTable.GetByID(cityId);
	if (pRowdata->territorylevel == 1)
	{
		if (pGuildTwo != NULL)
		{
			NoticeMgr::Instance()->NoticeGuildTerrAllianceOne(pGuild->GetID(), pGuildTwo->GetName(), pRowdata->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrAllianceOne(pGuildTwo->GetID(), pGuild->GetName(), pRowdata->territoryname);
		}
	}
	else if (pRowdata->territorylevel == 2)
	{
		if (pGuildTwo != NULL)
		{
			NoticeMgr::Instance()->NoticeGuildTerrAllianceTwo(pGuild->GetID(), pGuildTwo->GetName(), pRowdata->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrAllianceTwo(pGuildTwo->GetID(), pGuild->GetName(), pRowdata->territoryname);
		}
	}
	else if (pRowdata->territorylevel == 3)
	{
		if (pGuildTwo != NULL)
		{
			NoticeMgr::Instance()->NoticeGuildTerrAllianceThree(pGuild->GetID(), pGuildTwo->GetName(), pRowdata->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrAllianceThree(pGuildTwo->GetID(), pGuild->GetName(), pRowdata->territoryname);
		}
	}

	SetChange();
	SSInfo << "guildOne : " << pGuild->GetID() << "RcvAlliance" << " guildTwo : " << pGuildTwo->GetID() << END;
	return KKSG::ERR_SUCCESS;
}

void CGuildTerritoryBattle::_SetAllianceId(UINT64 guilId, UINT64 allianId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guilId)
			{
				TerritoryGuild data = *j;
				data.m_allianceId = allianId;
				i->second.erase(j);
				i->second.insert(data);
				SetChange();
				return ;
			}
		}
	}
}

UINT32 CGuildTerritoryBattle::ReqAlliance(UINT64 roleId, UINT64 guildId, UINT32 id)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (GetDeclareCityId(pGuild->GetID()) != id)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (GetDeclareCityId(guildId) != id)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (m_cityMap.find(id) == m_cityMap.end()) // 占领为空的时候不能邀请盟友
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (!pGuild->isLeader(pRole->GetID()) && !pGuild->isViceLeader(pRole->GetID()))
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pGuild->GetID() == guildId) // 不能盟友自己
	{
		return KKSG::ERR_UNKNOWN;
	}

	const TerritoryGuild* pUnitSelf = GetUnit(pGuild->GetID());
	const TerritoryGuild* pUnitTwo = GetUnit(guildId);
	if (pUnitSelf == NULL || pUnitTwo == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pUnitSelf->m_allianceId != 0)
	{
		return KKSG::ERR_SELF_HAS_ALLIANCE;
	}

	if (pUnitTwo->m_allianceId != 0)
	{
		return KKSG::ERR_OTHER_HAS_ALLIANCE;
	}

	auto iter = m_challList.find(id);
	if (iter != m_challList.end())
	{
		for (auto i = iter->second.begin(); i != iter->second.end(); ++i)
		{
			if (i->m_guildId == guildId)
			{
				TerritoryGuild data1 = *i;
				data1.m_reqAllianceSet[pGuild->GetID()] = GameTime::GetTime();
				iter->second.erase(i);
				iter->second.insert(data1);
				_NoticeAllIcon(guildId);
				SetChange();
				break;
			}
		}
	}
	return KKSG::ERR_SUCCESS;
}

const TerritoryGuild* CGuildTerritoryBattle::GetUnit(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		for (std::set<TerritoryGuild>::iterator iter = i->second.begin(); iter != i->second.end(); ++iter)
		{
			if (iter->m_guildId == guildId)
			{
				return &(*iter);
			}
		}
	}

	return NULL;
}


UINT32 CGuildTerritoryBattle::_GetCityType(UINT32 id)
{
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(id);

	if (pRowData != NULL)
	{
		return pRowData->territorylevel;
	}
	else
	{
		return 0;
	}
}

UINT32 CGuildTerritoryBattle::_GetGuildTerrId(UINT64 guildId)
{
	foreach (i in m_cityMap)
	{
		if (i->second.m_guildId == guildId)
		{
			return i->first;
		}
	}

	return 0;
}

UINT32 CGuildTerritoryBattle::_GetGuildTerrType(UINT64 guildId)
{
	UINT32 id = _GetGuildTerrId(guildId);
	if (id != 0)
	{
		return _GetCityType(id); 
	}

	return 0;
}

void CGuildTerritoryBattle::SetBattleResult(UINT32 id, UINT64 guildId)
{
	SSInfo << "SetBattleResult id : " << id << "guildid : " << guildId << END; 
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(id);
	if (pRowData == NULL /*|| m_tempWaringId.find(id) == m_tempWaringId.end()*/)
	{
		m_tempWaringId.erase(id);
		m_challList.erase(id);
		SetChange();
		return ;
	}
	else
	{
		m_tempWaringId.erase(id);
		m_challList.erase(id);
		SetChange();
	}

	if (!_IsWarTime())
	{
		SSInfo << "SetBattleResult timeout : id " << id << "guildid : " << guildId << END;
		_NoticeWarIcon(guildId, false);
		return ;
	}

	if (pRowData->territorylevel <= _GetGuildTerrType(guildId))
	{
		if (pRowData->territorylevel == _GetGuildTerrType(guildId))
		{
			if (pRowData->territorylevel == 1)
			{
				NoticeMgr::Instance()->NoticeGuildTerrDefGuildOne(guildId, pRowData->territoryname);
			}
			else if (pRowData->territorylevel == 2)
			{
				NoticeMgr::Instance()->NoticeGuildTerrDefGuildTwo(guildId, pRowData->territoryname);
			}
			else if (pRowData->territorylevel == 3)
			{
				NoticeMgr::Instance()->NoticeGuildTerrDefGuildThree(guildId, pRowData->territoryname);
			} 
		}
		if (!_IsGuildWaring(guildId))
		{
			_NoticeWarIcon(guildId, false);
		}
		return ;
	}

	CityUnit& cityUnit = m_cityMap[id];
	if (cityUnit.m_guildId != guildId)
	{
		cityUnit.m_guildId = guildId;
		cityUnit.m_time = GameTime::GetTime();
		SetChange();

		PtcM2C_NoticeGuildTerrBattleWin msg;
		msg.m_Data.set_id(id);
		msg.m_Data.set_guildid(guildId);
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
		if (NULL == pGuild)
		{
			return ;
		}
		// 如果本次领地等级高于之前等级，则更新领地等级
		if (pRowData->territorylevel > pGuild->GetMaxTerritoryLevel())
		{
			pGuild->SetMaxTerritoryLevel(pRowData->territorylevel);
		}

		const std::map<UINT64, GuildMember> allMem = pGuild->GetGuildMember();
		foreach (i in allMem)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
			if (pRole != NULL)
			{
				pRole->Send(msg);
			}
		}

		if (pRowData->territorylevel == 1)
		{
			NoticeMgr::Instance()->NoticeGuildTerrRewardOne(guildId, pRowData->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrBwinOne(guildId, pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 2)
		{
			NoticeMgr::Instance()->NoticeGuildTerrRewardTwo(guildId, pRowData->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrBwinTwo(guildId, pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 3)
		{
			NoticeMgr::Instance()->NoticeGuildTerrRewardThree(guildId, pRowData->territoryname);
			NoticeMgr::Instance()->NoticeGuildTerrBwinThree(guildId, pRowData->territoryname);
		} 		
	}
	else
	{
		if (pRowData->territorylevel == 1)
		{
			NoticeMgr::Instance()->NoticeGuildTerrSavOne(guildId, pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 2)
		{
			NoticeMgr::Instance()->NoticeGuildTerrSavTwo(guildId, pRowData->territoryname);
		}
		else
		{
			NoticeMgr::Instance()->NoticeGuildTerrSavThree(guildId, pRowData->territoryname);
		}
	}

	if (!_IsGuildWaring(guildId))
	{
		_NoticeWarIcon(guildId, false);
	}	
}

bool CGuildTerritoryBattle::IsInWaring(UINT64 guildId)
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return false;
	}

	UINT32 warStartTime = 0;
	UINT32 warEndTime = 0;
	UINT32 weekDay = XCommon::GetWeekNum(nowTime);
	foreach (i in GetGlobalConfig().GuildTerritoryDecTime)
	{
		if (i->seq[0] == weekDay)
		{
			warStartTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0); 
		}
	}

	auto iter = GetGlobalConfig().GuildTerritoryOpenTime.find(weekDay);
	if (iter != GetGlobalConfig().GuildTerritoryOpenTime.end())
	{
		warEndTime = XCommon::MakeTime(nowTime, iter->second.seq[2] / 100, iter->second.seq[2] % 100, 0); 
	}

	if (nowTime < warStartTime || nowTime > warEndTime)
	{
		return false;
	}

	foreach (j in m_cityMap)
	{
		if (j->second.m_guildId == guildId)
		{
			return true;
		}
	}
	
	foreach (k in m_challList)
	{
		foreach (x in k->second)
		{
			if (x->m_guildId == guildId)
			{
				return true;
			}
		}
	}

	return false;
}

bool CGuildTerritoryBattle::_IsGuildWaring(UINT64 guildId)
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return false;
	}

	if (!_IsWarTime())
	{
		return false;
	}

	foreach (i in m_tempWaringId)
	{
		auto iter = m_cityMap.find(*i);
		if (iter != m_cityMap.end() && iter->second.m_guildId == guildId)
		{
			return true;
		}

		auto it = m_challList.find(*i);
		if (it != m_challList.end())
		{
			foreach (j in it->second)
			{
				if (j->m_guildId == guildId)
				{
					return true;
				}
			}
		}
	}

	return false;
}


void CGuildTerritoryBattle::_SendFailReward()
{
	//巧手不返回引用
	std::unordered_map<UINT64, GCGuildJoin> mapGuildJoin = GuildCastleMgr::Instance()->GetGuildJoin();
	if (!mapGuildJoin.empty())
	{
		for (auto rter = m_cityMap.begin(); rter!=m_cityMap.end(); rter++)
		{
			CityUnit &unit = rter->second;
			auto tier = mapGuildJoin.find(unit.m_guildId);
			if (tier != mapGuildJoin.end())
			{
				mapGuildJoin.erase(tier);
			}
		}
	}

	for (auto iter =  mapGuildJoin.begin(); iter!=mapGuildJoin.end(); iter++)
	{
		GCGuildJoin &guildJoin = iter->second;
		GCGResult att		= guildJoin.GetAttack();
		GCGResult def		= guildJoin.GetDefent();
		if(att.m_isWin || def.m_isWin)
			continue;

		UINT32 attLevel	= 0;
		UINT32 defLevel	= 0;

		TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(att.m_territoryID);
		if (pRowData)
		{
			attLevel = pRowData->territorylevel;
		}

		pRowData = m_territoryTable.GetByID(def.m_territoryID);
		if (pRowData)
		{
			if (pRowData->territorylevel>attLevel)
			{
				attLevel = pRowData->territorylevel;
			}
		}
		std::vector<UINT64> refRoles;
		guildJoin.GetRoles(refRoles);
		GuildAuctMgr::Instance()->AddGuildAuctData(iter->first, GA_ACT_TERRITORY_FAILD, attLevel,refRoles);
		Notice notice(GuildTerrReward);
		notice.SendPlays(refRoles);
	}
}


void CGuildTerritoryBattle::_SendReward()
{
	foreach (i in m_cityMap)
	{
		TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(i->first);
		if (pRowData != NULL)
		{
			GuildAuctMgr::Instance()->AddGuildAuctData(i->second.m_guildId, GA_ACT_TERRITORY, pRowData->territorylevel);
		}
	}
	

}



void CGuildTerritoryBattle::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return ;
	}

	UINT32 weekDay = XCommon::GetWeekNum(nowTime,false);
	foreach (it in GetGlobalConfig().GuildTerritoryAucTime)// 拍卖行奖励
	{
		if (weekDay == it->seq[0])
		{
			UINT32 aucStartTime = XCommon::MakeTime(nowTime, it->seq[1] / 100, it->seq[1] % 100, 0); 
			if (m_aucBeginTime != aucStartTime && nowTime >= aucStartTime)
			{
				foreach (j in m_cityMap)
				{
					TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(j->first);
					if (pRowData != NULL)
					{
						GuildAuctMgr::Instance()->AddGuildAuctData(j->second.m_guildId, GA_ACT_TERRITORY_DAY, pRowData->territorylevel);
						SSInfo << " guild Territory AddGuildAuctData guildid : " << j->second.m_guildId << END; 
					}
				}
				m_aucBeginTime = aucStartTime;
				SetChange();
			}
		}

	}

	foreach (dec in GetGlobalConfig().GuildTerritoryDecTime)
	{

		if (weekDay == dec->seq[0]) // 宣战结束
		{
			UINT32 decBeginTime = XCommon::GetDayBeginTime(nowTime);
			if (nowTime == decBeginTime)
			{
				m_noticeRec.clear();

				PtcM2C_NoticeGuildTerrBigIcon msg;
				msg.m_Data.set_isnow(true);
				const std::unordered_map<UINT64, Guild*>& guildMap = CGuildMgr::Instance()->GetGuilds();
				foreach (pguild in guildMap)
				{
					bool isInCity = false;
					foreach (pcity in m_cityMap)
					{
						if (pguild->second->GetID() == pcity->second.m_guildId)
						{
							isInCity = true;
							break;
						}
					}

					if (!isInCity)
					{
						const std::map<UINT64, GuildMember> allMem = pguild->second->GetGuildMember();
						foreach (i in allMem)
						{
							CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
							if (pRole != NULL)
							{
								pRole->Send(msg);
								m_noticeRec.insert(pRole->GetID());
							}
						}
					}
				}
			}

			UINT32 decEndTime = XCommon::MakeTime(nowTime, dec->seq[2] / 100, dec->seq[2] % 100, 0); 
			if (m_declareEndTime != decEndTime && nowTime >= decEndTime)
			{
				_DoDeclareTimeEnd();
				_NoticeCityMaster();
				_DelGigIcon();
				m_declareEndTime = decEndTime;
				SetChange();
				SSInfo << " guild Territory declareEndTime !"<< END; 
			}

			UINT32 noticeOne = XCommon::MakeTime(nowTime, 10, 0, 0);
			UINT32 noticeTwo = XCommon::MakeTime(nowTime, 12, 0, 0);
			UINT32 noticeThree = XCommon::MakeTime(nowTime, 14, 0, 0);
			UINT32 noticeFour = XCommon::MakeTime(nowTime, 18, 0, 0);
			if (nowTime == noticeOne || nowTime == noticeTwo || nowTime == noticeThree || nowTime == noticeFour)
			{
				NoticeMgr::Instance()->NoticeGuildTerrBegin();
				_NoticeBeginWarning();
			}
		}
	}
	
	auto iter = GetGlobalConfig().GuildTerritoryOpenTime.find(weekDay);
	if (iter != GetGlobalConfig().GuildTerritoryOpenTime.end())
	{
		UINT32 terStartTime = XCommon::MakeTime(nowTime, iter->second.seq[1] / 100, iter->second.seq[1] % 100, 0); 

		if (nowTime < terStartTime)
		{
			UINT32 gapTime = terStartTime - nowTime; 
			if (gapTime == 60 * 3 || gapTime == 60 * 2 || gapTime == 60)
			{
				std::set<UINT64> declareSet;
				_GetWarGuildSet(declareSet);
				foreach (i in declareSet)
				{
					if (gapTime == 60 * 3)
					{
						NoticeMgr::Instance()->NoticeGuildTerrEnterSceneOne(*i);
					}
					else if (gapTime == 60 * 2)
					{
						NoticeMgr::Instance()->NoticeGuildTerrEnterSceneTwo(*i);
					}
					else
					{
						NoticeMgr::Instance()->NoticeGuildTerrEnterSceneThree(*i);
					}
				}
			}
		}

		if (terStartTime != m_battleBeginTime && nowTime >= terStartTime)  // 开启战斗
		{
			_DoStart();
			m_battleBeginTime = terStartTime;
			SetChange();
			SSInfo << " guild Territory do start !"<< END; 
		}

		UINT32 terEndTime = XCommon::MakeTime(nowTime, iter->second.seq[2] / 100, iter->second.seq[2] % 100, 0); 
		if (m_warEndTime != terEndTime && nowTime >= terEndTime)
		{
			_DoWarTimeEnd();
			m_warEndTime = terEndTime;
			SetChange();

			foreach (k in m_cityMap)
			{
				NoticeMgr::Instance()->NoticeGuildTerrReward(k->second.m_guildId);
				_NoticeWarIcon(k->second.m_guildId, false);

				TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(k->first);
				if (pRowData != NULL && pRowData->territorylevel == 3)
				{
					Guild* pGuild = CGuildMgr::Instance()->GetGuild(k->second.m_guildId);
					if (pGuild != NULL)
					{
						NoticeMgr::Instance()->NoticeGuildTerrEnd(pGuild->GetName());
					}
				}
			}
			SSInfo << " guild Territory war end time !"<< END; 

			_SendReward();
			_SendFailReward();
		}
	}
	else
	{
		UINT32 noticeTime = XCommon::MakeTime(nowTime, 20, 30, 0);
		if (nowTime == noticeTime)
		{
			foreach (x in m_cityMap)
			{
				NoticeMgr::Instance()->NoticeGuildTerrOtherReward(x->second.m_guildId);
			}
		}
	}

	// 数据存储
	if ((nowTime - m_UpDataTime) >= UPDATECD)
	{
		if (IsChange())
		{
			Update(); // 存储数据
		}
		m_UpDataTime = nowTime;
	}
}

void CGuildTerritoryBattle::_GetWarGuildSet(std::set<UINT64>& declareSet)
{
	
	foreach (j in m_challList)
	{
		foreach (k in j->second)
		{
			declareSet.insert(k->m_guildId);
		}
	}

	foreach (i in m_cityMap)
	{
		auto iter = m_challList.find(i->first);
		if (iter != m_challList.end() && !iter->second.empty())
		{
			declareSet.insert(i->first);
		}
	}

}

void CGuildTerritoryBattle::_DoWarTimeEnd()
{
	m_tempWaringId.clear();
	m_challList.clear();
	_DoClearJobe();
	SetChange();

	PtcM2C_NoticeGuildTerrEnd msg;
	const std::unordered_map<UINT64, Guild*> allGuild = CGuildMgr::Instance()->GetGuilds();
	foreach (i in allGuild)
	{
		if (i->second == NULL)
		{
			continue;
		}

		const std::map<UINT64, GuildMember> allMem = i->second->GetGuildMember();
		foreach (i in allMem)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
			if (pRole != NULL)
			{
				pRole->Send(msg);
			}
		}
	}
}

void CGuildTerritoryBattle::_DoStart()
{
	m_tempWaringId.clear();

	foreach (j in m_challList)
	{
		UINT64 guildId = 0;
		std::set<UINT64> guildSet;

		for (auto iter = j->second.begin(); iter != j->second.end(); ++iter)
		{
			guildSet.insert(iter->m_guildId);
		}

		if (guildSet.size() == 0)
		{
			continue;
			SSInfo << " START GUILD TERRITORY GUILDSET IS ZERO !" << END;
		}

		if (guildSet.size() > 2)
		{
			continue;
			SSInfo << " START GUILD TERRITORY GUILDSET IS BIG THAN 2 !" << END;
		}

		if (m_cityMap.find(j->first) != m_cityMap.end())
		{
			guildId = m_cityMap.find(j->first)->second.m_guildId; 
		}

		m_tempWaringId.insert(j->first);
		GuildCastleMgr::Instance()->AddOneFight(j->first, guildId, guildSet);
		SSInfo << "guild terr add fight id : " << j->first << "guildid : " << guildId << "guildset size : " << guildSet.size() << END; 

		if (guildId != 0)
		{
			_NoticeWarIcon(guildId, true);
		}
		foreach (it in guildSet)
		{
			_NoticeWarIcon(*it, true);
		}
	}

	foreach (i in m_cityMap)
	{
		if (m_tempWaringId.find(i->first) == m_tempWaringId.end())
		{
			std::set<UINT64> guildset;
			if (XCommon::IsDaySame(GameTime::GetTime(), i->second.m_time))
			{
				guildset.insert(i->second.m_guildId);
				GuildCastleMgr::Instance()->AddOneFight(i->first, 0, guildset);
				SSInfo << "guild terr add fight reward id : " << i->first << "guildid : 0 " << "guildset size : " << guildset.size() << END; 
			}
			else
			{
				GuildCastleMgr::Instance()->AddOneFight(i->first, i->second.m_guildId, guildset);
				SSInfo << "guild terr add fight reward id : " << i->first << "guildid : " << i->second.m_guildId << "guildset size : " << guildset.size() << END; 
			}
		}

		auto it = m_challList.find(i->first);
		if (it == m_challList.end() || it->second.empty())
		{
			TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(i->first);
			if (pRowData != NULL)
			{
				if (pRowData->territorylevel == 1)
				{
					NoticeMgr::Instance()->NoticeGuildTerrDefGuildOne(i->second.m_guildId, pRowData->territoryname);
				}
				else if (pRowData->territorylevel == 2)
				{
					NoticeMgr::Instance()->NoticeGuildTerrDefGuildTwo(i->second.m_guildId, pRowData->territoryname);
				}
				else if (pRowData->territorylevel == 3)
				{
					NoticeMgr::Instance()->NoticeGuildTerrDefGuildThree(i->second.m_guildId, pRowData->territoryname);
				} 
			}

		}
	}

	NoticeMgr::Instance()->NoticeGuildTerrEnterScene();
	SetChange();
}

void CGuildTerritoryBattle::_StartAucReward()
{

}

UINT32 CGuildTerritoryBattle::GetDeclareCityId(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guildId)
			{
				return i->first;
			}
		}
	}
	return 0;
}

UINT32 CGuildTerritoryBattle::GetTimeType()
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return KKSG::TERR_NOT_OPEN;
	}

	UINT32 weekDay = XCommon::GetWeekNum(nowTime);
	
	UINT32 decEndTime = 0;
	foreach (i in GetGlobalConfig().GuildTerritoryDecTime)
	{
		UINT32 decStartTime = XCommon::MakeTime(nowTime, i->seq[1] / 100, i->seq[1] % 100, 0); 
		decEndTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0); 
		if (weekDay == i->seq[0] && nowTime >= decStartTime && nowTime <= decEndTime)
		{
			return KKSG::ALLIANCE;
		}
	}

	foreach (j in GetGlobalConfig().GuildTerritoryOpenTime)
	{
		UINT32 terStartTime = XCommon::MakeTime(nowTime, j->second.seq[1] / 100, j->second.seq[1] % 100, 0); 
		UINT32 terEndTime = XCommon::MakeTime(nowTime, j->second.seq[2] / 100, j->second.seq[2] % 100, 0); 
		if (weekDay == j->second.seq[0])
		{
			if (nowTime > decEndTime && nowTime < terStartTime)
			{
				return KKSG::WAITING;
			}

			if (nowTime >= terStartTime && nowTime < terEndTime) 
			{
				if (!m_tempWaringId.empty())
				{
					return KKSG::TERR_WARING;
				}
				else
				{
					return KKSG::TERR_END;
				}
			}
			else if (nowTime >= terEndTime)
			{
				return KKSG::TERR_END;
			}
		}
	}
	

	return KKSG::TERR_NOT_OPEN;
}

void CGuildTerritoryBattle::_DoDeclareTimeEnd()
{
	foreach (i in m_challList)
	{
		if (i->second.empty())
		{
			continue;
		}

		ReSort(i->first);

		if (m_cityMap.find(i->first) == m_cityMap.end())
		{
			if (i->second.size() == 1)  // 唯一一个直接胜利
			{
				/*
				foreach (j in m_cityMap)
				{
					if (j->second.m_guildId == i->second.begin()->m_guildId)
					{
						m_cityMap.erase(j);
						break;
					}
				}
				*/

				CityUnit& data = m_cityMap[i->first];
				data.m_guildId = i->second.begin()->m_guildId;
				data.m_time = GameTime::GetTime(); 
				i->second.clear();
				SetChange();
				TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(i->first);
				if (pRowData->territorylevel == 1)
				{
					NoticeMgr::Instance()->NoticeGuildTerrRewardOne(m_cityMap[i->first].m_guildId, pRowData->territoryname);
				}
				else if (pRowData->territorylevel == 2)
				{
					NoticeMgr::Instance()->NoticeGuildTerrRewardTwo(m_cityMap[i->first].m_guildId, pRowData->territoryname);
				}
				else if (pRowData->territorylevel == 3)
				{
					NoticeMgr::Instance()->NoticeGuildTerrRewardThree(m_cityMap[i->first].m_guildId, pRowData->territoryname);
				}

				Guild* pGuild = CGuildMgr::Instance()->GetGuild(data.m_guildId);
				if (pGuild == NULL)
				{
					continue;
				}
				// 如果本次领地等级高于之前等级，则更新领地等级
				if (pRowData->territorylevel > pGuild->GetMaxTerritoryLevel())
				{
					pGuild->SetMaxTerritoryLevel(pRowData->territorylevel);
				}
			}
			else
			{
				auto iter = i->second.begin(); // 保存两个
				_NoticeCanDeclare(i->first, iter->m_guildId);
				++iter;
				_NoticeCanDeclare(i->first, iter->m_guildId);
				++iter;

				for ( ; iter != i->second.end(); )
				{
					_ReturnDrogn(iter->m_roleId); // 返回龙币
					_NoticeLoseDeclare(i->first, iter->m_guildId);
					i->second.erase(iter++);
					SetChange();
				}
			}
		}
		else
		{

			UINT64 guildOne = i->second.begin()->m_guildId;
			UINT64 guildTwo = i->second.begin()->m_allianceId;
			if (guildOne != 0)
			{
				_NoticeCanDeclare(i->first, guildOne);
			}
			if (guildTwo != 0)
			{
				_NoticeCanDeclare(i->first, guildTwo);
			}

			for (auto iter = i->second.begin(); iter != i->second.end(); )
			{
				if (iter->m_guildId != guildOne && iter->m_guildId != guildTwo)
				{
					_ReturnDrogn(iter->m_roleId); // 返回龙币
					_NoticeLoseDeclare(i->first, iter->m_guildId);
					i->second.erase(iter++);
					SetChange();
				}
				else
				{
					++iter;
				}
			}
		}
	}

	for (auto it = m_challList.begin(); it != m_challList.end(); )
	{
		if (it->second.empty())
		{
			m_challList.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void CGuildTerritoryBattle::_NoticeCanDeclare(UINT32 id, UINT64 guildId)
{
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(id);
	if (pRowData != NULL)
	{
		if (pRowData->territorylevel == 1)
		{
			NoticeMgr::Instance()->NoticeGuildTerrCanDeclareOne(guildId, pRowData->territoryname);
		}
		if (pRowData->territorylevel == 2)
		{
			NoticeMgr::Instance()->NoticeGuildTerrCanDeclareTwo(guildId, pRowData->territoryname);
		}
		if (pRowData->territorylevel == 3)
		{
			NoticeMgr::Instance()->NoticeGuildTerrCanDeclareThree(guildId, pRowData->territoryname);
		}
	}
}

void CGuildTerritoryBattle::_NoticeLoseDeclare(UINT32 id, UINT64 guildId)
{
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(id);
	if (pRowData != NULL)
	{
		if (pRowData->territorylevel == 1)
		{
			NoticeMgr::Instance()->NoticeGuildTerrNotDeclareOne(guildId, pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 2)
		{
			NoticeMgr::Instance()->NoticeGuildTerrNotDeclareTwo(guildId, pRowData->territoryname);
		}
		else if (pRowData->territorylevel == 3)
		{
			NoticeMgr::Instance()->NoticeGuildTerrNotDeclareThree(guildId, pRowData->territoryname);
		}
	}
}

void CGuildTerritoryBattle::_NoticeCityMaster()
{
	foreach (j in m_cityMap)
	{
		std::set<UINT64> declareSet;
		_GetAllDeclare(j->first, declareSet);

		if (declareSet.empty())
		{
			if (!XCommon::IsDaySame(GameTime::GetTime(), j->second.m_time))
			{
				NoticeMgr::Instance()->NoticeGuildTerrWarningNoDeclare(j->second.m_guildId);
			}
		}
		else if (declareSet.size() == 1)
		{
			Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(*(declareSet.begin()));
			if (pGuildOne != NULL)
			{
				NoticeMgr::Instance()->NoticeGuildTerrWarningDeclare(j->second.m_guildId, pGuildOne->GetName());
			}
		}
		else if (declareSet.size() == 2)
		{
			auto iterSet = declareSet.begin(); 
			Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(*iterSet);
			++iterSet;
			Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(*iterSet);
			if (pGuildOne != NULL && pGuildTwo != NULL)
			{
				NoticeMgr::Instance()->NoticeGuildTerrWarningDeclareTwo(j->second.m_guildId, pGuildOne->GetName(), pGuildTwo->GetName());
			}

		}
	}

}

UINT32 CGuildTerritoryBattle::GetGameType(UINT32 id,bool haveTerritory,Guild* guild)
{
	UINT64 guildId = guild->GetID();
	if (_IsWaitTime())
	{
		if (_IsDeclareHasGuild(id, guildId))
		{
			return KKSG::WAITING;
		}
		else
		{
			auto iter = m_cityMap.find(id);
			if (iter != m_cityMap.end() && iter->second.m_guildId == guildId && _IsCityBeDeclare(id))
			{
				return KKSG::WAITING;
			}
			return KKSG::TERR_NOT_OPEN;
		}
	}

	if (_IsWarTime())
	{
		if (_IsDeclareHasGuild(id, guildId))
		{
			return KKSG::TERR_WARING;
		}
		else
		{
			auto iter = m_cityMap.find(id);
			if (iter != m_cityMap.end() && iter->second.m_guildId == guildId && _IsCityBeDeclare(id))
			{
				return KKSG::TERR_WARING;
			}
			return KKSG::TERR_NOT_OPEN;
		}
	}

	if (IsDeclareTime())
	{
		// 如果有领地，只能对更高的宣战，如果没有领地，可以对不高于之前最高占领的宣战
		if (haveTerritory)
		{
			if ((_GetGuildTerrType(guildId) + 1) == _GetCityType(id))
			{
				return KKSG::ALLIANCE;
			}
			else
			{
				return KKSG::TERR_NOT_OPEN;
			}
		}
		else
		{
			TerritoryBattle::RowData* pConf = GetTerrConf(id);
			if(NULL == pConf)
			{
				SSError << " find terr conf failed id = " << id << END;
				return KKSG::TERR_NOT_OPEN;
			}

			if( pConf->territorylevel <= guild->GetMaxTerritoryLevel())
			{
				return KKSG::ALLIANCE;
			}
			else
			{
				return KKSG::TERR_NOT_OPEN;
			}			
		}
	}

	return KKSG::TERR_NOT_OPEN;
}


bool CGuildTerritoryBattle::_IsCityBeDeclare(UINT32 id)
{
	auto iter = m_challList.find(id);
	if (iter != m_challList.end() && !iter->second.empty())
	{
		return true;
	}
	return false;
}

UINT32 CGuildTerritoryBattle::GetCDTime()
{
	if (!_IsWaitTime())
	{
		return 0;
	}

	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekDay = XCommon::GetWeekNum(nowTime);
	auto iter = GetGlobalConfig().GuildTerritoryOpenTime.find(weekDay); 
	if (iter != GetGlobalConfig().GuildTerritoryOpenTime.end())
	{
		UINT32 terstartTime = XCommon::MakeTime(nowTime, iter->second.seq[1] / 100, iter->second.seq[1] % 100, 0); 
		return terstartTime > nowTime ? (terstartTime - nowTime) : 0;
	}
	return 0;
}

bool CGuildTerritoryBattle::_IsWaitTime()
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return false;
	}


	UINT32 weekDay = XCommon::GetWeekNum(nowTime);
	foreach (i in GetGlobalConfig().GuildTerritoryDecTime)
	{
		if (weekDay != i->seq[0])
		{
			continue;
		}

		auto iter = GetGlobalConfig().GuildTerritoryOpenTime.find(i->seq[0]);
		if (iter != GetGlobalConfig().GuildTerritoryOpenTime.end())
		{
			UINT32 decEndTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0); 
			UINT32 terstertTime = XCommon::MakeTime(nowTime, iter->second.seq[1] / 100, iter->second.seq[1] % 100, 0); 
			if (nowTime > decEndTime && nowTime < terstertTime)
			{
				return true;
			}
		}
	}
	return false;
}

bool CGuildTerritoryBattle::_IsDeclareHasGuild(UINT32 id, UINT64 guildId)
{
	auto iter = m_challList.find(id);
	if (iter != m_challList.end())
	{
		foreach (i in iter->second)
		{
			if (i->m_guildId == guildId)
			{
				return true;
			}
		}
	}

	return false;
}

bool CGuildTerritoryBattle::IsDeclareTime()
{
	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return false;
	}

	UINT32 weekDay = XCommon::GetWeekNum(nowTime,false);
	foreach (i in GetGlobalConfig().GuildTerritoryDecTime)
	{
		UINT32 decStartTime = XCommon::MakeTime(nowTime, i->seq[1] / 100, i->seq[1] % 100, 0); 
		UINT32 decEndTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0); 
		if (weekDay == i->seq[0] && nowTime >= decStartTime && nowTime < decEndTime)
		{
			return true;
		}
	}
	return false;
}

bool CGuildTerritoryBattle::_IsWarTime()
{

	UINT32 nowTime = GameTime::GetTime();
	if (_GetWeekNum() < GetGlobalConfig().GuildTerritoryOpenWeek)
	{
		return false;
	}

	UINT32 weekDay = XCommon::GetWeekNum(nowTime);
	auto iter = GetGlobalConfig().GuildTerritoryOpenTime.find(weekDay);
	if (iter != GetGlobalConfig().GuildTerritoryOpenTime.end())
	{
		UINT32 warStartTime = XCommon::MakeTime(nowTime, iter->second.seq[1] / 100, iter->second.seq[1] % 100, 0); 
		UINT32 warEndTime = XCommon::MakeTime(nowTime, iter->second.seq[2] / 100, iter->second.seq[2] % 100, 0); 
		if (nowTime >= warStartTime && nowTime <= warEndTime)
		{
			return true;
		}
	}
	return false;
}

bool CGuildTerritoryBattle::_HasAlliance(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guildId && j->m_allianceId != 0)
			{
				return true;
			}
		}
	}

	return false;
}

UINT32 CGuildTerritoryBattle::ClearAlliance(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guildId)
			{
				TerritoryGuild data = *j;
				data.m_reqAllianceSet.clear();
				i->second.erase(j);
				i->second.insert(data);
				SetChange();
				return KKSG::ERR_SUCCESS;
			}
		}
	}
	return KKSG::ERR_SUCCESS;
}

void CGuildTerritoryBattle::_ReturnDrogn(UINT64 roleid)
{
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_ReturnGuildTerr);
	if (pMailConf == NULL)
	{
		return ;
	}

	std::vector<ItemDesc> vecItem;
	ItemDesc item(GetGlobalConfig().GuildTerritoryCost.seq[0], GetGlobalConfig().GuildTerritoryCost.seq[1], false);
	vecItem.push_back(item);

	MailData mail(pMailConf->m_title, pMailConf->m_content, vecItem, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(roleid, mail);
}

void CGuildTerritoryBattle::_NoticeWarIcon(UINT64 guildId, bool isBegin)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return; 
	}

	PtcM2C_NoticeGuildTerrWar msg;
	if (isBegin)
	{
		msg.m_Data.set_isbegin(true);
	}
	else
	{
		msg.m_Data.set_isbegin(false);
	}

	const std::map<UINT64, GuildMember> allMem = pGuild->GetGuildMember();
	foreach (i in allMem)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
		if (pRole != NULL)
		{
			pRole->Send(msg);
		}
	}
}

void CGuildTerritoryBattle::_NoticeAllIcon(UINT64 guildId)
{
	if (_HasAlliance(guildId))
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return; 
	}

	PtcM2C_NoticeGuildTerrall msg;
	msg.m_Data.set_num(_GetAlliacnceNum(pGuild->GetID()));

	const std::map<UINT64, GuildMember> allMem = pGuild->GetGuildMember();
	foreach (i in allMem)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
		if (pRole != NULL && (pGuild->isLeader(pRole->GetID()) || pGuild->isViceLeader(pRole->GetID())))
		{
			pRole->Send(msg);
		}
	}
}

UINT32 CGuildTerritoryBattle::_GetAlliacnceNum(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guildId)
			{
				return j->m_reqAllianceSet.size();
			}
		}
	}

	return 0;
}

void CGuildTerritoryBattle::_CheckAllIcon(UINT64 roleId)
{
	if (!IsDeclareTime())
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return; 
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return ;
	}

	bool isInCity = false;
	foreach (i in m_cityMap)
	{
		if (i->second.m_guildId == pGuild->GetID())
		{
			isInCity = true;
			break;
		}
	}
	if (!isInCity && m_noticeRec.find(roleId) == m_noticeRec.end())
	{
		PtcM2C_NoticeGuildTerrBigIcon msgBig;
		msgBig.m_Data.set_isnow(true);
		pRole->Send(msgBig);
		m_noticeRec.insert(roleId);
	}

	if (GetDeclareCityId(pGuild->GetID()) == 0)
	{
		return ;
	}

	if (_HasAlliance(pGuild->GetID()))
	{
		return ;
	}

	PtcM2C_NoticeGuildTerrall msg;
	msg.m_Data.set_num(_GetAlliacnceNum(pGuild->GetID()));

	if (pGuild->isLeader(pRole->GetID()) || pGuild->isViceLeader(pRole->GetID()))
	{
		pRole->Send(msg);
	}
}

void CGuildTerritoryBattle::_CheckWarIcon(UINT64 roleId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return; 
	}

	UINT32 id = GetDeclareCityId(pGuild->GetID());
	if (id == 0)
	{
		return ;
	}

	if (m_tempWaringId.find(id) != m_tempWaringId.end())
	{
		PtcM2C_NoticeGuildTerrWar msg;
		msg.m_Data.set_isbegin(true);
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
		if (pRole != NULL)
		{
			pRole->Send(msg);
		}
	}
}

void CGuildTerritoryBattle::DoOnlineCheck(UINT64 roleId)
{
	_CheckAllIcon(roleId);
	_CheckWarIcon(roleId);
}

void CGuildTerritoryBattle::GetAllCityId(std::set<UINT32>& idSet)
{
	idSet.clear();
	foreach (i in m_territoryTable.Table)
	{
		idSet.insert((*i)->ID);
	}
}

UINT64 CGuildTerritoryBattle::GetAllianceId(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		foreach (j in i->second)
		{
			if (j->m_guildId == guildId)
			{
				return j->m_allianceId;
			}
		}
	}

	return 0;
}

void CGuildTerritoryBattle::_NoticeBeginWarning()
{
	foreach (i in m_cityMap)
	{
		auto iter = m_challList.find(i->first); 
		if (iter != m_challList.end())
		{
			NoticeMgr::Instance()->NoticeGuildTerrWarning(i->second.m_guildId, iter->second.size());
		}
	}
}

void CGuildTerritoryBattle::_GetAllDeclare(UINT32 id, std::set<UINT64>& declareSet)
{
	auto iter = m_challList.find(id);
	if (iter != m_challList.end() && !iter->second.empty())
	{
		declareSet.insert(iter->second.begin()->m_guildId);
		if (iter->second.begin()->m_allianceId != 0)
		{
			declareSet.insert(iter->second.begin()->m_allianceId);
		}
	}
}

void CGuildTerritoryBattle::_ClearAllianceList(UINT64 guildId)
{
	foreach (i in m_challList)
	{
		std::set<TerritoryGuild> tempSet;
		for (auto iter = i->second.begin(); iter != i->second.end();)
		{
			if (iter->m_reqAllianceSet.find(guildId) != iter->m_reqAllianceSet.end()
				|| iter->m_allianceId == guildId)
			{
				TerritoryGuild data = *iter;
				data.m_reqAllianceSet.erase(guildId);
				if (data.m_allianceId == guildId)
				{
					data.m_allianceId = 0;

					Guild* pGuild = CGuildMgr::Instance()->GetGuild(iter->m_guildId);
					Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(guildId);
					if (pGuild != NULL && pGuildTwo != NULL)
					{
						NoticeMgr::Instance()->NoticeGuildTerrDelAlliance(pGuild->GetID(), pGuildTwo->GetName());
						NoticeMgr::Instance()->NoticeGuildTerrDelAlliance(pGuildTwo->GetID(), pGuild->GetName());
					}
				}
				tempSet.insert(data);
				i->second.erase(iter++);
				SetChange();
			}
			else
			{
				++iter;
			}
		}

		foreach (j in tempSet)
		{
			i->second.insert(*j);
		}
	}
}

void CGuildTerritoryBattle::ClearData(bool all)
{
	m_battleBeginTime = 0;
	m_aucBeginTime = 0;
	m_declareEndTime = 0;
	m_warEndTime = 0;
	m_rewardStartTime = 0;

	m_tempWaringId.clear();
	m_challList.clear();
	if (all)
	{
		m_cityMap.clear();
	}
	SetChange();
}

void CGuildTerritoryBattle::ReSort(UINT32 id)
{
	auto iter = m_challList.find(id);
	if (iter != m_challList.end())
	{
		std::set<TerritoryGuild> temp = iter->second;
		iter->second.clear();
		foreach (i in temp)
		{
			iter->second.insert(*i);
		}
	}
}

UINT32 CGuildTerritoryBattle::_GetWeekNum()
{
	UINT32 weekTime = 7 * 24 * 60 * 60;
	UINT32 nowTime = GameTime::GetTime(); 
	UINT32 serverOpenTime = GameTime::GetWeekBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()) - 5 * 60 * 60; //开服时间的周开始时间
	if (nowTime <= serverOpenTime)
	{
		return 0;
	}

	return (nowTime - serverOpenTime) / weekTime + 1; 
}

void CGuildTerritoryBattle::DoGuildDismiss(UINT64 guildId)
{
	for (auto iter = m_cityMap.begin(); iter != m_cityMap.end();)
	{
		if (iter->second.m_guildId == guildId)
		{
			m_cityMap.erase(iter++);
			SetChange();
		}
		else
		{
			++iter;
		}
	}

	for (auto it = m_challList.begin(); it != m_challList.end(); ++it)
	{
		auto i = it->second.find(guildId);
		if (i != it->second.end())
		{
			it->second.erase(i);
			SetChange();
		}
	}
}

void CGuildTerritoryBattle::OnDesignation(UINT64 nGuildID,UINT32 nTerritoryID)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild)
		return;
	TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(nTerritoryID);
	if (!pRowData)
		return;
	EventMgr::Instance()->AddEvent(pGuild->GetLeaderID(), DESIGNATION_COM_GUILDTERRITORYBATTLE,nTerritoryID); 
}

void CGuildTerritoryBattle::_DoClearJobe()
{
	std::map<UINT64, std::set<UINT32> > tempData;
	foreach (i in m_cityMap)
	{
		std::set<UINT32>& data = tempData[i->second.m_guildId]; 
		data.insert(i->first);
	}

	foreach (j in tempData)
	{
		if (j->second.size() >= 2)
		{
			UINT32 maxLvl = 0;
			UINT32 maxId = 0;
			UINT32 oldId = 0;
			foreach (k in j->second)
			{
				TerritoryBattle::RowData* pRowData = m_territoryTable.GetByID(*k);
				if (pRowData != NULL && pRowData->territorylevel > maxLvl)
				{
					maxId = *k;
					maxLvl = pRowData->territorylevel; 
				}
			}

			foreach (x in j->second)
			{
				if (*x != maxId)
				{
					m_cityMap.erase(*x);
					oldId = *x;
				}
			}

			TerritoryBattle::RowData* pRowDataOne = m_territoryTable.GetByID(maxId);
			TerritoryBattle::RowData* pRowDataTwo = m_territoryTable.GetByID(oldId);
			if (pRowDataOne != NULL && pRowDataTwo != NULL && pRowDataOne->territorylevel == 2)
			{
				NoticeMgr::Instance()->NoticeGuildTerrWinOne(j->first, pRowDataOne->territoryname, pRowDataTwo->territoryname);
			}
			if (pRowDataOne != NULL && pRowDataTwo != NULL && pRowDataOne->territorylevel == 3)
			{
				NoticeMgr::Instance()->NoticeGuildTerrWinTwo(j->first, pRowDataOne->territoryname, pRowDataTwo->territoryname);
			}
		}
	}

	for(auto iter = m_cityMap.begin(); iter != m_cityMap.end();iter++)
	{
		OnDesignation(iter->second.m_guildId, iter->first);
	}
}

void CGuildTerritoryBattle::_DelGigIcon()
{
	PtcM2C_NoticeGuildTerrBigIcon msg;
	msg.m_Data.set_isnow(false);

	foreach (i in m_noticeRec)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(*i);
		if (pRole != NULL)
		{
			pRole->Send(msg);
		}
	}
	m_noticeRec.clear();
}
