#ifndef __OPERATING_ACTIVITY_H_
#define __OPERATING_ACTIVITY_H_
#include "table/OperatingActivity.h"


class Role;
class OperatingActivityMgr
{
	DECLARE_SINGLETON(OperatingActivityMgr);
public:
	OperatingActivityMgr();
	~OperatingActivityMgr();
public:
	struct stOperatingBase
	{
		UINT32 nStartHandle;
		UINT32 nEndHandle;
		UINT32 nStartTime;
		UINT32 nEndTime;
	};

	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	bool OnStart(UINT32 nID);
	bool OnClose(UINT32 nID);
	void Close();
	void OnTriger(Role *pRole);
private:
	std::map<UINT32,stOperatingBase> m_mapHandle;
	UINT32 m_nTime;
	OperatingActivity m_Table;
};

#endif