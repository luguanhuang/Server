#ifndef __PVPMGR_H__
#define __PVPMGR_H__

#include "timer.h"

class CRole;

class PvpMgr
{
	PvpMgr();
	~PvpMgr();
	DECLARE_SINGLETON(PvpMgr);
public:
	bool Init();
	void Uninit();

	int GetInMatchCount(CRole* pRole);
	void SetWorldMatchNum(UINT32 num) { m_worldMatchNum = num; }

private:

	UINT32 m_worldMatchNum;			// 世界匹配人数
};

#endif