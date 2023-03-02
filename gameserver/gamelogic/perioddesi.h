#ifndef __PERIOD_DESI_MGR_H_
#define __PERIOD_DESI_MGR_H_
#include "table/PeriodDesignation.h"



class Role;
class PeriodDesiMgr
{
	DECLARE_SINGLETON(PeriodDesiMgr);
public:
	PeriodDesiMgr();
	~PeriodDesiMgr();
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	void OnTriger(Role *pRole,UINT32 nDesID);
private:
	PeriodDesignation m_Table;
};

#endif