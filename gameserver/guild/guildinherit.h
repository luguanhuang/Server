#ifndef __GUILDINHERIT_H__
#define __GUILDINHERIT_H__

#include "table/GuildInheritTable.h"
#include "multiindex_map.h"
#include "math/XVector3.h"

class CGuildInherit;

struct SInheritPos
{
	Vector3 pos;	
	float face;

	SInheritPos()
	{
		face = 0.0f;
	}
};

struct SInheritData
{
	UINT64 roleOne;
	SInheritPos onepos;
	bool isoneready;

	UINT64 roleTwo;
	SInheritPos twopos;
	bool istwoready;

	UINT32 startTime;
	bool isallready;

	UINT32 type;
	

	SInheritData()
	{
		roleOne = 0;
		roleTwo = 0;
		startTime = 0;
		isallready = false;
		isoneready = false;
		istwoready = false;
		type = 1; // 传功类型 默认公会传功 
	}
};

class Role;
class CGuildInherit : public ITimer
{

public:
	enum INHERIT_TYPE
	{
		GUILD_INHERIT   = 1, // 公会传功
		TEACHER_INHERIT = 2, // 师徒传功
	};

private:
	CGuildInherit();
	~CGuildInherit();
	DECLARE_SINGLETON(CGuildInherit);

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount); 

public:
	void Push(UINT64 roleidone, UINT64 roletidwo, INHERIT_TYPE type = GUILD_INHERIT);
	void Add(Role* role, INHERIT_TYPE type = GUILD_INHERIT);
	void Del(UINT64 roleid);
	bool IsInherit(UINT64 roleId);
	void DoLeaveScene(UINT64 roleId);
	bool IsMapRight(UINT64 roleOne, UINT64 roleTwo);
private:
	void _Ready(SInheritData* data);
	void _End(SInheritData* data);
	void _PosOne(Role* role, SInheritPos& selfpos, INHERIT_TYPE type = GUILD_INHERIT);
	void _PosTwo(Role* role, SInheritPos& selfpos, const SInheritPos& otherpos, INHERIT_TYPE type = GUILD_INHERIT);
	void _DoState(Role* role, bool isstart);

	void _AddExp(SInheritData& data, UINT32 turn, bool isLast, std::vector<UINT64>& delVec);
	void _NoticeStart(UINT64 roleOne, UINT64 roleTwo);
	void _NoticeEnd(UINT64 roleOne, UINT64 roleTwo);
	void _SynMsEnd(UINT64 roleOne, UINT64 roleTwo);

	void _DelOne(UINT64 roleid)
	{
		SInheritData* data = m_roles.find<RoleOneIndex>(roleid);
		if (NULL != data)
		{
			SSInfo<<"erase role:"<<roleid<<END;
			m_roles.erase<RoleOneIndex>(roleid);
			delete data;
		}
	}
	void _DelTwo(UINT64 roleid)
	{
		SInheritData* data = m_roles.find<RoleTwoIndex>(roleid);
		if (NULL != data)
		{
			SSInfo<<"erase role:"<<roleid<<END;
			m_roles.erase<RoleTwoIndex>(roleid);
			delete data;
		}
	}

private:
	HTIMER m_timehandler;
	GuildInheritTable m_table;
	struct RoleOneIndex
	{
		typedef UINT64 key;
		key Key(const SInheritData* r) { return r->roleOne; }
	};

	struct RoleTwoIndex
	{
		typedef UINT64 key;
		key Key(const SInheritData* r) { return r->roleTwo; }
	};

	mutliindex_map_ptr<SInheritData, RoleOneIndex, RoleTwoIndex> m_roles;
};

#endif
