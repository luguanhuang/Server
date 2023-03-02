#ifndef __MAYHEMMSMGR_H__
#define __MAYHEMMSMGR_H__
class CRole;



class MayhemMsMgr : ITimer
{
	MayhemMsMgr();
	~MayhemMsMgr();
	DECLARE_SINGLETON(MayhemMsMgr)
public:
	bool Init();
	void CloseTimerHandle();
	void Uninit();
	void PartCutScene();
	bool LoadFile();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void OnLogin(CRole *pRole);
	void DelRole(UINT64 nRole);
	void SetReadyScene(const KKSG::BMCrossNtfData &data);
	const KKSG::BMCrossNtfData& GetBMCrossNtfData();

	void SendIcon(UINT32 nSysID,KKSG::HallIconState state);
	void OnStart();
	bool ShowStartIcon(UINT32 nOpen);
	bool ShowAwardIcon();
	bool IsFightMap(UINT32 nMapID);
	void TestTask();
	void OnMapChange(CRole *pRole,UINT32 oldMapID, UINT32 newMapID);
	UINT32 GetStep() { return m_nStep;}

	void AddRoleID(UINT64 nRoleID);
	void AddCutScene(const KKSG::EnterBMFightScene& data);
	void EnterScene(const KKSG::EnterBMFightScene& data);
	bool IsCanJoin();
private:
	void SetTimeEvent();
	bool PreNoticeByHour(); 
	bool PreNoticeBy30Minute(); 
	bool PreNoticeBy20Minute(); 
	bool PreNoticeBy10Minute(); 
	bool PreNoticeBy3Minute(); 
	bool PreNoticeBy2Minute(); 
	bool PreNoticeBy1Minute(); 

	bool PreNoticeBy3Start();
	bool PreNoticeBy2Start();
	bool PreNoticeBy1Start();

	bool NoticeStart();

	void ClearNoticeTimeInfo();
	
	bool CanOpenActivity();

private:
	std::list<KKSG::EnterBMFightScene> m_listBigMelee;
	KKSG::BMCrossNtfData m_Data;
	HTIMER time_handler_;
	bool event_flag_;
	std::unordered_set<UINT32> timer_id_;
	UINT32 m_hStartIcon;
	UINT32 m_nStep;
	std::set<UINT64> m_setRoleID;
	UINT32 m_nDisIconTime;
	UINT32 m_nJoinPlayers;
	UINT32 m_nMaxPlayers;

};

#endif