#ifndef _PayConsumeMgr_H_
#define _PayConsumeMgr_H_
#include "unit/role.h"

class CPayConsumeMgr
{
	CPayConsumeMgr();
	~CPayConsumeMgr();
	DECLARE_SINGLETON(CPayConsumeMgr)

public:
	bool Init();
	void Uninit();
	void SetData(const KKSG::ConsumeRebateNtfData& roInfo);
	UINT32 GetRate(UINT64 roleid);
	UINT32 GetRate(UINT32 start, UINT32 end, UINT32 rate);
	void GetRebatInfo(Role *pRole, std::string& out);

private:
	UINT32 m_StartDate;
	UINT32 m_EndDate;
	UINT32 m_Rate;
};
#endif
