#ifndef __LINESELECTOR_H__
#define __LINESELECTOR_H__

#include "commondef.h"

enum
{
	SelectLine_Hall      = 0,
	SelectLine_Dynamic   = 1,
	SelectLine_WorldBoss = 2,
	SelectLine_Specific  = 3,
	SelectLine_GuildBoss = 4,
	SelectLine_Owner	 = 5,
};

struct LineDest
{
	UINT32 line;
	UINT32 destid;
	UINT32 mapid;
	UINT32 type;
	bool isdelay;

	LineDest()
	{
		isdelay = false;
		line = INVALID_LINE_ID;
		destid = INVALID_SCENE_ID;
		mapid = INVALID_MAP_ID;
		type = 0;
	}
};
class ILineSelector
{
public:
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest) = 0;
};

class LineSelectorHall: public ILineSelector
{
public:
	static LineSelectorHall GlobalSelectorHall;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);
};

class LineSelectorDynamic : public ILineSelector
{
public:
	static LineSelectorDynamic GlobalSelectorDynamic;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);
};

class CRole;
class LineSelectorWorldBoss : public ILineSelector
{
public:
	static LineSelectorWorldBoss GlobalSelectorWorldBoss;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);

	std::vector<CRole*> m_roles;
};

class LineSelectorSpecific : public ILineSelector
{
public:
	static LineSelectorSpecific GlobalSelectorSpecific;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);
};

class LineSelectorGuildBoss : public ILineSelector
{
public:
	static LineSelectorGuildBoss GlobalSelectorGuildBoss;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);

	std::vector<CRole*> m_roles;
};

class LineSelectorOwner: public ILineSelector
{
public:
	static LineSelectorOwner GlobalSelectorOwner;
	virtual UINT32 SelectLine(UINT32 destid, LineDest& dest);

	UINT32 m_mapid;
	UINT64 m_uid;
	std::vector<CRole*> m_roles;
};


#endif