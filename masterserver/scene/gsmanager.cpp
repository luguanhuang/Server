#include "pch.h"
#include "gsmanager.h"
#include "role/rolemanager.h"
#include "util.h"
#include "scenemanager.h"
#include "common/rpcm2g_crashrecoverm2g.h"
#include "network/gslink.h"
#include "levelseal/levelsealMgr.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "global/GlobalConfigDBBuild.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "pay/payconsumemgr.h"
#include "pay/paymgr.h"


INSTANCE_SINGLETON(CGsManager)


CGsManager::CGsManager()
{
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
	m_oGsMap[dwLine] = oInfo;

	UINT32 dwTimes = GetGsLinkTimes(dwLine);
	if(dwTimes == 0)
	{
		//first connected
		RpcM2G_CrashRecoverM2G* rpc = RpcM2G_CrashRecoverM2G::CreateRpc();
		rpc->m_oArg.set_gsline(dwLine);
		GSLink::Instance()->SendToLine(dwLine, *rpc);
	}

	SetGsLinkTimes(dwLine, dwTimes + 1);

	//levelseal ntf
	CLevelSealMgr::Instance()->NotifyLevelSealInfo();
	//npc feeling
	CNpcFeelingMgr::Instance()->SyncData(dwLine);

	PtcM2G_BroadCastGlobalValue globalvalue;
	globalvalue.m_Data.set_id(eDragonGlobal);
	globalvalue.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal));
	GSLink::Instance()->SendToLine(dwLine, globalvalue);

	PtcM2G_BroadCastGlobalValue loginact;
	loginact.m_Data.set_id(eLoginActivity);
	loginact.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLoginActivity));
	GSLink::Instance()->SendToLine(dwLine, loginact);


	if (dwLine == MAIN_HALL_GS_LINE)
	{
		PtcM2G_BroadCastGlobalValue competedragonvalue;
		competedragonvalue.m_Data.set_id(eCompeteDragonInfo);
		competedragonvalue.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eCompeteDragonInfo));
		GSLink::Instance()->SendToLine(dwLine, competedragonvalue);

	}


	CPayConsumeMgr::Instance()->SendToGs(dwLine);
}

void CGsManager::OnLineClosed(UINT32 dwLine)
{
	CRoleManager::Instance()->LogoutRoleOnGsLine(dwLine, false);
	CSceneManager::Instance()->DestroySceneOnGsLine(dwLine);

	m_oGsMap.erase(dwLine);
}

CGsInfo* CGsManager::GetGsInfo(UINT32 dwLine)
{
	CGsInfoMap::iterator it = m_oGsMap.find(dwLine);
	return it == m_oGsMap.end() ? NULL : &it->second;
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

void CGsManager::SetGsLinkTimes(UINT32 dwLine, UINT32 dwTimes)
{
	m_oGsLinkRecMap[dwLine] = dwTimes;
}

UINT32 CGsManager::GetGsLinkTimes(UINT32 dwLine) const
{
	CGsLinkRecMap::const_iterator it = m_oGsLinkRecMap.find(dwLine);
	return it == m_oGsLinkRecMap.end() ? 0 : it->second;
}
