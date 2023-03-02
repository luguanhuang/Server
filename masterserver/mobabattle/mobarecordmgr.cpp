#include "pch.h"
#include "mobarecordmgr.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "mobabattle/ptcm2d_savemobarolerecord.h"
#include "network/dblink.h"
#include "mobabattle/rpcm2d_querymobarolerecord.h"


#define DELAYTIME 3600*2

MobaRecordMgr::MobaRecordMgr()
{
	m_timeHandler = INVALID_HTIMER;
	m_lastDestroyTime = 0;
}

MobaRecordMgr::~MobaRecordMgr()
{

}

bool MobaRecordMgr::Init()
{
	m_timeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 10*1000, -1, __FILE__, __LINE__);
	if (m_timeHandler == INVALID_HTIMER)
	{
		return false;
	}
	return true;
}

void MobaRecordMgr::Uninit()
{
	if (m_timeHandler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timeHandler);
	}
}

void MobaRecordMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	///> 每10秒更新4个
	_SaveToDB(4);
	///> 每一个小时清一次cache
	time_t nowTime = time(NULL);
	if (m_lastDestroyTime + 3600 <= nowTime)
	{
		m_lastDestroyTime = nowTime;
		for (auto i = m_recordCache.begin(); i != m_recordCache.end();)
		{
			if (!CRoleManager::Instance()->IsOnline(i->first) &&
				!m_isChangedQueue.has(i->first))
			{
				m_recordCache.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}
}

void MobaRecordMgr::OnLogin(UINT64 roleid)
{
	auto iter = m_recordCache.find(roleid);
	if (iter == m_recordCache.end())
	{
		_QueryMobaRecordFromDB(roleid);
	}
}

MobaRecord* MobaRecordMgr::GetMobaRoleRecord(UINT64 roleid)
{
	auto iter = m_recordCache.find(roleid);
	if (iter != m_recordCache.end())
	{
		return &iter->second;
	}
	if (CRoleSummaryMgr::Instance()->GetRoleSummary(roleid) != NULL)
	{
		_QueryMobaRecordFromDB(roleid);
	}
	return NULL;
}

void MobaRecordMgr::AddMobaBattleRecord(const KKSG::AddMobaBattleRecordData& addData)
{
	std::vector<UINT64> roleids;
	for (int i = 0; i < addData.game().team1_size(); ++i)
	{
		roleids.push_back(addData.game().team1(i).roleid());
	}
	for (int i = 0; i < addData.game().team2_size(); ++i)
	{
		roleids.push_back(addData.game().team2(i).roleid());
	}
	for (auto i = roleids.begin(); i != roleids.end(); ++i)
	{
		if (CRoleSummaryMgr::Instance()->GetRoleSummary(*i) == NULL)
		{
			continue;
		}
		auto iter = m_recordCache.find(*i);
		if (iter == m_recordCache.end())
		{
			_QueryMobaRecordFromDB(*i);
			m_waitAddRecord[*i].push_back(addData);
			continue;
		}
		iter->second.AddGameRecord(addData);
	}
}

void MobaRecordMgr::AddChangedToQueue(UINT64 roleid)
{
	if (m_recordCache.find(roleid) == m_recordCache.end())
	{
		return;
	}
	m_isChangedQueue.push(roleid);
}

void MobaRecordMgr::SaveAllToDB()
{
	_SaveToDB(-1);
}

void MobaRecordMgr::OnGetMobaRecordFromDB(UINT64 roleid, const KKSG::QueryMobaRoleRecordRes& res)
{
	if (m_recordCache.find(roleid) != m_recordCache.end())
	{
		LogError("role [%llu], it shouldn't happen, record cache may be add in other place", roleid);
		return;
	}
	// unlock
	m_lockQueryRole.erase(roleid);

	if (res.errorcode() != KKSG::ERR_SUCCESS)
	{
		LogWarn("role [%llu], GetMobaRecordFromDB not succeed!!!, errorcode [%d]", roleid, res.errorcode());
		if (res.errorcode() == KKSG::ERR_TIMEOUT || res.errorcode() == KKSG::ERR_FAILED)
		{
			_QueryMobaRecordFromDB(roleid);
			return;
		}
		LogError("role [%llu], GetMobaRecordFromDB, unknown error, record reset!!", roleid);
	}

	m_recordCache.insert(std::make_pair(roleid, MobaRecord(roleid)));
	if (!res.has_record())
	{
		return;
	}

	MobaRecord& roleRecord = m_recordCache[roleid];
	roleRecord.ToThis(res.record());

	auto iter = m_waitAddRecord.find(roleid);
	if (iter != m_waitAddRecord.end())
	{
		for (auto i = iter->second.begin(); i != iter->second.end(); ++i)
		{
			roleRecord.AddGameRecord(*i);
		}
		m_waitAddRecord.erase(roleid);
	}
}

void MobaRecordMgr::_QueryMobaRecordFromDB(UINT64 roleid)
{
	if (m_recordCache.find(roleid) != m_recordCache.end())
	{
		return;
	}
	if (m_lockQueryRole.find(roleid) != m_lockQueryRole.end())
	{
		return;
	}
	// lock
	m_lockQueryRole.insert(roleid);
	RpcM2D_QueryMobaRoleRecord* rpc = RpcM2D_QueryMobaRoleRecord::CreateRpc();
	rpc->m_oArg.set_roleid(roleid);
	DBLink::Instance()->SendTo(*rpc);
}

void MobaRecordMgr::_SaveToDB(UINT32 maxNum)
{
	if (m_isChangedQueue.empty()) return;
	PtcM2D_SaveMobaRoleRecord oPtc;
	UINT32 count = 0;
	while (!m_isChangedQueue.empty() && count < maxNum)
	{
		UINT64 roleid = m_isChangedQueue.front();
		m_isChangedQueue.pop();
		auto iter = m_recordCache.find(roleid);
		if (iter == m_recordCache.end())
		{
			LogError("role [%llu] need to save to db, but not in cache!!!", roleid);
			continue;
		}
		++count;
		oPtc.m_Data.set_roleid(roleid);
		iter->second.ToKKSG(*oPtc.m_Data.mutable_record());
		DBLink::Instance()->SendTo(oPtc);
	}
}