#ifndef __TOWERRECORD_H__
#define __TOWERRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "timer.h"

class Role;
//#define TOTAL_FLOOR_SIZE 20
#define DEFAULT_HARDLEVEL 1
namespace KKSG
{
	class GetTowerActivityTopRes;
}
struct RoleTowerInfo
{
	int openHardLevel;
	int reachTopFloor;
	int bestTime;

	//扫荡结束更新
	int sweepTime;
	int sweepFloor;

	int curFloor;
	int curCostTime;
	int refreshCount;
	int refreshResult;

	std::vector<UINT32> gotFirstPassRewards;

	RoleTowerInfo()
	{
		Reset();
	}

	void ResetTower()
	{
		refreshCount = 0;
		refreshResult = 0;
		sweepFloor = 0;
		sweepTime = 0;
		curFloor = 0;
		curCostTime = 0;
	}

	void Reset()
	{
		openHardLevel = 0;
		reachTopFloor = 0;
		bestTime = 0;

		sweepTime = 0;
		sweepFloor = 0;

		curFloor = 0;
		curCostTime = 0;
		refreshCount = 0;
		refreshResult = 0;

		gotFirstPassRewards.clear();
	}

	bool InSweep();
	bool IsSweepOver();//扫荡完，未领奖
	void OnSweepOver();
};
class CTowerRecord : public RoleSerializeInterface, public ITimer
{
public:
	CTowerRecord(Role* role);
	virtual ~CTowerRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual bool AutoDestory();

	void Change();

	void Update();

	void DebugReset(bool resetInfo);
	void DebugPassTower(UINT32 floor);

	bool CheckIsBestInfo(RoleTowerInfo& info);
	void UpdateBestInfo(RoleTowerInfo& info);
	void UpdateTowerRank(RoleTowerInfo& info);
	void UpdateCurInfo(RoleTowerInfo& info);
	int GetStartFloor(int hardLevel);

	void OnLogin();
	const RoleTowerInfo* GetTowerInfo(int hardLevel);
	void FillTopInfo2Client(KKSG::GetTowerActivityTopRes& roRes);
	UINT32 CheckRoleCondition(int hardLevel, int floorSize);
	UINT32 CheckEnterScene(int hardLevel, int floor);//单机

	UINT32 SweepTower(UINT32 hardLevel, UINT32 itemID, UINT32 count, UINT32& leftTime);
	void CheckSweepTopestFloor(int hardLevel);
	void GiveSweepReward(int hardLevel);
	UINT32 GetSweepAlreadyFloor(int hardLevel);
	UINT32 GetSweepLeftTime(int hardLevel);

	UINT32 GetSweepFloor(int hardLevel);
	UINT32 RefreshSweepReward(int hardLevel, int& result);
	UINT32 GetRefreshSweepRandValue();

	UINT32 ResetTower(int hardLevel);
	UINT32 AutoResetTower(int hardLevel);

	UINT32 GetFirstPassReward(int hardLevel, int floor);
private:
	Role* m_pRole;
	Switch m_switch;

	HTIMER m_timer;
	//RoleTowerInfo m_towerInfo;
	std::map<int, RoleTowerInfo> m_towerInfo;

	int m_useCount;
	int m_updateTime;

};

#endif