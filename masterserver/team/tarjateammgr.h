#ifndef __TARJATEAMMGR_H__
#define __TARJATEAMMGR_H__
class CRole;

class TarjaTeamMgr : public ITimer
{
	TarjaTeamMgr();
	~TarjaTeamMgr();
	DECLARE_SINGLETON(TarjaTeamMgr)

public:
	bool Init();
	void Uninit();
	void SyncStatus(CRole *pRole);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void SetTime(UINT64 nRoleID,UINT32 nTime);
	void OnLogin(CRole *pRole);
	UINT32 GetTime(UINT64 nRole);
private:
	std::map<UINT64,UINT32> m_mapRole;
	HTIMER m_Handler;
};


#endif