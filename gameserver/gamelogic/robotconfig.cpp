#include "pch.h"
#include "robotconfig.h"
#include "rolesummarymgr.h"
#include "robotgenerator.h"
#include "util/XRandom.h"
#include "LogInit.h"
#include "unit/dummyrolemanager.h"
#include "table/PlayerLevelManager.h"

static bool robot_debug_print = false;

INSTANCE_SINGLETON(RobotConfig)


void RobotConfig::TogglePrint()
{
	robot_debug_print = !robot_debug_print;
}


RobotConfig::RobotConfig()
{

}

RobotConfig::~RobotConfig()
{

}

bool RobotConfig::Init()
{
	if (!m_oTemplateTable.LoadFile("table/RobotTemplate.txt"))
	{
		LogWarn("load file table/RobotTemplate.txt failed!");
		return false;
	}

	if (!m_oLookupTable.LoadFile("table/RobotLookup.txt"))
	{
		LogWarn("load file table/RobotLookup.txt failed!");
		return false;
	}

	if (!m_oRandNameTable.LoadFile("table/RandomName.txt"))
	{
		LogWarn("load file table/RandomName.txt failed!");
		return false;
	}

	return true;
}

void RobotConfig::Uninit()
{
	m_oTemplateTable.Clear();
	m_oLookupTable.Clear();
	m_oRandNameTable.Clear();
}

RobotConf *RobotConfig::FindRobotConf(int id)
{
	return m_oTemplateTable.GetByRobotTemplateID(id);
}

 
RobotLookupConf *RobotConfig::FindLookupConf(int lookupID)
{
	return m_oLookupTable.GetByRobotLookupID(lookupID);
}

class IRobotFilter
{
public:
	virtual ~IRobotFilter(){}
	virtual void ExecuteFilter(const FilterRoleData &filter, RobotLookupConf *pConf) = 0;
	virtual UINT32 GetCount() = 0;
	virtual RoleSummary *CreateSummary(UINT32 offset) = 0;
	RoleSummary *CreateRandom() { return CreateSummary(XRandom::randInt(0, GetCount())); }
};

static bool FilterSummary(RoleSummary *pSummary, const FilterRoleData &filter, RobotLookupConf *pConf)
{
	if (pSummary->qwRobotID != 0)
	{
		return false;
	}

	if (pSummary->GetID() == filter.qwRoleID)
	{
		return false;
	}

	if (pConf->Level.size() == 2)
	{
		int leveldiff = pSummary->GetLevel() - filter.level;
		if (leveldiff < pConf->Level[0] ||
			leveldiff > pConf->Level[1])
		{
			LogDebug("[X] %s becasuse level diff %d ", pSummary->GetName().c_str(), leveldiff); 
			return false;
		}
	}

	if (pConf->CombatScore.size() == 2)
	{
		if (filter.ppt > 0)
		{
			int pptdiff = pSummary->GetPowerPoint() * 100 / filter.ppt - 100;
			if (pptdiff < pConf->CombatScore[0] ||
				pptdiff > pConf->CombatScore[1])
			{
				LogDebug( "[X] %s becasuse level diff %d ", pSummary->GetName().c_str(), pptdiff); 
				//return false; for test
			}
		}
	}

	if (pConf->Profession != 0)
	{
		if (pConf->Profession == 11 && filter.prof%10 != (int)pSummary->GetProfession()%10)
		{
			LogDebug( "[X] %s becasuse profession 11 %d<>%d", pSummary->GetName().c_str(), filter.prof%10, pSummary->GetProfession()%10); 
			return false;
		}

		if (pConf->Profession == 12 && filter.prof%10 == (int)pSummary->GetProfession()%10)
		{
			LogDebug( "[X] %s becasuse profession 12 %d<>%d", pSummary->GetName().c_str(), filter.prof%10, pSummary->GetProfession()%10); 
			return false;
		}

		if (pConf->Profession != (int)pSummary->GetProfession()%10)
		{
			LogDebug( "[X] %s becasuse profession %d<>%d", pSummary->GetName().c_str(), pConf->Profession, pSummary->GetProfession()%10); 
			return false;
		}
	}

	LogDebug( "[O] %s", pSummary->GetName().c_str()); 
	return true;
}

class RealRoleSummaryFilter : public IRobotFilter
{
	std::vector<RoleSummary *> m_summarys;

public:

	virtual void ExecuteFilter(const FilterRoleData &filter, RobotLookupConf *pConf)
	{
		LogDebug( "start RealRoleSummaryFilter"); 
		RoleSummary *pSummary = NULL;
		auto begin = RoleSummaryMgr::Instance()->Begin();
		auto end = RoleSummaryMgr::Instance()->End();
		for (auto i = begin; i != end; ++i)
		{
			pSummary = i->second;

			if (FilterSummary(pSummary, filter, pConf))
			{
				m_summarys.push_back(pSummary);
			}
		}
	}

	virtual UINT32 GetCount()
	{
		return m_summarys.size();
	}

	virtual RoleSummary * CreateSummary(UINT32 offset)
	{
		if (offset >= m_summarys.size())
		{
			return NULL;
		}

		return m_summarys[offset];
	}
};

class ExistRobotSummaryFilter : public IRobotFilter
{
	std::vector<RoleSummary *> m_summarys;

public:

	virtual void ExecuteFilter(const FilterRoleData &filter, RobotLookupConf *pConf)
	{
		LogDebug( "start ExistRobotSummaryFilter"); 
		RoleSummary *pSummary = NULL;
		auto begin = RobotGenerator::Instance()->Begin();
		auto end = RobotGenerator::Instance()->End();
		for (auto i = begin; i != end; ++i)
		{
			pSummary = i->first;

			if (FilterSummary(pSummary, filter, pConf))
			{
				m_summarys.push_back(pSummary);
			}
		}
	}

	virtual UINT32 GetCount()
	{
		return m_summarys.size();
	}

	virtual RoleSummary * CreateSummary(UINT32 offset)
	{
		if (offset >= m_summarys.size())
		{
			return NULL;
		}

		return m_summarys[offset];
	}

};

class RobotTemplateFilter : public IRobotFilter
{
	std::vector<RobotConf *> m_confs;

public:

	virtual void ExecuteFilter(const FilterRoleData &filter, RobotLookupConf *pConf)
	{
		LogDebug( "start RobotTemplateFilter");

		RobotTemplateTable &oTemplateTable = RobotConfig::Instance()->GetTemplateTable();
		for (UINT32 i = 0; i < oTemplateTable.Table.size(); ++i)
		{
			auto pRobotConf = oTemplateTable.Table[i];
			if (pRobotConf->RobotLevel > PlayerLevelManager::Instance()->MaxLevel())
			{
				continue;
			}

			if (pConf->Level.size() == 2)
			{
				int leveldiff = pRobotConf->RobotLevel - filter.level;
				if (leveldiff < pConf->Level[0] ||
					leveldiff > pConf->Level[1])
				{
					LogDebug( "[X] %s becasuse level diff %d", pRobotConf->RobotName.c_str(), leveldiff);
					continue;
				}
			}

			if (pConf->Profession != 0)
			{
				if (pConf->Profession == 11 && filter.prof%10 != pRobotConf->RobotProfession)
				{
					LogDebug( "[X] %s becasuse profession 11 %d<>%d", pRobotConf->RobotName.c_str(), filter.prof%10, pRobotConf->RobotProfession%10);
					continue;
				}

				if (pConf->Profession == 12 && filter.prof%10 == pRobotConf->RobotProfession)
				{
					LogDebug( "[X] %s becasuse profession 12 %d<>%d", pRobotConf->RobotName.c_str(), filter.prof%10, pRobotConf->RobotProfession%10);
					continue;
				}

				if (pConf->Profession != pRobotConf->RobotProfession)
				{
					LogDebug( "[X] %s becasuse profession %d<>%d", pRobotConf->RobotName.c_str(), pConf->Profession%10, pRobotConf->RobotProfession%10);
					continue;
				}
			}

			LogDebug( "[O] %s", pRobotConf->RobotName.c_str());
			m_confs.push_back(pRobotConf);
		}
	}

	virtual UINT32 GetCount()
	{
		return m_confs.size();
	}

	virtual RoleSummary * CreateSummary(UINT32 offset)
	{
		if (offset >= m_confs.size())
		{
			return NULL;
		}

		return RobotGenerator::Instance()->CreateRobotSummary(m_confs[offset]);
	}
};

RoleSummary *RobotConfig::CreateRobotSummary(const FilterRoleData &filter, RobotLookupConf *pConf)
{
	RealRoleSummaryFilter filter1;
	ExistRobotSummaryFilter filter2;
	RobotTemplateFilter filter3;

	//if (pConf->LookupRule[0] == 1)
	//{
	//	filter1.ExecuteFilter(filter, pConf);
	//	if (filter1.GetCount() > 0)
	//	{
	//		return filter1.CreateRandom();
	//	}
	//}

	if (pConf->LookupRule[1] == 1)
	{
		filter2.ExecuteFilter(filter, pConf);
		if (filter2.GetCount() > 0 && filter2.GetCount() >= pConf->ExistSelectLimit)
		{
			return filter2.CreateRandom();
		}
	}
	
	if (pConf->LookupRule[2] == 1)
	{
		filter3.ExecuteFilter(filter, pConf);
		if (filter3.GetCount() > 0)
		{
			return filter3.CreateRandom();
		}
	}

	return NULL;
}

RoleSummary * RobotConfig::CreateRobotSummary(const FilterRoleData &filter, int lookupID)
{
	auto pConf = FindLookupConf(lookupID);
	if (pConf == NULL)
	{
		LogWarn("not found robot lookup id: %d", lookupID);
		return NULL;
	}

	RoleSummary *pSummary = CreateRobotSummary(filter, pConf);
	if (pSummary)
	{
		RobotGenerator::Instance()->UseRobotSummary(pSummary);
	}
	
	return pSummary;
}

bool RobotConfig::Reload()
{
	Uninit();
	return Init();
}

std::string RobotConfig::CreateRandomName()
{
	UINT32 n = m_oRandNameTable.Table.size();
	UINT32 f = XRandom::randInt(0, n);
	UINT32 l = XRandom::randInt(0, n);
	std::string s = m_oRandNameTable.Table[f]->FirstName;
	s.append(m_oRandNameTable.Table[l]->FirstName);
	return s;
}

