#include "pch.h"
#include "pvprecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"

#include <time.h>
#include "globalconfig.h"
#include "pvpmgr.h"
#include "rolesummarymgr.h"
#include "rolesummary.h"
#include "reportdatamgr.h"
#include "gamelogic/military_rank_record.h"
#include "util/gametime.h"
#include "common/ptcg2c_gserrornotify.h"

MyPvpRecord::MyPvpRecord(Role* role)
{
	m_poRole = role;
	m_joinCountTodayInT = 0;
	m_winCountThisWeek = 0;
	m_winCountAll = 0;
	m_loseCountAll = 0;
	m_drawCountAll = 0;

	m_lastDayUpdateT = (time_t)0;
	m_lastWeekUpdateT = (time_t)0;

	m_weekRewardHaveGet = false;

	m_todayplaytime = 0;
	m_todayplaytimes = 0;
}

MyPvpRecord::~MyPvpRecord()
{

}

bool MyPvpRecord::Load( const KKSG::RoleAllInfo *poRoleAllInfo )
{
	if(poRoleAllInfo->has_pvpdata())
	{
		const KKSG::PvpData& data = poRoleAllInfo->pvpdata();
		m_joinCountTodayInT = data.joincounttodayint();
		m_winCountThisWeek = data.wincountthisweek();
		m_winCountAll = data.wincountall();
		m_loseCountAll = data.losecountall();
		m_drawCountAll = data.drawcountall();

		m_lastDayUpdateT = (time_t)data.lastdayupt();
		m_lastWeekUpdateT = (time_t)data.lastweekupt();

		m_weekRewardHaveGet = data.weekrewardhaveget();

		m_todayplaytime = data.todayplaytime();
		m_todayplaytimes = data.todayplaytimes();

		m_pvpRecs.clear();
		for(int i = 0; i < data.pvprecs_size(); ++i)
		{
			m_pvpRecs.push_back(data.pvprecs(i));
		}
	}
	else
	{
		time_t now = time(NULL);
		m_lastDayUpdateT = now;
		m_lastWeekUpdateT = now;
	}
	Update();
	return true;
}

void MyPvpRecord::Save( KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged )
{
	Update();
	if(m_isModified.TestAndReset())
	{
		KKSG::PvpData* data = poRoleAllInfo->mutable_pvpdata();
		roChanged.insert(data);
		data->Clear();

		data->set_joincounttodayint(m_joinCountTodayInT);
		data->set_wincountthisweek(m_winCountThisWeek);
		data->set_wincountall(m_winCountAll);
		data->set_losecountall(m_loseCountAll);
		data->set_drawcountall(m_drawCountAll);

		data->set_weekrewardhaveget(m_weekRewardHaveGet);

		data->set_lastdayupt((UINT32)(m_lastDayUpdateT));
		data->set_lastweekupt((UINT32)(m_lastWeekUpdateT));

		data->set_todayplaytime(m_todayplaytime);
		data->set_todayplaytimes(m_todayplaytimes);

		for(auto it = m_pvpRecs.begin(); it != m_pvpRecs.end(); ++it)
		{
			data->add_pvprecs()->CopyFrom((*it));
		}
	}
}

void MyPvpRecord::Update()
{
	time_t now = GameTime::GetTime();
	if(!XCommon::IsDaySame(m_lastDayUpdateT, now))
	{
		DayZero();
		m_lastDayUpdateT = now;
	}
	if(!XCommon::IsWeekSame(m_lastWeekUpdateT, now))
	{
		WeekZero();
		m_lastWeekUpdateT = now;
	}
}

void MyPvpRecord::DayZero()
{
	if (m_todayplaytime)
	{
		ReportDataMgr::Instance()->PvpTime(m_poRole, m_todayplaytime);
		m_todayplaytime = 0;
		m_todayplaytimes = 0;
	}
	m_joinCountTodayInT = 0;
	m_isModified.Set();
}

void MyPvpRecord::WeekZero()
{
	m_winCountThisWeek = 0;
	m_weekRewardHaveGet = false;
	m_isModified.Set();
}

void MyPvpRecord::AddOneRec( KKSG::PvpOneRec& rec )
{
	Update();
	if(PvpMgr::Instance()->IsInOpenTime())
	{
		++m_joinCountTodayInT;	
	}
	if(rec.wincount() > rec.losecount())
	{
		++m_winCountThisWeek;	
		++m_winCountAll;
		
		std::set<UINT64> enemyIds;
		for (int i = 0; i < rec.opside_size(); ++i)
		{
			enemyIds.insert(rec.opside(i).roleid());
		}
		bool sameEnemy = false;
		UINT32 num = 0;
		for (auto it = m_pvpRecs.rbegin(); it != m_pvpRecs.rend(); ++it)
		{
			if (num >= GetGlobalConfig().MilitarySameOpponentRound)
			{
				break;
			}
			if ((*it).wincount() <= (*it).losecount())
			{
				continue;
			}
			++num;
			for (int i = 0; i < (*it).opside_size(); ++i)
			{
				UINT64 roleId = (*it).opside(i).roleid();
				if (enemyIds.find(roleId) != enemyIds.end())
				{
					sameEnemy = true;
					break;;
				}
			}
			if (sameEnemy)
			{
				break;
			}
		}
		if (sameEnemy == false)
		{
			UINT32 v = m_poRole->Get<MilitaryRankRecord>()->AddBattleRecord(KKSG::SCENE_PVP);
			rec.set_military(v);
		}
		else
		{
			PtcG2C_GSErrorNotify oPtc;
			oPtc.m_Data.set_errorno(KKSG::ERR_MILITARY_TRIGGER_CHECK);
			oPtc.m_Data.set_istip(true);
			m_poRole->Send(oPtc);
		}
	}
	else if(rec.wincount() < rec.losecount())
	{
		++m_loseCountAll;
		UINT32 v = m_poRole->Get<MilitaryRankRecord>()->ReduceBattleRecord(KKSG::SCENE_PVP);
		rec.set_military(v);
		 
	}
	else
	{
		++m_drawCountAll;
	}
	int maxcount = 10;//ÅäÖÃ
	m_pvpRecs.push_back(rec);
	if((int)m_pvpRecs.size() > maxcount && !m_pvpRecs.empty())
	{
		m_pvpRecs.erase(m_pvpRecs.begin());
	}
	m_isModified.Set();
}

void MyPvpRecord::FillAllCountClient( KKSG::PvpBaseData* data )
{
	Update();
	if(NULL == data)
	{
		return ;
	}
	data->set_wincountall(m_winCountAll);
	data->set_losecountall(m_loseCountAll);
	data->set_drawcountall(m_drawCountAll);
	data->set_wincountthisweek(m_winCountThisWeek);
	data->set_wincountweekmax(GetGlobalConfig().PVPWeekReqCount);
	data->set_jointodayintime(m_joinCountTodayInT);
	data->set_jointodayintimemax(GetGlobalConfig().PVPDayReqCount);
	data->set_weekrewardhaveget(m_weekRewardHaveGet);
}

void MyPvpRecord::FillMyRecsClient( KKSG::PvpHistory* data )
{
	Update();
	if(NULL == data)
	{
		return ;
	}
	for(auto it = m_pvpRecs.begin(); it != m_pvpRecs.end(); ++it)
	{
		data->add_recs()->CopyFrom(*it);
	}
}

void MyPvpRecord::TestPrint()
{
	Update();
	SSWarn << "join today intime = " << m_joinCountTodayInT << END;
	SSWarn << "win count this week = " << m_winCountThisWeek << END;
	SSWarn << "win count all = " << m_winCountAll << END;
	SSWarn << "lose count all = " << m_loseCountAll << END;
	SSWarn << "draw count all = " << m_drawCountAll << END;
	SSWarn << "last day update time = " << m_lastDayUpdateT << END;
	SSWarn << "last week update time = " << m_lastWeekUpdateT << END;
	SSWarn << "week reward have get = " << (int)(m_weekRewardHaveGet) << END;
	SSWarn << "rec count = " << m_pvpRecs.size() << END;
	for(auto it = m_pvpRecs.begin(); it != m_pvpRecs.end(); ++it)
	{
		auto& data = (*it);
		SSWarn << " win = " << data.wincount() << " lose = " << data.losecount() << " draw = " << data.drawcount() << " mvp = " << data.mvpid() << END;
		for(int i = 0; i < data.myside_size(); ++i)
		{
			SSWarn << " myside = " << data.myside(i).rolename() << END;
		}
		for(int i = 0; i < data.opside_size(); ++i)
		{
			SSWarn << " opside = " << data.opside(i).rolename() << END;
		}
	}
}

void MyPvpRecord::TestClear()
{
	m_joinCountTodayInT = 0;
	m_winCountThisWeek = 0;
	m_winCountAll = 0;
	m_loseCountAll = 0;
	m_drawCountAll = 0;

	m_lastDayUpdateT = time(NULL);
	m_lastWeekUpdateT = time(NULL);

	m_pvpRecs.clear();

	m_isModified.Set();
}

void MyPvpRecord::SetWeekRewardHaveGet()
{
	m_weekRewardHaveGet = true;
	m_isModified.Set();
}

bool MyPvpRecord::GetWeekRewardHaveGet()
{
	Update();
	return m_weekRewardHaveGet;
}

int MyPvpRecord::GetWinCountThisWeek()
{
	Update();
	return m_winCountThisWeek;
}

int MyPvpRecord::GetJoinCountTodayInT()
{
	Update();
	return m_joinCountTodayInT;
}

int MyPvpRecord::GetWinCountAll()
{
	Update();
	return m_winCountAll;
}

int MyPvpRecord::GetLoseCountAll()
{
	Update();
	return m_loseCountAll;
}

int MyPvpRecord::GetDrawCountAll()
{
	Update();
	return m_drawCountAll;
}

int MyPvpRecord::GetCountAll()
{
	Update();
	return m_winCountAll + m_loseCountAll + m_drawCountAll;
}

int MyPvpRecord::GetMatchScore()
{
	Update();

	/*
	SSWarn << __FUNCTION__ << END;
	SSWarn << m_winCountAll << " " <<  GetGlobalConfig().PVPMatchScore[0] << END; 
	SSWarn << m_loseCountAll << " " <<  GetGlobalConfig().PVPMatchScore[1] << END;
	SSWarn << m_drawCountAll << " " <<  GetGlobalConfig().PVPMatchScore[2] << END;
	*/
	return (m_winCountAll *  GetGlobalConfig().PVPMatchScore[0] + 
		m_loseCountAll *  GetGlobalConfig().PVPMatchScore[1] + 
		m_drawCountAll *  GetGlobalConfig().PVPMatchScore[2]);
}

int MyPvpRecord::LeftJoinCountTodayInT()
{
	int ret = GetGlobalConfig().PVPDayReqCount - GetJoinCountTodayInT();	
	/*
	if(ret < 0)
	{
		ret = 0;
	}
	*/
	return ret;
}

void MyPvpRecord::AddPlayTime(UINT32 iTime)
{
	Update();
	m_todayplaytime += iTime;
	m_todayplaytimes ++;
	m_isModified.Set();
}
