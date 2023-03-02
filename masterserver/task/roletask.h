#ifndef _ROLE_TASK_H_
#define _ROLE_TASK_H_


struct DailyTaskRefreshAskInfo
{
	DailyTaskRefreshAskInfo(): m_roleId(0), m_time(0) {}

	UINT64 m_roleId;
	UINT32 m_time;

	void FillInfo(KKSG::DailyTaskRefreshRoleInfo& info);
	void FromKKSG(const KKSG::DailyTaskRefreshRoleInfo& info);
	void ToKKSG(KKSG::DailyTaskRefreshRoleInfo& info);
};

struct DailyTaskRefreshRecord
{
	DailyTaskRefreshRecord(): m_roleId(0), m_score(0), m_oldScore(0), m_time(0), m_isNew(true) {};

	UINT64 m_roleId;
	UINT32 m_score;
	UINT32 m_oldScore;
	UINT32 m_time;
	bool m_isNew;

	void FromKKSG(const KKSG::DailyTaskRefreshInfo& info);
	void ToKKSG(KKSG::DailyTaskRefreshInfo& info);
};


class CRole;
class CRoleTask
{
public:
	CRoleTask();

	void FromKKSG(const KKSG::RoleTaskSaveData& data);
	void ToKKSG(KKSG::RoleTaskSaveData& data);
	void UpdateToDB();

	void OnDayPass();
	void OnRoleLogin(CRole* pRole);
	void OnNewAccept();

	bool IsAsked(UINT64 roleId);
	bool CanBeRefresh();
	bool CanShowInOtherHelp();

	KKSG::ErrorCode AskHelp(UINT64 roleId);
	KKSG::ErrorCode RefuseHelp(UINT64 roleId);
	KKSG::ErrorCode Refresh(CRole* pRole, UINT64 roleId);
	KKSG::ErrorCode RefreshByRole(CRole* pRole, UINT32 luck = 0);
	void AddBeAskHelpInfo(UINT64 roleId);

	UINT32 GetRemainFreeRefreshNum();
	UINT32 GetRemainHelpNum();
	UINT32 GetCanRefreshNum();

	void SetDirty(bool dirty = true);

	void UpdateLuckToGS(CRole* pRole = NULL);
	void CheckHelpIcon(CRole* pRole = NULL);
	void CheckBeHelpIcon(CRole* pRole = NULL);
	void PushTaskRefresh();


public:
	bool m_dirty;
	UINT64 m_roldId;
	bool m_canRefresh;
	UINT32 m_currScore;
	UINT32 m_luck;
	UINT32 m_dailyRefreshCount;			// 免费刷新用的次数
	UINT32 m_extraDailyRefreshCount;	// 剩余的购买了的次数
	UINT32 m_dailyBuyCount;				// 今日总共购买的次数
	UINT32 m_dailyHelpCount;

	std::set<UINT64> m_askedRoleId;
	std::vector<DailyTaskRefreshAskInfo> m_beAskHelpInfo;
	std::vector<DailyTaskRefreshRecord> m_refreshRecord;
};

#endif