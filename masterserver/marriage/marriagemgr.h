#ifndef __MARRIAGEMGR_H__
#define __MARRIAGEMGR_H__

#include "timer.h"
#include "mentorship/mentormgr.h"

#define MARRIAGEDIVORCETIME 86400

class Marriage;
class CRole;

namespace KKSG
{
	class MarriageAllData2DB;
}

class MarriageMgr : public ITimer
{
public:
	MarriageMgr();
	~MarriageMgr();
	DECLARE_SINGLETON(MarriageMgr)

public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Init();
	void Uninit();
	void OnLogin(CRole* role);
	void OnLogout(CRole* role);
	void OnEnterHall(CRole* role);

	bool LoadFromDb();
	void Save2Db(bool isAll=true);
	void QuickSave(Marriage* pOneSide);

	bool OnDayPass();
	void OnWeekPass();

	void AddChanged(UINT64 roleID);

	void ChangeDesName(UINT64 roleID, const std::string &name);
	Marriage* GetMarriage(UINT64 roleID, bool force=true);

	void AutoBreak(Marriage* pMarriage1, Marriage* pMarriage2);
	void AddAutoBreak(BreakApplyPair& pair, int time);
	void RemoveAutoBreak(BreakApplyPair& pair);

	void CheckMarriageActivity(UINT32 endTime);

private:
	bool _ParseFromDb(UINT64 roleID, const KKSG::MarriageAllData2DB& data);

private:
	std::map<UINT64, Marriage*> m_marriageList;
	std::set<UINT64> m_changedList;

	std::map<BreakApplyPair, int> m_autoBreakList;
	HTIMER m_timer;
	HTIMER m_dayTimer;
};

#endif // 
