#ifndef __VSSCENERECORD_H__
#define __VSSCENERECORD_H__

#include <time.h>

///////////////////////////////////////////

struct VsUnit
{
	VsUnit()
	{
		uid = 0;
		ppt = 0.0;
	}
	UINT64 uid;//role dummyrole
	std::string name;
	double ppt;
};

class Unit;

class VkOneRec
{
public:
	VkOneRec(const VsUnit& kill, const VsUnit& dead);
	VsUnit& GetKill(){ return m_kill; } 
	VsUnit& GetDead(){ return m_dead; }
private:
	time_t m_ktime;
	VsUnit m_kill;
	VsUnit m_dead;
};

class VsKillRecord
{
public:
	VsKillRecord();
	void AddOneRecord(Unit* kill, Unit* dead);
	void Clear();

private:
	void GetVsUnit(Unit* pUnit, VsUnit& vsunit);
	std::vector<VkOneRec> m_killlist;
};

///////////////////////////////////////////

class GmfKickRecord
{
public:
	void AddRole(UINT64 roleid);
	UINT32 GetCoolDownTime(UINT64 roleid);

private:
	std::unordered_map<UINT64, UINT32> m_kickrec;
};

///////////////////////////////////////////

#endif