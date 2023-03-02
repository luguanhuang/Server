#ifndef __CUSTOMBATTLEROLEMGR_H__
#define __CUSTOMBATTLEROLEMGR_H__

#include "timer.h"
#include "custombattle/custombattlerole.h"

class CustomBattleRoleMgr : public ITimer
{
	CustomBattleRoleMgr();
	~CustomBattleRoleMgr();
	DECLARE_SINGLETON(CustomBattleRoleMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddInfo(UINT64 roleid, SCustomBattleRole* info);
	SCustomBattleRole* GetInfo(UINT64 roleid);
	SCustomBattleRole* TryGetInfo(UINT64 roleid);

	void SetEnd() { m_isend = true; }

private:
	bool m_isend;
	HTIMER m_timehandle;
	std::unordered_map<UINT64, SCustomBattleRole*> m_roleid2info;
};
#endif
