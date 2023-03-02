#include "pch.h"
#include "robotsummarymgr.h"
#include "role/rolesummary.h"
#include "team/rpcm2g_msusesummaryreq.h"
#include "role/role.h"
#include "network/gslink.h"
#include <time.h>


INSTANCE_SINGLETON(RobotSummaryMgr)

RobotSummaryMgr::RobotSummaryMgr()
{

}

RobotSummaryMgr::~RobotSummaryMgr()
{

}

bool RobotSummaryMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);

	return true;
}

void RobotSummaryMgr::Uninit()
{
	for(auto it = m_robotSummarys.begin(); it != m_robotSummarys.end(); ++it)
	{
		//这里没有发协议通知GS
		delete it->second;
	}
	m_robotSummarys.clear();

	CTimerMgr::Instance()->KillTimer(m_handler);
}

void RobotSummaryMgr::AddRobotSummary(UINT64 roleID, int lookupid, const KKSG::MSRobotSumamryRes& data)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << " loolupid = " << lookupid << END;
	if(0 == roleID || 0 == lookupid || 0 == data.robotid())
	{
		SSError << __FUNCTION__ << " roleid = " << roleID << " loolupid = " << lookupid << " robotid = " << data.robotid() << END;
		return ;
	}

	if(m_robotSummarys.find(roleID) != m_robotSummarys.end())
	{
		SSError << __FUNCTION__ << " repeat add roleid = " << roleID << END;
		return ;
	}

	if(m_registerRoles.find(roleID) == m_registerRoles.end())//超期了
	{
		SSError << __FUNCTION__ << " register timeout roleid = " << roleID << END;
		return;	
	}

	m_registerRoles.erase(roleID);

	RobotSummary* useSumamry = new RobotSummary(roleID, lookupid, data.robotid(), data.summarystored());
	if(useSumamry)
	{
		m_robotSummarys[roleID] = useSumamry;
	}
}

void RobotSummaryMgr::RegisterRobotSummary(UINT64 roleID, int lookupid)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << " loolupid = " << lookupid << END;
	if(0 == roleID || 0 == lookupid)
	{
		return ;
	}

	if(m_registerRoles.find(roleID) != m_registerRoles.end())//在等待
	{
		return ;
	}

	auto it = m_robotSummarys.find(roleID);
	if(it != m_robotSummarys.end())
	{
		SSWarn << __FUNCTION__ << " roleid = " << roleID << " want lookupid = " << lookupid << " have loolupid = " << (it->second)->GetLookUpID() << END;
		return ;
	}

	m_registerRoles[roleID] = time(NULL);

	RpcM2G_MSUseSummaryReq *rpc = RpcM2G_MSUseSummaryReq::CreateRpc();
	rpc->m_oArg.set_roleid(roleID);
	rpc->m_oArg.set_lookupid(lookupid);
	rpc->m_oArg.set_type(KKSG::MSUS_GET);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE,*rpc);
}

RobotSummary* RobotSummaryMgr::GetRobotSummary(UINT64 roleID, int lookupid)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << " loolupid = " << lookupid << END;
	if(0 == roleID || 0 == lookupid)
	{
		return NULL;
	}
	auto it = m_robotSummarys.find(roleID);
	if(it == m_robotSummarys.end())
	{
		return NULL;
	}
	if((it->second)->GetLookUpID() != lookupid)
	{
		return NULL;
	}

	return (it->second);
}

void RobotSummaryMgr::FreeRobotSummary(UINT64 roleID)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << END;
	if(0 == roleID)
	{
		return ;
	}
	auto it = m_robotSummarys.find(roleID);
	if(it != m_robotSummarys.end())
	{
		delete it->second;
		m_robotSummarys.erase(it);
	}
}

bool RobotSummaryMgr::HaveCanUseRobotSummary(UINT64 roleID, int lookupid)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << " lookupid = " << lookupid << END;
	if(0 == roleID || 0 == lookupid)
	{
		return false;
	}
	auto it = m_robotSummarys.find(roleID);
	if(it == m_robotSummarys.end())
	{
		return false;
	}
	if((it->second)->GetLookUpID() != lookupid)
	{
		SSWarn << __FUNCTION__ << " roleid = " << roleID 
			<< " want lookupid = " << lookupid << " have lookupid = " << (it->second)->GetLookUpID() << END;
		return false;
	}

	return true;
}

void RobotSummaryMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t now = time(NULL);

	for(auto it = m_registerRoles.begin(); it != m_registerRoles.end();)
	{
		if(it->second + 10 < now)
		{
			m_registerRoles.erase(it++);
		}
		else
		{
			it++;
		}
	}

	std::vector<UINT64> waitDel;
	for(auto it = m_robotSummarys.begin(); it != m_robotSummarys.end(); ++it)
	{
		RobotSummary* rs = it->second;
		if(rs->GetCreateTime() + 30 < now)
		{
			waitDel.push_back(it->first);
		}
	}

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		FreeRobotSummary(waitDel[i]);
	}
}


RobotSummary::RobotSummary(UINT64 roleID, int lookupid, UINT64 robotid, const KKSG::RoleSummaryStored& data)
{
	m_roleID = roleID;
	m_lookupid = lookupid;
	m_robotID = robotid;
	m_data = data;
	m_createTime = time(NULL);
}

RobotSummary::~RobotSummary()
{
}
