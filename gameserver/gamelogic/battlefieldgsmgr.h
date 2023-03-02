#ifndef __BATTLEFIELD_GS_MGR_H__
#define __BATTLEFIELD_GS_MGR_H__

#include "table/BattleFieldPointReward.h"
class Role;


class BattleFieldGSMgr: public ITimer
{
	BattleFieldGSMgr();
	~BattleFieldGSMgr();
	DECLARE_SINGLETON(BattleFieldGSMgr);
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	void ClearFile();
	bool CheckFile();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void SetTime(UINT32 nTime);
	void SetSceneID(UINT32 nSceneID);
	UINT32 GetTime();
	void Start();
	void ResetTime(UINT32 nRound,UINT32 nNextMatchTime,bool bFlag,bool bEnd);
	void OnEnterScene(Role* role);
	void SyncTime();
	BattleFieldPointReward * GetTable()
	{
		return &m_Table;
	}
	UINT32 GetRound() 
	{
		return m_nRound;
	}
private:
	BattleFieldPointReward m_Table;
	UINT32 m_nTime;
	UINT32 m_nRound;
	UINT32 m_nSceneID;
	HTIMER m_timerId;
	bool m_bEnd;

};
#endif
