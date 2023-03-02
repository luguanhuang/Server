#ifndef __DRAGONRECORD_H__
#define __DRAGONRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
namespace KKSG
{
	class GetDragonTopInfoRes;
}

struct DragonHardInfo
{
	int hardLevel;
	int reachTopFloor;
	int bestTime;
	int dragonDiamondBoxTimes;
	DragonHardInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct RoleDragonInfo
{
	int dragonType;
	int openHardLevel;
	std::map<int, DragonHardInfo> dragonHard;
	RoleDragonInfo()
	{
		Reset();
	}
	void Reset()
	{
		dragonType = 0;
		openHardLevel = 0;
		dragonHard.clear();
	}
};

class CDragonRecord : public RoleSerializeInterface
{
public:
	CDragonRecord(Role* role);
	virtual ~CDragonRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Change();

	static int GetLastActivityTime(std::vector<UINT32>& weekDays);
	static int GetNextActivityTime(std::vector<UINT32>& weekDays);
	static int CalcDragonRefreshTimes(int beginTime, int hard = 1);
	void Update();

	void Reset(int lastTime, int hard);
	void DebugReset(int type, int hard, bool clear);

	const RoleDragonInfo* GetDragonInfo(int type, int hardLevel);
	void FillTopInfo2Client(KKSG::GetDragonTopInfoRes& roRes);
	bool CheckRoleCondition(int expID);

	bool OnDayPass();

	int AddFloor(int expID);

	int GetNextExpID(int expID);


	bool CheckDefiniteDrop(UINT32& qualityDrop, bool resetTimes);

	void OnLogin();

private:
	void _FillDragonInfo(int type, int hard, int floor, KKSG::DragonInfo2Client* info);
private:
	Role* m_pRole;
	Switch m_switch;

	std::map<int, RoleDragonInfo> m_dragonInfo;
	int m_updateDay;
	int m_updateHardDragonDay;
	int m_updateSmallDragonDay;

};

#endif
