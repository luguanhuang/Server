#ifndef __MOBARECORDMGR_H__
#define __MOBARECORDMGR_H__

#include "util/utilsingleton.h"
#include "util/uniquequeue.h"
#include "mobarecord.h"



class CRole;
class MobaRecordMgr : public Singleton<MobaRecordMgr>, public ITimer
{
public:
	MobaRecordMgr();
	~MobaRecordMgr();

	bool Init();
	void Uninit();

	void OnTimer(UINT32 dwID, UINT32 dwCount);

	void OnLogin(UINT64 roleid);	

	///> 上线的大概率不为空(要等db返回)
	MobaRecord* GetMobaRoleRecord(UINT64 roleid);

	void AddMobaBattleRecord(const KKSG::AddMobaBattleRecordData& addData);

	void AddChangedToQueue(UINT64 roleid);

	void SaveAllToDB();

	void OnGetMobaRecordFromDB(UINT64 roleid, const KKSG::QueryMobaRoleRecordRes& res);	

private:
	void _QueryMobaRecordFromDB(UINT64 roleid);
	void _SaveToDB(UINT32 maxNum);

private:
	HTIMER m_timeHandler;
	std::unordered_map<UINT64, MobaRecord> m_recordCache;

	UniqueQueue<UINT64> m_isChangedQueue;

	std::unordered_set<UINT64> m_lockQueryRole;

	std::unordered_map<UINT64, std::vector<KKSG::AddMobaBattleRecordData>> m_waitAddRecord;

	time_t m_lastDestroyTime;
};




#endif