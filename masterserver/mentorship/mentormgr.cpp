#include "pch.h"
#include <time.h>
#include "mentormgr.h"
#include "mentoroneside.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "mentorrelationmgr.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "util.h"
#include "mentormsg.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "mentorship/ptcm2d_savementorinfontf.h"
#include "network/dblink.h"
#include "mentorrelationlist.h"
#include "foreach.h"

INSTANCE_SINGLETON(MentorMgr)

#define UPDATE_INTERVAL 71*1000

MentorMgr::MentorMgr()
{

}

MentorMgr::~MentorMgr()
{
	foreach(i in m_mentorshipList)
	{
		delete i->second;
	}
	m_mentorshipList.clear();
	m_changedList.clear();
}

void MentorMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	int now = GameTime::GetTime();
	for (auto iter = m_autoBreakList.begin(); iter != m_autoBreakList.end();)
	{
		if (now - iter->second >= MENTORAUTOBREAKTIME)
		{
			MentorRelationMgr::Instance()->AutoBreak(GetMentorShip(iter->first.m_srcID), GetMentorShip(iter->first.m_destID));
			m_autoBreakList.erase(iter++);
		}
		else
			++iter;
	}

	Save2Db();
}

bool MentorMgr::Init()
{
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, UPDATE_INTERVAL, -1, __FILE__, __LINE__);

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_dayTimer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&MentorMgr::OnDayPass, this));

	LoadFromDb();

	return true;
}

void MentorMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timer);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_dayTimer);
}

bool MentorMgr::OnDayPass()
{
	for (auto iter = m_mentorshipList.begin(); iter != m_mentorshipList.end(); ++iter)
	{
		iter->second->GetMentorMsg()->OnDayPass();
	}
	return true;
}

void MentorMgr::OnLogin(UINT64 roleID)
{
	MentorShipOneSide* pOneSide = GetMentorShip(roleID);
	if (pOneSide)
	{
		//pOneSide->GetMentorMsg()->OnLogin();
		pOneSide->OnRoleLogin();
	}
}

bool MentorMgr::LoadFromDb()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from mentorship";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mentorship failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT32 count = 0;
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

		KKSG::MentorAllData2Db dbinfo;

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
			dbinfo.ParseFromString(strData);
		}
		else
		{
			LogError("Read 'mentorinfo' column failed");
			return false;
		}
		
		_ParseFromDb(qwRoleID, dbinfo);
		++count;
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all mentor data, size:%u from db use time: %llu ms", count,qwTimeUsed);

	return true;
}

void MentorMgr::Save2Db(bool isAll)
{
	for (auto iter = m_changedList.begin(); iter != m_changedList.end(); ++iter)
	{
		MentorShipOneSide* pOneSide = GetMentorShip(*iter);
		if (pOneSide)
		{
			PtcM2D_SaveMentorInfoNtf ntf;
			KKSG::MentorAllData2Db tempData;
			pOneSide->Save(tempData);
			ntf.m_Data.set_roleid(pOneSide->GetOwnerID());
			ntf.m_Data.set_info(tempData.SerializeAsString());
			DBLink::Instance()->SendTo(ntf);
		}
	}
	LogInfo("save mentor size:%u", m_changedList.size());
	m_changedList.clear();
}

void MentorMgr::QuickSave(MentorShipOneSide* pOneSide)
{
	if (!pOneSide)
	{
		return;
	}
	PtcM2D_SaveMentorInfoNtf ntf;
	KKSG::MentorAllData2Db tempData;
	pOneSide->Save(tempData);
	ntf.m_Data.set_roleid(pOneSide->GetOwnerID());
	ntf.m_Data.set_info(tempData.SerializeAsString());
	DBLink::Instance()->SendTo(ntf);

	m_changedList.erase(pOneSide->GetOwnerID());

	LogInfo("quick save mentor size:%llu", pOneSide->GetOwnerID());
}

void MentorMgr::AddChangedMentor(UINT64 roleID)
{
	m_changedList.insert(roleID);
}

MentorShipOneSide* MentorMgr::GetMentorShip(UINT64 roleID, bool force)
{
	auto find = m_mentorshipList.find(roleID);
	if (find == m_mentorshipList.end())
	{
		if (!force)
		{
			auto findSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
			if (!findSummary)
			{
				return NULL;
			}
		}

		auto ptr = new MentorShipOneSide(roleID);
		m_mentorshipList.insert(std::make_pair(roleID, ptr));

		return ptr;
	}

	return find->second;
}

void MentorMgr::AddAutoBreak(BreakApplyPair& pair, int time)
{
	m_autoBreakList.insert(std::make_pair(pair, time));
}

void MentorMgr::RemoveAutoBreak(BreakApplyPair& pair)
{
	m_autoBreakList.erase(pair);
}

KKSG::EMentorRelationPosition MentorMgr::GetMentorPos(UINT64 srcID, UINT64 destID)
{
	MentorShipOneSide* pOneSide = GetMentorShip(srcID);
	if (!pOneSide)
	{
		return KKSG::EMentorPosMax;
	}
	return (KKSG::EMentorRelationPosition)pOneSide->GetRelationList()->GetRolePos(destID);
}

bool MentorMgr::_ParseFromDb(UINT64 roleID, const KKSG::MentorAllData2Db& data)
{
	auto oneside = GetMentorShip(roleID, true);
	if (!oneside)
	{
		return true;
	}

	oneside->Load(data);

	return true;
}

