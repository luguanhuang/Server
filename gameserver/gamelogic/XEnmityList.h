#ifndef __ENMITYLIST_H__
#define __ENMITYLIST_H__

#include "entity/XEntity.h"

class Unit;

struct Enmity
{
	float value;
	Unit* self;
	UINT64 id;
};

struct ProvokeInfo
{
	ProvokeInfo(UINT64 _id, UINT64 _effectTime, UINT64 _startTime);
	UINT64 id;
	UINT64 effectTime;
	UINT64 startTime;
};

class XEnmityList
{
	std::vector<Enmity> m_HatredList;
	bool m_Inited;
	long m_LastRefreshTime;
	Unit* m_Host;
	static float OT_VALUE;
	std::vector<ProvokeInfo> m_ProvokeInfos;
	std::vector<Unit*> m_Targets;
	UINT64 m_ProvokeTime;
	UINT64 m_ProvokeContain;

public:
	XEnmityList();
	~XEnmityList();
	bool IsInited();
	void Init(Unit* host);
	void Reset();
	void AddHateValue(Unit* unit, float value);
	void AddNewHateValue(Unit* unit, float value);
	void AddInitHateValue(Unit* unit);
	void AddInitHateValue(Unit* unit, float value);
	void RemoveHateValue(Unit* unit);
	void UpdateHateList();
	const std::vector<Unit*>& GetHateEntity(bool filterImmortal = false);
	void SetProvokeUnit(Unit* provoke, float effecttime);
	void UpdateProvoke();
	bool SetProvokeUnitIfNotProvoked(Unit* provoke, float effecttime);
	void RemoveProvokeUnit(UINT64 uid);
	inline bool IsProvoked() { return m_ProvokeInfos.size() > 0; }

	void Print();
private:
	float _CalcHateValue(Unit* unit, float value);
};

#endif