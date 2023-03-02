#include "pch.h"
#include "unit/role.h"
#include "rolesummarymgr.h"
#include "robotgenerator.h"
#include "unit/calcattribute.h"
#include "foreach.h"
#include "unit/dummyrolemanager.h"


INSTANCE_SINGLETON(RobotGenerator)

RobotGenerator::RobotGenerator()
{
	m_handler = 0;
}

RobotGenerator::~RobotGenerator()
{

}

bool RobotGenerator::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 60000, -1, __FILE__, __LINE__);	

	m_CreateRobotStep.push_back(new RobotGenBasicStep());
	m_CreateRobotStep.push_back(new RobotGenSkillStep());
	m_CreateRobotStep.push_back(new RobotGenEquipStep());
	m_CreateRobotStep.push_back(new RobotGenItemStep());
	m_CreateRobotStep.push_back(new RobotGenEnhanceStep());
	m_CreateRobotStep.push_back(new RobotGenFashionShowStep());

	return true;
}

void RobotGenerator::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}

	foreach(i in m_CreateRobotStep)
	{
		delete (*i);
	}

	m_CreateRobotStep.clear();

	for(auto it = m_RobotLibrary.begin(); it != m_RobotLibrary.end(); ++it)
	{
		delete it->first;
	}
	m_RobotLibrary.clear();
}

RoleSummary * RobotGenerator::CreateRobotSummary(RobotConf *pConf)
{
	Role *pRole = new Role();
	foreach (i in m_CreateRobotStep)
	{
		(*i)->DoWork(pConf, pRole);
	}

	CalcRoleAttribute calc;
	calc.Execute(pRole);

	RoleSummary *pSummary = new RoleSummary();
	pSummary->Update(pRole, SUMMARY_ALL, NULL);
	pRole->Uninit();
	delete pRole;

	pSummary->IsRobot = true;

	RobotUsedInfo &info = m_RobotLibrary[pSummary];
	info.UsedCount = 0;
	info.TotalUsed = 0;
	info.LastOpTime = UINT32(time(NULL));

	return pSummary;
}

void RobotGenerator::FreeRoleSummary(RoleSummary *pSummary)
{
	if (pSummary->IsRobot && m_RobotLibrary.find(pSummary) != m_RobotLibrary.end())
	{
		RobotUsedInfo &info = m_RobotLibrary[pSummary];
		info.UsedCount -= 1;
		info.LastOpTime = UINT32(time(NULL));
	}

	if (pSummary->qwRobotID)
	{
		pSummary->qwRobotID = 0;
	}
}

void RobotGenerator::UseRobotSummary(RoleSummary *pSummary)
{
	if (pSummary->IsRobot && m_RobotLibrary.find(pSummary) != m_RobotLibrary.end())
	{
		RobotUsedInfo &info = m_RobotLibrary[pSummary];
		info.UsedCount += 1;
		info.TotalUsed += 1;
		info.LastOpTime = UINT32(time(NULL));
	}

	pSummary->qwRobotID = DummyRoleManager::Instance()->CreateNewID();
}

void RobotGenerator::DebugDumpLibrary(std::ostream &oss)
{
	oss << "Dump RobotGenerator:\n";
	for (auto i = m_RobotLibrary.begin(); i != m_RobotLibrary.end(); ++i)
	{
		oss << i->first->GetName() << " level=" << i->first->GetLevel() << " TotalUsed=" << i->second.TotalUsed << " CurUsed=" << i->second.UsedCount
			<< " robotID:" << i->first->qwRobotID << "\n";
	}
	oss << "\n";
}

Role* RobotGenerator::CreateTestRole()
{
	RobotConf* pConf = RobotConfig::Instance()->FindRobotConf(31);
	Role *pRole = new Role();

	foreach (i in m_CreateRobotStep)
	{
		(*i)->DoWork(pConf, pRole);
	}

	CalcRoleAttribute calc;
	calc.Execute(pRole);

	return pRole;
}

void RobotGenerator::DebugTransforRobotConfToRole(Role *pRole, RobotConf *pConf)
{
	foreach (i in m_CreateRobotStep)
	{
		(*i)->DoWork(pConf, pRole);
	}

	CalcRoleAttribute calc;
	calc.Execute(pRole);
}

void RobotGenerator::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 DelTime = 3600;//一小时
	UINT32 now = UINT32(time(NULL));
	std::vector<RoleSummary*> waitDel;

	for (auto it = m_RobotLibrary.begin(); it != m_RobotLibrary.end(); ++it)
	{
		RoleSummary* pSummary = (it->first);
		const RobotUsedInfo& info = (it->second);
		if (0 != info.UsedCount)//在使用
		{
			continue;
		}
		if (0 != pSummary->qwRobotID)//在使用
		{
			continue;
		}
		if (info.LastOpTime + DelTime > now)//最近被使用
		{
			continue;
		}
		waitDel.push_back(pSummary);
	}

	for (size_t i = 0; i < waitDel.size(); ++i)
	{
		m_RobotLibrary.erase(waitDel[i]);	
		delete waitDel[i];//释放内存
	}
	waitDel.clear();
}
