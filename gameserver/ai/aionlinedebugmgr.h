#ifndef _AIONLINEDEBUGMGR_H_ 
#define _AIONLINEDEBUGMGR_H_

#include <unordered_map>
#include <string>
#include <vector>
#include <tinyxml2.h>
#include "unit/unit.h"

class AINodeBase;
class Unit;

class AIDebugHelper
{
public:
	AIDebugHelper(AINodeBase* pAINode, Unit* pUnit);
	~AIDebugHelper();

	inline bool ShouldLog() { return m_bShouldLog; }

	///> 把日志发给客户端
	void TryReport();
private:
	///> 把日志记录进来
	void Finish();

	AINodeBase* m_pAINode;
	Unit* m_pUnit;
	bool m_bShouldLog;
};

class AIDebugInfo
{
public:
	UINT64 uid;
	std::list<UINT64> observers;
	std::vector<string> infos;

public:
	void ClearInfos() { infos.clear(); }
	void AddObserver(UINT64 observer) { observers.push_back(observer); }
	size_t RemoveObserver(UINT64 observer)
	{
		observers.remove(observer);
		return observers.size();
	}
	void AppendInfo(const string& str) { infos.push_back(str); }
};

class AIOnlineDebugMgr
{
	AIOnlineDebugMgr();
	~AIOnlineDebugMgr();
	DECLARE_SINGLETON(AIOnlineDebugMgr)
public:
	bool Init(){return true;};
	void Uninit(){};
	void Update(){};

public:
	void AddDebugUnit(Unit* unit);
	void RemoveDebugUnit(Unit* unit);
	void SendDebugMsg(UINT32 sceneid, string msg);

	void AddWatch(Unit* pTarget, Unit* pObserver);
	void AddWatch(Unit* pTarget);
	void AddWatch(UINT64 uid);
	void RemoveWatch(UINT64 uid);
	void RemoveObserver(UINT64 uid);
	bool IsWatched(UINT64 uid);
	void AppendInfo(UINT64 uid, const string& info);
	void SendMsg(Unit* pUnit);
	void SetDebugAll(UINT64 uid, bool bDebugAll);

	bool IsDebugAll() { return !mDebugAll.empty();}
private:
	std::unordered_map<UINT32, std::unordered_map<UINT64, Unit*>> mMsgUnit;

	std::unordered_map<UINT64, AIDebugInfo> mWatchUnits;
	std::unordered_map<UINT64, UINT64> mObservers;
	std::unordered_set<UINT64> mDebugAll;
};

#endif