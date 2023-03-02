#ifndef __TAKECOSTMGR_H__
#define __TAKECOSTMGR_H__

#include "define/tlogenum.h"

struct CostInfo
{
	UINT32 type;
	UINT32 count;
};

class ICostHandler
{
public:
	virtual ~ICostHandler(){}
	virtual CostInfo GetCost(int i) = 0;
	virtual bool ParseCostConfigString(const std::string &str) = 0;
};

class Role;

class TakeCostManager
{
	TakeCostManager();
	~TakeCostManager();
	DECLARE_SINGLETON(TakeCostManager)

public:

	bool Init();
	void Uninit();

	bool TakeCost(const char *CostName, Role *pRole, int Times, int reason, int subreason);
	CostInfo QueryCost(const char *CostName, int Times);

private:
	ICostHandler *GetHandler(const char *CostName);

	std::unordered_map<std::string, ICostHandler *> m_CostCache;
};

#endif // __TAKECOSTMGR_H__