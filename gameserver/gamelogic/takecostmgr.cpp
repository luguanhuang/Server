#include "pch.h"
#include "takecostmgr.h"
#include "bagtransition.h"
#include "globalconfig.h"

class DragonCoinCostHandler : public ICostHandler
{
public:

	virtual CostInfo GetCost(int i)
	{
		CostInfo info;
		info.type = DRAGON_COIN;
		
		if (m_Costs.empty())
		{
			info.count = 0;
		}

		if (i >= 0 && i < (int)m_Costs.size())
		{
			info.count = m_Costs[i];
		}
		else
		{
			info.count = m_Costs.back();
		}
		return info;
	}

	virtual bool ParseCostConfigString(const std::string &str)
	{
		std::vector<std::string> vec = Split(str, '|');
		for (unsigned i = 0; i < vec.size(); ++i)
		{
			m_Costs.push_back(convert<int>(vec[i]));
		}

		return true;
	}

private:

	std::vector<int> m_Costs;

};


static ICostHandler *MakeCostHandler()
{
	return new DragonCoinCostHandler();
}

INSTANCE_SINGLETON(TakeCostManager)

TakeCostManager::TakeCostManager()
{

}

TakeCostManager::~TakeCostManager()
{

}

bool TakeCostManager::Init()
{
	return true;
}

void TakeCostManager::Uninit()
{

}

bool TakeCostManager::TakeCost(const char *CostName, Role *pRole, int Times, int reason, int subreason)
{
	ICostHandler *pHandler = GetHandler(CostName);
	if (pHandler == NULL)
	{
		LogWarn("Can't find CostName: [%s] in globalconfig.txt", CostName);
		return false;
	}

	CostInfo info = pHandler->GetCost(Times);
	BagTakeItemTransition transition(pRole);
	transition.SetReason(reason, subreason);
	if (transition.TakeItem(info.type, info.count))
	{
		transition.NotifyClient();
		return true;
	}

	transition.RollBack();
	return false;
}

ICostHandler * TakeCostManager::GetHandler(const char *CostName)
{
	ICostHandler *pHandler = NULL;

	auto it = m_CostCache.find(CostName);
	if (it == m_CostCache.end())
	{
		std::string s =  GetGlobalConfig().GetConfig(CostName);
		if (!s.empty())
		{
			pHandler = MakeCostHandler();
			if (pHandler->ParseCostConfigString(s) == false)
			{
				delete pHandler;
				pHandler = NULL;
			}
			else
			{
				m_CostCache[CostName] = pHandler;
			}
		}
	}
	else
	{
		pHandler = it->second;	
	}

	return pHandler;
}

CostInfo TakeCostManager::QueryCost(const char *CostName, int Times)
{
	CostInfo info;
	info.type = 0;
	info.count = 0;

	ICostHandler *pHandler = GetHandler(CostName);
	if (pHandler == NULL)
	{
		LogWarn("Can't find CostName: [%s] in globalconfig.txt", CostName);
		return info;
	}

	return pHandler->GetCost(Times);
}
