#ifndef __TEAMRECORD_H__
#define __TEAMRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;

enum TeamCostRecoverReason
{
	TCRR_NORMAL = 0,
	TCRR_WIN = 1,
	TCRR_RESET = 2,
};

class STeamCountInfo
{
public:
	STeamCountInfo()
	{
		DayZero();
		WeekZero();
	}
	~STeamCountInfo(){}

	void DayZero();
	void WeekZero();
	//static int GetGiveCountToday(int teamType);

	int m_finishCountToday;//今天进入次数
	int m_buyCountToday;//今天购买次数(对购买进行计数)
	int m_extraAddCount;//额外增加次数(购买或其他),永久使用
	UINT32 m_helpCount;//助战次数
};

class CTeamRecord : public RoleSerializeInterface
{
public:
	CTeamRecord(Role* role);
	~CTeamRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update(); 
	void DayZero();
	void WeekZero();

	int GetTeamFinishCountToday(int teamType);
	int GetTeamBuyCountToday(int teamType);
	int GetTeamLeftCountToday(int teamType);
	int GetTeamMaxCountToday(int teamType);
	void AddTeamFinishCountToday(int teamType);
	void AddTeamBuyCountToday(int teamType);	
	void AddTeamExtraAddCount(int teamType);

	UINT32 GetWNRewardLeftCount();
	UINT32 GetWNRewardMaxCount();
	void AddWNRewardCount();

	void AddGoddessRewardToday();//加女神试炼奖励领取次数
	int GetGoddessRewardToday();
	int GetLeftGoddessRewardToday();//剩余女神试炼次数

	//KKSG::RoleTeamCostInfo* GetTeamCost() { m_IsModified.Set(); return &m_teamcost;}
	// team cost
	void MarkTeamCostInfo(UINT32 expid, UINT32 index);
	void RecoverTeamCostInfo(TeamCostRecoverReason reason = TCRR_NORMAL);
	void ResetTeamCostInfo(TeamCostRecoverReason reason);
	bool GetTeamCostInfo(UINT32& expid, UINT32& index);
	bool IsInValidCostInfo();
	void UpdateCostInfo();
	UINT32 GetTeamCostCount();
	void AddTeamCostCount();
	UINT32 GetTeamCostGiftValue();
	void SetTeamCostGift(UINT32 value);

	UINT32 GetDiamondCostCountLeft();
	void AddDiamondCostCount();

	UINT32 GetUseTicketLeftCount();
	void AddUseTicketCount();

	int GetStarProgressByType(UINT32 type);

	void AddHelpCount(int type);
	UINT32 GetHelpCount(int type);
	void AddDragonHelpFetchedReward(int id);
	bool HaveDragonHelpFetchedReward(int id);
	void ClearDragonHelpFetchedReward();

	void SetWantDragonHelp(bool want);
	bool IsWantDragonHelp() { return m_wantDragonHelp; }
	bool IsWantHelp(int type);

	void DebugResetTeamCount();

	void TestPrint();

private:
	Switch m_IsModified;
	Role* m_pRole;
	UINT32 m_lastDayUpTime;
	UINT32 m_lastWeekUpTime;
	UINT32 m_GoddessGetRewardToday; //女神今日领取的次数
	UINT32 m_weekNestRewardCount;
	UINT32 m_diamondCostCount;
	UINT32 m_useTicketCount;
	bool m_wantDragonHelp;

	std::unordered_set<int> m_dragonHelpFetchedReward;//龙本助战已领取奖励

	KKSG::RoleTeamCostInfo m_teamcost;

	std::unordered_map<int, STeamCountInfo> m_TeamCountInfo;//组队参加次数
};

#endif