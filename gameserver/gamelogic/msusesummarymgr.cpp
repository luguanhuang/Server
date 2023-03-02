#include "pch.h"
#include "msusesummarymgr.h"
#include "robotgenerator.h"
#include "unit/role.h"
#include "rolesummarymgr.h"

//INSTANCE_SINGLETON(MSUseSummaryMgr)
//
//MSUseSummaryMgr::MSUseSummaryMgr()
//{
//
//}
//
//MSUseSummaryMgr::~MSUseSummaryMgr()
//{
//
//}
//
//bool MSUseSummaryMgr::Init()
//{
//	return true;
//}
//
//void MSUseSummaryMgr::Uninit()
//{
//	for(auto it = m_useSummarys.begin(); it != m_useSummarys.end(); ++it)
//	{
//		delete it->second;
//	}
//	m_useSummarys.clear();
//}
//
//bool MSUseSummaryMgr::GetUseSummary(Role* pRole, int lookupid, KKSG::MSRobotSumamryRes& data)
//{
//	if(NULL == pRole)
//	{
//		return false;
//	}
//	auto it = m_useSummarys.find(pRole->GetID());
//	if(it == m_useSummarys.end())
//	{
//		FilterRoleData filter;
//		filter.qwRoleID = pRole->GetID();
//		filter.level = pRole->GetLevel();
//		filter.ppt = (int)pRole->GetAttr(TOTAL_POWERPOINT);
//		filter.prof = pRole->GetProfession();
//
//		RoleSummary* summary = RobotConfig::Instance()->CreateRobotSummary(filter, lookupid); 
//		if(NULL == summary)
//		{
//			return false;
//		}
//
//		UseSumamry* pUseSummary = new UseSumamry(pRole->GetID(), summary);		
//
//		if(NULL == pUseSummary)
//		{
//			return false;
//		}
//
//		m_useSummarys[pRole->GetID()] = pUseSummary;
//	}
//
//	it = m_useSummarys.find(pRole->GetID());
//	if(it == m_useSummarys.end())
//	{
//		return false;
//	}
//	else
//	{
//		UseSumamry* pUseSummary = it->second;
//		if(NULL == pUseSummary || NULL == pUseSummary->GetRoleSummary())
//		{
//			return false;
//		}
//		
//		data.set_robotid(pUseSummary->GetRoleSummary()->GetRobotID());
//		SSDebug << __FUNCTION__ << " robotuseid = " << data.useid() << " robotid = " << data.robotid() << END;
//		RoleSummaryMgr::Instance()->SummaryToDBStored(pUseSummary->GetRoleSummary(), *data.mutable_summarystored());
//	}
//
//	return true;
//}
//
//void MSUseSummaryMgr::FreeUseSummary(UINT64 roleID)
//{
//	auto it = m_useSummarys.find(roleID);
//	if(it != m_useSummarys.end())
//	{
//		delete it->second;
//		m_useSummarys.erase(it);
//	}
//	else
//	{
//		SSWarn << " free use summary donot find roleid = " << roleID << END;
//	}
//}
//
//RoleSummary* MSUseSummaryMgr::GetRoleSummaryByUseID(int robotUseID)
//{
//	for(auto it = m_useSummarys.begin(); it != m_useSummarys.end(); ++it)
//	{
//		if((it->second)->GetUseID() == robotUseID)
//		{
//			return (it->second)->GetRoleSummary();
//		}
//	}
//	SSError << " find not robotuseid = " << robotUseID << END;
//	return NULL;
//}
//
//
//UseSumamry::UseSumamry(UINT64 roleID, RoleSummary* summary)
//{
//	static int MSUseSummaryID = 1;
//	m_useID = MSUseSummaryID++;
//	m_roleID = roleID;
//	m_roleSummary = summary;
//	SSDebug << __FUNCTION__ << " useID = " << m_useID << END;
//}
//
//UseSumamry::~UseSumamry()
//{
//	RobotGenerator::Instance()->FreeRoleSummary(m_roleSummary);
//	SSDebug << __FUNCTION__ << " useID = " << m_useID << END;
//}
