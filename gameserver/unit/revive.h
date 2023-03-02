#ifndef __REVIVE_H__
#define __REVIVE_H__

#include "pb/enum.pb.h"
#include "buff/XBuffTemplateManager.h"

struct SReviveInfo
{
	bool isreviving;
	bool islimit;
	KKSG::ReviveType type;
	std::vector<BuffDesc> m_buffs;

	SReviveInfo()
	{
		Clear();
	}

	void Reset()
	{
		isreviving = false;
		islimit = false;
		type = KKSG::ReviveNone;
	}

	void Clear()
	{
		Reset();
		m_buffs.clear();
	}

	void AddBuff(BuffDesc buff)
	{
		m_buffs.push_back(buff);
	}
};

#endif