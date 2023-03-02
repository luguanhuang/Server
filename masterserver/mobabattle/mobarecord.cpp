#include "pch.h"
#include "mobarecord.h"
#include "table/globalconfig.h"
#include <cmath>
#include "role/rolemanager.h"
#include "militaryrank/ptcm2g_addmilitaryrank.h"
#include "mobarecordmgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "mobabattlemgr.h"
#include "role/rolesummarymgr.h"
#include "common/rpcm2g_msgiveitem.h"
#include "config/itemdropconfig.h"
#include "define/tlogenum.h"
#include "common/ptcm2c_mserrornotify.h"
#include "loghelper/reportdatamgr.h"
#include "loghelper/tlogr.h"

MobaRecord::MobaRecord()
{
	_Init();
}

MobaRecord::MobaRecord(UINT64 roleid)
{
	_Init();
	m_roleid = roleid;
}

MobaRecord::~MobaRecord()
{

}

void MobaRecord::ToKKSG(KKSG::MobaBattleRoleRecord& record)
{
	record.Clear();
	record.set_elopoint(m_eloPoint);
	record.set_tagcount(m_tagCount);
	record.set_totalnum(m_totalNum);
	record.set_winnum(m_winNum);
	record.set_lastupdatetime(m_lastUpdateTime);
	record.set_winthisweek(m_winThisWeek);
	record.set_weekprize(m_weekPrize);
	record.set_todaynum(m_todayNum);
	record.set_realtotalnum(m_realTotalNum);
	record.set_realwinnum(m_realWinNum);
	for (auto i = m_gameRecord.begin(); i != m_gameRecord.end(); i++)
	{
		record.add_gamerecord()->CopyFrom(*i);
	}
}

void MobaRecord::ToThis(const KKSG::MobaBattleRoleRecord& record)
{
	m_eloPoint = record.elopoint();
	m_tagCount = record.tagcount();
	m_totalNum = record.totalnum();
	m_winNum = record.winnum();
	m_lastUpdateTime = record.lastupdatetime();
	m_winThisWeek = record.winthisweek();
	m_weekPrize = record.weekprize();
	m_todayNum = record.todaynum();
	m_realWinNum = record.realwinnum();
	if (record.has_realtotalnum()) m_realTotalNum = record.realtotalnum();
	else m_realTotalNum = m_totalNum;
	for (int i = 0; i < record.gamerecord_size(); i++)
	{
		m_gameRecord.push_back(record.gamerecord(i));
	}
}

void MobaRecord::SetChanged()
{
	MobaRecordMgr::Instance()->AddChangedToQueue(m_roleid);
}

void MobaRecord::AddGameRecord(const KKSG::AddMobaBattleRecordData& data)
{
	Update();

	if (!data.has_game()) return;
	UINT32 belongTeamid = 0;
	const KKSG::MobaBattleOneGameRole* pOneGameRole = NULL;
	std::vector<UINT64> opponents;
	for (int i = 0; i < data.game().team1_size(); ++i)
	{
		if (data.game().team1(i).roleid() == m_roleid)
		{
			belongTeamid = 1;
			pOneGameRole = &data.game().team1(i);
			opponents.clear();
			break;
		}
		opponents.push_back(data.game().team1(i).roleid());
	}
	if (belongTeamid == 0)
	{
		for (int i = 0; i < data.game().team2_size(); ++i)
		{
			if (data.game().team2(i).roleid() == m_roleid)
			{
				belongTeamid = 2;
				pOneGameRole = &data.game().team2(i);
				break;
			}
		}
		if (belongTeamid == 0)
		{
			LogError("role [%llu], not in this moba game record, strange error", m_roleid);
			return;
		}
	}
	else
	{
		for (int i = 0; i < data.game().team2_size(); ++i)
		{
			opponents.push_back(data.game().team2(i).roleid());
		}
	}
	bool isWin;
	if (pOneGameRole->isescape())
	{
		isWin = false;
	}
	else
	{
		isWin = belongTeamid == data.game().winteamid();
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roleid);
	if (pRole != NULL)
	{
		if (isWin)
		{
			bool isHave = false;
			UINT32 num = 0;
			for (auto i = m_gameRecord.rbegin(); i != m_gameRecord.rend(); ++i)
			{
				if (num >= GetGlobalConfig().MilitarySameOpponentRound)
				{
					break;
				}
				std::set<UINT64> team1, team2;
				for (int j = 0; j < (*i).team1_size(); ++j)
				{
					team1.insert((*i).team1(j).roleid());
				}
				for (int j = 0; j < (*i).team2_size(); ++j)
				{
					team2.insert((*i).team2(j).roleid());
				}

				std::set<UINT64>* pTeam;
				if (team1.find(m_roleid) == team1.end())
				{
					pTeam = &team1;
					if ((*i).winteamid() == 1) continue;
				}
				else
				{
					pTeam = &team2;
					if ((*i).winteamid() == 2) continue;
				}
				for (UINT32 j = 0; j < opponents.size(); ++j)
				{
					if (pTeam->find(opponents[j]) != pTeam->end())
					{
						isHave = true;
						break;
					}
				}
				if (isHave)
				{
					break;
				}
				++num;
			}
			if (!isHave)
			{
				PtcM2G_AddMilitaryRank oPtc;
				oPtc.m_Data.set_iswin(true);
				oPtc.m_Data.set_roleid(m_roleid);
				oPtc.m_Data.set_scenetype(KKSG::SCENE_MOBA);
				pRole->SendMsgToGS(oPtc);
			}
			else
			{
				PtcM2C_MSErrorNotify oPtc;
				oPtc.m_Data.set_errorno(KKSG::ERR_MILITARY_TRIGGER_CHECK);
				oPtc.m_Data.set_istip(true);
				pRole->Send(oPtc);
			}
		}
		else
		{
			PtcM2G_AddMilitaryRank oPtc;
			oPtc.m_Data.set_iswin(false);
			oPtc.m_Data.set_roleid(m_roleid);
			oPtc.m_Data.set_scenetype(KKSG::SCENE_MOBA);
			pRole->SendMsgToGS(oPtc);
		}
	}

	double newELOPoint, otherELO = belongTeamid == 1 ? data.elopoint2() : data.elopoint1();	
	if (isWin)
	{
		++m_winNum;
		++m_realWinNum;
		++m_winThisWeek;
		newELOPoint = m_eloPoint + 1.0 * GetGlobalConfig().HeroBattleELOK * (1.0 - 1.0 / (1.0 + pow(10, (otherELO - m_eloPoint) / 400.0)));
	}
	else
	{
		newELOPoint = m_eloPoint + 1.0 * GetGlobalConfig().HeroBattleELOK * (0.0 - 1.0 / (1.0 + pow(10, (otherELO - m_eloPoint) / 400.0)));
	}
	++m_totalNum;
	++m_todayNum;
	++m_realTotalNum;
	CReportDataMgr::Instance()->MobaResult(m_roleid, m_todayNum, m_realTotalNum, m_realWinNum, isWin);

	LogInfo("role [%llu], isescape [%d], iswin [%d], elopoint [%.2lf] to [%.2lf]", m_roleid, pOneGameRole->isescape(), isWin, m_eloPoint, newELOPoint);

	///> elo TLog
	TMobaEloFlow tlog(data.transtag());
	tlog.m_uRoleID = m_roleid;
	tlog.m_Elo = m_eloPoint;
	tlog.m_NewElo = newELOPoint;
	tlog.Do();

	m_eloPoint = newELOPoint;
	m_gameRecord.push_back(data.game());
	
	m_gameRecord.back().set_tag(m_tagCount);
	++m_tagCount;

	while (m_gameRecord.size() > GetGlobalConfig().MobaGameRecordMaxNum)
	{
		m_gameRecord.pop_front();
	}

	SetChanged();	
}

void MobaRecord::FillGameBriefRecord(KKSG::GetMobaBattleBriefRecordRes& res)
{
	Update();

	res.set_totalnum(m_totalNum);
	res.set_winnum(m_winNum);
	for (auto i = m_gameRecord.begin(); i != m_gameRecord.end(); ++i)
	{
		const KKSG::MobaBattleOneGameRole* pOneGameRole = NULL;
		const KKSG::MobaBattleOneGame& oneGame = *i;
		UINT32 belongTeamid = 0;
		for (int j = 0; j < oneGame.team1_size(); ++j)
		{
			if (oneGame.team1(j).roleid() == m_roleid)
			{
				belongTeamid = 1;
				pOneGameRole = &oneGame.team1(j);
				break;
			}
		}
		if (belongTeamid == 0)
		{
		    for (int j = 0; j < oneGame.team2_size(); ++j)
		    {
				if (oneGame.team2(j).roleid() == m_roleid)
		   		{
					belongTeamid = 2;
		   			pOneGameRole = &oneGame.team2(j);
				}
			}
			if (belongTeamid == 0)
			{
				LogError("role [%llu], not in this moba game record, but should judge previous, very strange error!!", m_roleid);
				continue;
			}
		}
		KKSG::MobaBattleOneGameBrief* pBrief = res.add_brief();
		pBrief->set_tag(oneGame.tag());
		pBrief->set_date(oneGame.date());
		pBrief->set_heroid(pOneGameRole->heroid());
		pBrief->set_iswin(belongTeamid == oneGame.winteamid());
		pBrief->set_isescape(pOneGameRole->isescape());
		pBrief->set_ismvp(m_roleid == oneGame.mvpid());
		pBrief->set_islosemvp(m_roleid == oneGame.losemvpid());
	}
}

void MobaRecord::FillOneGameRecord(UINT32 tag, KKSG::GetMobaBattleGameRecordRes& res)
{
	Update();

	for (auto i = m_gameRecord.begin(); i != m_gameRecord.end(); ++i)
	{
		if ((*i).tag() == tag)
		{
			res.mutable_record()->CopyFrom(*i);
			return;
		}
	}
}

void MobaRecord::Update()
{
	time_t nowTime = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_lastUpdateTime, nowTime))
	{
		if (!XCommon::IsWeekSame(m_lastUpdateTime, nowTime))
		{
			m_winThisWeek = 0;
			m_weekPrize = 0;
		}
		m_lastUpdateTime = nowTime;
		m_todayNum = 0;

		SetChanged();
	}
}

bool MobaRecord::CanGetNextWeekPrize()
{
	Update();

	MobaWeekRewardConf* conf = MobaBattleMgr::Instance()->GetMobaWeekRewardConf(m_weekPrize + 1);
	if (conf == NULL)
	{
		return false;
	}
	return m_winThisWeek >= conf->winnum;
}

void MobaRecord::FillMobaBattleInfo(KKSG::GetMobaBattleInfoRes& res)
{
	Update();

	res.set_winthisweek(m_winThisWeek);
	res.set_weekprize(m_weekPrize);
	res.set_getnextweekprize(CanGetNextWeekPrize());
}

KKSG::ErrorCode MobaRecord::GetWeekNextReward(CRole* pRole)
{
	if (pRole == NULL || pRole->GetID() != m_roleid)
	{
		return KKSG::ERR_UNKNOWN;
	}

    Update();

	MobaWeekRewardConf* conf = MobaBattleMgr::Instance()->GetMobaWeekRewardConf(m_weekPrize + 1);
	if (!CanGetNextWeekPrize() || conf == NULL)
    {
		return KKSG::ERR_HEROBATTLE_CANTGETPRIZE;
	}

    CRoleSummary* pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleid);
	if (pSummary == NULL)
    {
		return KKSG::ERR_FAILED;
	}

    ++m_weekPrize;
	
	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(m_roleid);
	rpc->m_oArg.set_reason(ItemFlow_Mobabattle);
	rpc->m_oArg.set_subreason(ItemFlow_MobaBattle_WeekReward); 
	for (UINT32 i = 0; i < conf->reward.size(); ++i) 
	{
		UINT32 dropid = conf->reward[i];
		const ItemDesc* desc = ItemDropConfig::Instance()->GetRandomDrop(dropid, pSummary->GetLevel());
		if (desc == NULL)
		{
			continue; 
		}
		desc->ToKKSG(rpc->m_oArg.add_items());
	}
	pRole->SendMsgToGS(*rpc);

	SetChanged();
	LogInfo("role [%llu] get moba week reward [%u]", m_roleid, m_weekPrize);
	
	return KKSG::ERR_SUCCESS;
}

void MobaRecord::_Init()
{
	m_roleid = 0;
	m_eloPoint = GetGlobalConfig().HeroBattleOriginELOPoint;
	m_tagCount = 0;
	m_totalNum = 0;
	m_winNum = 0;

	m_lastUpdateTime = 0;
	m_winThisWeek = 0;
	m_weekPrize = 0;
	m_todayNum = 0;
	m_realTotalNum = 0;
	m_realWinNum = 0;
}