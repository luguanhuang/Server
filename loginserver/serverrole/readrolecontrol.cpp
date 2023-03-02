#include "pch.h"
#include "readrolecontrol.h"
#include "crpc.h"
#include "gatemgr/gatemgr.h"

ReadRoleControl::ReadRoleControl()
{

}

ReadRoleControl::~ReadRoleControl()
{

}

bool ReadRoleControl::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);
	return true;
}

void ReadRoleControl::Uninit()
{

}

void ReadRoleControl::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for (auto it = m_readingInfo.begin(); it != m_readingInfo.end(); )
	{
		if (CheckFinish(it->second))
		{
			delete it->second;
			m_readingInfo.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

ReadRoleInfo* ReadRoleControl::GetReadRole(const std::string& account)
{
	auto it = m_readingInfo.find(account);
	if (it == m_readingInfo.end())
	{
		return NULL;
	}
	return it->second;
}

ReadRoleInfo* ReadRoleControl::NewReadRole(const std::string& account)
{
	if (GetReadRole(account))
	{
		LogError("already has account[%s]", account.c_str());
		return NULL;
	}
	ReadRoleInfo* pInfo = new ReadRoleInfo;
	m_readingInfo.insert(std::make_pair(account, pInfo));
	return pInfo;
}

void ReadRoleControl::RemoveReadRole(const std::string& account)
{
	ReadRoleInfo* pInfo = GetReadRole(account);
	if (pInfo == NULL)
	{
		return;
	}
	delete pInfo;
	m_readingInfo.erase(account);
}

void ReadRoleControl::OnReadCompleate(const std::string& account, UINT32 serverId, const KKSG::GetZoneRoleInfoI2MRes &roRes)
{
	ReadRoleInfo* pInfo = GetReadRole(account);
	if (pInfo == NULL)
	{
		LogWarn("can't find account[%s] when server[%u] read back", account.c_str(), serverId);
		return;
	}
	auto it = pInfo->m_zoneRoles.find(serverId);
	if (it == pInfo->m_zoneRoles.end())
	{
		LogWarn("can't find serverid when server[%u] read back", serverId);
		return;
	}

	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		it->second.m_state = ReadState_Finish;
		for (int i = 0; i < roRes.roles_size(); ++i)
		{
			KKSG::ZoneRoleInfo info;
			info.CopyFrom(roRes.roles(i));
			GateInfo* pServer = GateMgr::Instance()->GetServerInfo(serverId);
			if (pServer)
			{
				info.set_serverid(serverId);
				info.set_servername(pServer->serverName);
				info.set_opentime(pServer->openTime);
			}
			it->second.m_roles.push_back(info);
		}
	}
	else
	{
		it->second.m_state = ReadState_Failed;
		LogWarn("account[%s] read server[%u] role failed[%u]", account.c_str(), serverId, roRes.result());
	}
	if (CheckFinish(pInfo))
	{
		RemoveReadRole(account);
	}
}

void ReadRoleControl::OnReadTimeOut(const std::string& account, UINT32 serverId)
{
	ReadRoleInfo* pInfo = GetReadRole(account);
	if (pInfo == NULL)
	{
		LogWarn("can't find account[%s] when server[%u] read back", account.c_str(), serverId);
		return;
	}
	auto it = pInfo->m_zoneRoles.find(serverId);
	if (it == pInfo->m_zoneRoles.end())
	{
		LogWarn("can't find serverid when server[%u] read back", serverId);
		return;
	}

	it->second.m_state = ReadState_Failed;
	LogWarn("account[%s] read server[%u] role timeout", account.c_str(), serverId);
	if (CheckFinish(pInfo))
	{
		RemoveReadRole(account);
	}
}

bool ReadRoleControl::CheckFinish(ReadRoleInfo* pInfo)
{
	bool finish = true;
	for (auto it = pInfo->m_zoneRoles.begin(); it != pInfo->m_zoneRoles.end(); ++it)
	{
		if (it->second.m_state == ReadState_Reading)
		{
			finish = false;
			break;
		}
	}
	UINT32 now = TimeUtil::GetTime();
	if (now > (pInfo->m_startTime + 20))
	{
		finish = true;
	}
	if (finish == false)
	{
		return false;
	}
	CUserData roUserData(KKSG::ERR_SUCCESS, (void*)pInfo);
	CRpc::ReplyDelayRpc(pInfo->m_rpcId, roUserData);
	return true;
}















