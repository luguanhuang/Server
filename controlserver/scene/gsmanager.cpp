#include "pch.h"
#include "gsmanager.h"
#include "role/rolemanager.h"
#include "util.h"
#include "account/accountsessionmgr.h"
#include "scenemanager.h"


INSTANCE_SINGLETON(CGsManager)


CGsManager::CGsManager()
{
	m_pollingcount = 0;
}

CGsManager::~CGsManager()
{
}

bool CGsManager::Init()
{
	return true;
}

void CGsManager::Uninit()
{
}

void CGsManager::OnLineConnected(UINT32 dwLine)
{
	CGsInfo oInfo;
	oInfo.m_dwLineID = dwLine;
	oInfo.m_dwRoleNum = CRoleManager::Instance()->GetRoleCount(dwLine);
	m_oGsMap[dwLine] = oInfo;
}

void CGsManager::OnLineClosed(UINT32 dwLine)
{
	//踢掉所有在这个GS上的Role
	CAccountSessionMgr::Instance()->KickoutPlayerOnGsLine(dwLine);
	CSceneManager::Instance()->DestroySceneOnGsLine(dwLine);

	m_oGsMap.erase(dwLine);
}

CGsInfo* CGsManager::GetGsInfo(UINT32 dwLine)
{
	CGsInfoMap::iterator it = m_oGsMap.find(dwLine);
	return it == m_oGsMap.end() ? NULL : &it->second;
}

UINT32 CGsManager::GetGsLine(UINT32 type)
{
	return (0 == type) ? GetMinimalLoadGsLine() : GetPollingGsLine();
}

UINT32 CGsManager::GetMinimalLoadGsLine()
{
	if(m_oGsMap.empty())
	{
		return INVALID_LINE_ID;
	}
	if(m_oGsMap.size() == 1)
	{
		return m_oGsMap.begin()->second.m_dwLineID;
	}

	UINT32 dwMinNum = (UINT32)-1;
	UINT32 dwDestLine = INVALID_LINE_ID;
	for(CGsInfoMap::iterator it = m_oGsMap.begin(); it != m_oGsMap.end(); ++it)
	{
		CGsInfo& roInfo = it->second;
		if(roInfo.m_dwLineID == MAIN_HALL_GS_LINE) continue;

		if(roInfo.m_dwRoleNum < dwMinNum)
		{
			dwMinNum = roInfo.m_dwRoleNum;
			dwDestLine = roInfo.m_dwLineID;
		}
	}
	return dwDestLine;
}

UINT32 CGsManager::GetPollingGsLine()
{
	if(m_oGsMap.empty())
	{
		return INVALID_LINE_ID;
	}
	if(m_oGsMap.size() == 1)
	{
		return m_oGsMap.begin()->second.m_dwLineID;
	}

	m_pollingcount = (m_pollingcount + 1) % (m_oGsMap.size() - 1);
	UINT32 index = 0;
	UINT32 dwDestLine = INVALID_LINE_ID;
	for(CGsInfoMap::iterator it = m_oGsMap.begin(); it != m_oGsMap.end(); ++it)
	{
		CGsInfo& roInfo = it->second;
		if(roInfo.m_dwLineID == MAIN_HALL_GS_LINE) continue;

		if (index == m_pollingcount)
		{
			dwDestLine = roInfo.m_dwLineID;
			break;
		}
		++index;
	}

	if(dwDestLine == INVALID_LINE_ID)
	{
		LogWarn("Get polling gs line failed");
		return GetMinimalLoadGsLine();
	}

	return dwDestLine;
}

void CGsManager::IncGsLineRoleNum(UINT32 dwLine, UINT32 dwNum)
{
	CGsInfo* poInfo = GetGsInfo(dwLine);
	if(poInfo != NULL)
	{
		poInfo->m_dwRoleNum += dwNum;
	}
}

void CGsManager::DecGsLineRoleNum(UINT32 dwLine, UINT32 dwNum)
{
	CGsInfo* poInfo = GetGsInfo(dwLine);
	if(poInfo != NULL)
	{
		poInfo->m_dwRoleNum = (poInfo->m_dwRoleNum >= dwNum ? poInfo->m_dwRoleNum - dwNum : 0);
	}
}
