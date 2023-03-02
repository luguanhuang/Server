#ifndef __BATTLEFIELD_MS_MGR_H__
#define __BATTLEFIELD_MS_MGR_H__
class CRole;



class BattleFieldMgr : ITimer
{
	BattleFieldMgr();
	~BattleFieldMgr();
	DECLARE_SINGLETON(BattleFieldMgr)
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void OnLogin(CRole *pRole);
	bool IsLevelOk(CRole *pRole);
	void OnMapChange(CRole *pRole,UINT32 oldMapID, UINT32 newMapID);
	bool IsFightMap(UINT32 nMapID);
	void TestTask(UINT32 nPlayer);
	void ReportRoleInfo(CRole *pRole,KKSG::BattleFieldType type);

	void OnStop();
	bool CreateSceneReadyBegin();
	void CreateSceneWaitEnd(UINT32 nSceneType, UINT32 nLineID, UINT32 nSceneID);
	void GoBackReadyScene(CRole *pRole);
	bool EnterReadyScene(CRole *pRole);
	void OnStart();
	void EnterScene(const KKSG::EnterBFFightBatchScene& data);
	void SendReadySceneInfo(const KKSG::BattleFieldReadyScene &refInfo);
	void SendMatchFail(const KKSG::BattleFailInfo &refInfo);
private:
	UINT32 m_nReadyLine;
	UINT32 m_nReadySceneID;
	UINT32 m_nRound;
	void SendIcon(CRole* pRole, UINT32 sysid, KKSG::HallIconState state);
	void SendIconToAll(UINT32 sysid, KKSG::HallIconState state);

	void LoadTimer();
	void ClearTimer();
	bool PreNoticeStart(INT32 minute);
	HTIMER time_handler_;
	UINT32 m_LastUpdateTime;
	std::unordered_set<UINT32> timer_id_;
};

#endif