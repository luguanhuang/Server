#ifndef __SUPERRISKRECORD_H__
#define __SUPERRISKRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "gamelogic/superriskmap.h"
#include "superrisk.h"
#include "superriskconfig.h"

class Role;

class Scene;
namespace KKSG
{
	class GetRiskMapInfosRes;
	class RiskBoxInfo;
	class GetRiskMapInfosArg;
	class ChangeRiskBoxStateRes;
}

#define MAX_RISK_BOX_NUM 3

struct AllRiskBoxInfo
{
	AllRiskBoxInfo()
	{
	}
	int AddBox(int itemID, int itemNum);

	UINT32 ChangeBoxState(Role* role, int slot, KKSG::RiskBoxState state, KKSG::ChangeRiskBoxStateRes& roRes);

	int GetLeftOpenTime(int slot);

	bool Accelerate(Role* role, int slot);

	bool CanGetBoxReward(int& slot);

	bool HasBoxOpened();

	bool OnTimer();

	const KKSG::RiskBoxInfo* GetSlotInfo(int slot) const;


	std::map<int, KKSG::RiskBoxInfo> boxInfos;
};
struct RiskMapRoleInfo
{
	RiskMapRoleInfo():pos(0, 0)
	{
		mapID = 0;
		moveDirection = 0;
	}


	int mapID;
	//std::map<int, KKSG::RiskBoxInfo> boxInfos;

	Coordinate pos;
	int moveDirection;
};
class CSuperRiskRecord : public RoleSerializeInterface, public ITimer
{
public:
	CSuperRiskRecord(Role* role);
	virtual ~CSuperRiskRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual bool AutoDestory();
	virtual void FirstInit(UINT32 roleNum);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void Reset();
	void Change();

	void Update();
	void OnFirstEnterScene();
	void OnDayPass();

	void DebugReset();

	void DebugResetDice();
	void OnOpenSystem();
	bool IsMapOpen(int mapID);
	void OnLevelUp();

	void AddDiceNum(int num, bool canOver = false);
	int GetMaxDiceNum();
	int GetLeftDiceNum();

	int GetLeftDiceTime();

	UINT32 GetMapInfo2Client(const KKSG::GetRiskMapInfosArg& roArg, KKSG::GetRiskMapInfosRes& roRes);

	const RiskMapRoleInfo* GetRoleRiskMapInfo(int mapID);

	const SuperRiskMapGeneratedInfo* GetGeneratedMapInfo(int mapID);

	const AllRiskBoxInfo& GetBoxInfos();

	void EndMove(int mapID, Coordinate& dest, int direction);

	void RefreshMapInfo(int mapID, bool isEndRefresh = true);

	int AddBox(int mapID, int itemID, int itemNum);

	UINT32 ChangeBoxState(int mapID, int slot, KKSG::RiskBoxState state, KKSG::ChangeRiskBoxStateRes& roRes);

	bool ReduceDiceNum(int num);

	void OnLogout();

private:
	bool HasFullDiceOrBoxReward(bool& full, int& mapID, int& slot);

	void OnRecoverDiceNum(int time);

	void OnTimeCheckSendNtf(int time);

	void OnTimeOpenBox(int time);

	UINT32 GetDiceRecoverFullTime();


public:
	SuperRisk m_superRisk;

private:
	Role* m_pRole;
	Switch m_switch;

	HTIMER m_timer;
	int m_updateTime;

	//std::map<int, SuperRiskMapGeneratedInfo*> m_mapInfos;
	std::map<int, SuperRiskMapGeneratedInfo> m_mapInfos;

	std::map<int, RiskMapRoleInfo> m_roleMapInfo;

	AllRiskBoxInfo m_boxInfos;

	int m_diceNum;

	int m_getDiceTime;
	int m_getDiceNum;

	bool m_hasSendPlayNtf;
};

#endif