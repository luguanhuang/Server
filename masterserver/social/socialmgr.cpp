#include "pch.h"
#include "socialmgr.h"
#include "social.h"
#include "timermgr.h"
#include "scene/sceneconfig.h"
#include "foreach.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util/timespecificmgr.h"
#include "social/social.h"
#include "network/dblink.h"
#include "role/rolemanager.h"
#include "social/ptcm2d_mssavesocialinfontf.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/flowerranklist.h"
#include "util/gametime.h"
#include "event/eventmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "role/rolesummarymgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "loghelper/tlogr.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "define/systemiddef.h"

#define UPDATE_INTERVAL 60*1000
#define PARTCOUNT 10

INSTANCE_SINGLETON(SocialMgr);

SocialMgr::SocialMgr()
{
	m_isloaded = false;
}

SocialMgr::~SocialMgr()
{
	for (auto i = m_roleid2socialinfo.begin(); i != m_roleid2socialinfo.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
}

bool SocialMgr::Init()
{
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, UPDATE_INTERVAL, -1, __FILE__, __LINE__);

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);

	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&SocialMgr::OnDayTimer, this));

	if (!QueryDb())
	{
		return false;
	}
	return true;
}

void SocialMgr::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

void SocialMgr::OnLogin(CRole* role)
{
	if (!role)
	{
		return;
	}

	SocialInfo* info = GetSocialInfo(role->GetID());
	if (!info)
	{
		return;
	}

	if(info->flower.CheckGetFlowerActivityReward())
	{
		PtcM2C_HintNotifyMS ntf;
		ntf.m_Data.add_systemid(SysFlowerActiviyId);
		role->Send(ntf);
	}
}

bool SocialMgr::QueryDb()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from social";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table social failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		KKSG::SocialInfo2Db dbinfo;

		dbinfo.set_roleid(qwRoleID);

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'flower' column failed");
			return false;
		}
		dbinfo.mutable_flower()->assign(strData);


		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'gaojigu' column failed");
			return false;
		}
		dbinfo.mutable_gaojigu()->assign(strData);


		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'tshow' column failed");
			return false;
		}
		dbinfo.mutable_tshow()->assign(strData);

		
		LoadFromDb(&dbinfo);
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all social data from db use time: %llu ms", qwTimeUsed);

	return true;
}

void SocialMgr::LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db)
{
	UINT64 roleid = socialinfo2db->roleid();

	SocialInfo* info = new SocialInfo(roleid);
	info->Db2SocialInfo(socialinfo2db);
	m_roleid2socialinfo[roleid] = info;
}

void SocialMgr::Save2Db()
{
	PtcM2D_MSSaveSocialInfoNtf ptc;
	int count = 1;
	for (auto i = m_changeds.begin(); i != m_changeds.end(); ++i)
	{
		UINT64 roleid = *i;
		auto j = m_roleid2socialinfo.find(roleid);
		if (j == m_roleid2socialinfo.end())
		{
			continue;
		}
		KKSG::SocialInfo2Db* socialinfo2db = ptc.m_Data.add_socialinfo();
		socialinfo2db->set_roleid(*i);
		(*j->second).SocialInfo2Db(socialinfo2db);

		if (0 == count % PARTCOUNT)
		{
			DBLink::Instance()->SendTo(ptc);
			ptc.m_Data.Clear();
		}
		++count;
	}
	DBLink::Instance()->SendTo(ptc);
	m_changeds.clear();

	SSInfo<<"save socialinfo to db, count:"<<count - 1<<END;
}

void SocialMgr::DebugDayPass()
{
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
	if (list)
	{
		list->Clear();
	}
	list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerRank);
	if (list)
	{
		list->Clear();
	}
	for (auto iter = m_roleid2socialinfo.begin(); iter != m_roleid2socialinfo.end(); ++iter)
	{
		iter->second->flower.DebugDayPass();
	}
}

void SocialMgr::DebugWeekPass()
{
	FlowerRankList* ranklist = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerThisWeekRank);
	ranklist->SendThisWeekRankReward();
	ranklist->Clear();
	ranklist = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerRank);
	ranklist->Clear();

	for (auto iter = m_roleid2socialinfo.begin(); iter != m_roleid2socialinfo.end(); ++iter)
	{
		iter->second->flower.DebugWeekPass();
	}
}

void SocialMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (!m_changeds.empty())
	{
		Save2Db();
	}
}

void SocialMgr::StartUpFlowerEvent()
{
	std::string szEvent =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eSocialMgr);
	UINT32 nEventTime   =  0; 
	if(!szEvent.empty())
		nEventTime = convert<UINT32>(szEvent);
	UINT32 nTime      = GameTime::GetTime();
	if (nEventTime&&!XCommon::IsDaySame(GameTime::GetTime(), nEventTime))
	{
		LogInfo("FLOWER EVENT");
		OnFlowerEvent(nTime);
	}
}

void SocialMgr::OnFlowerEvent(UINT32 nTime)
{
	FlowerRankList* rlist = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
	std::vector<FlowerRankInfo*> rrank;
	rlist->GetSomeTopInfo(10, rrank);
	int nRank = 0;
	int nReal = 0;
	LogDebug("SocialMgr::OnFlowerEvent()====yrank size===%d", rrank.size());
	for (auto iter = rrank.begin(); iter!=rrank.end(); iter++)
	{
		nRank++;
		FlowerRankInfo* pInfo = *iter;
		if (nRank<=3)
			nReal = nRank;
		else
			nReal = 10;
		EventMgr::Instance()->AddEvent(pInfo->GetUniqueId(), DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY,nReal);
		DoFlowerRank(pInfo->GetUniqueId(), KKSG::FlowerYesterdayRank, nRank);
	}
	rrank.clear();
	nRank = 0;
	rlist = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerTotalRank);
	rlist->GetSomeTopInfo(10, rrank);
	LogDebug("SocialMgr::OnFlowerEvent()====hrank size===%d", rrank.size());
	for (auto iter = rrank.begin(); iter!=rrank.end(); iter++)
	{
		nRank++;
		FlowerRankInfo* pInfo = *iter;
		if (nRank<=3)
			nReal = nRank;
		else
			nReal = 10;
		EventMgr::Instance()->AddEvent(pInfo->GetUniqueId(), DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY,nReal);
		DoFlowerRank(pInfo->GetUniqueId(), KKSG::FlowerTotalRank, nRank);
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eSocialMgr,ToString(GameTime::GetTime()));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eSocialMgr);
}

bool SocialMgr::OnDayTimer()
{
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
	if (list)
	{
		list->Clear();
	}
	foreach(i in m_roleid2socialinfo)
	{
		(*i).second->tshow.Update();
		(*i).second->flower.OnDayPass();
	}
	OnFlowerEvent();
	CRankListMgr::Instance()->SendTShowTop3Rank();
	return true;
}


void SocialMgr::UpdateFlowerActivityInfo()
{
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerActivityRank);
	if (list)
	{
		list->Clear();
	}

	foreach (i in m_roleid2socialinfo)
	{
		i->second->flower.CheckUpdateFlowerActivity();
	}

}

void SocialMgr::ClearFlowerActivityInfo()
{
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerActivityRank);
	if (list)
	{
		list->Clear();
	}
	foreach (i in m_roleid2socialinfo)
	{
		i->second->flower.ClearActivityFlowerInfo();
	}
}

SocialInfo* SocialMgr::GetSocialInfo(UINT64 roleid)
{
	SocialInfo* info = NULL;
	auto i = m_roleid2socialinfo.find(roleid);
	if (i == m_roleid2socialinfo.end())
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
		if (!summary)
		{
			return NULL;
		}
		info = new SocialInfo(roleid);
		m_roleid2socialinfo[roleid] = info;
	}
	else
	{
		info = i->second;
	}
	return info;
}

void SocialMgr::DoFlowerRank(UINT64 roleid, KKSG::RankeType type, UINT32 rank)
{
	SocialInfo*  info = GetSocialInfo(roleid);
	if (info)
	{
		UINT32 num = 0;
		if (type == KKSG::FlowerYesterdayRank)
		{
			num = info->flower.GetYestodayReceiveCount();
		}
		else if (type == KKSG::FlowerThisWeekRank)
		{
			num = info->flower.GetThisWeekReceiveCount();
		}
		else if (type == KKSG::FlowerTotalRank)
		{
			num = info->flower.GetTotalReceiveCount();
		}
		else
		{
			return;
		}
		TFlowerRankFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_Num = num;
		oLog.m_Type = type;
		oLog.m_uRoleID = roleid;
		oLog.m_Rank = rank;
		oLog.Do();
	}
}
