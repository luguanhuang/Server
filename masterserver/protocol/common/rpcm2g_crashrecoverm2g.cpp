#include "pch.h"
#include "common/rpcm2g_crashrecoverm2g.h"
#include "role/rolemanager.h"
#include "scene/scenemanager.h"
#include "team/teammgr.h"
#include "chat/fmmgr.h"
#include "match/matchmgr.h"

// generate by ProtoGen at date: 2016/11/22 21:09:23

RPC_CLIENT_IMPLEMETION(RpcM2G_CrashRecoverM2G, CrashRecoverM2GArg, CrashRecoverM2GRes)

void RpcM2G_CrashRecoverM2G::OnReply(const CrashRecoverM2GArg &roArg, const CrashRecoverM2GRes &roRes, const CUserData &roUserData)
{
	if(roRes.rolelist_size() != roRes.sessionlist_size())
	{
		LogError("Crash Recover from gs_%u error, invalid role/session list size", roArg.gsline());
	}
	else
	{
		
		for(INT32 i = 0; i < roRes.rolelist_size(); ++i)
		{
			UINT64 qwSessionID = roRes.sessionlist(i);
			const KKSG::RoleInfoOnMs& roInfo = roRes.rolelist(i);
			if(CRoleManager::Instance()->GetByRoleID(roInfo.roleid()) != NULL)
			{
				continue;
			}

			CRole* poRole = CRoleManager::Instance()->RecoverRole(&roInfo, qwSessionID);
			if(poRole != NULL)
			{

				poRole->SetState(ROLE_STATE_IN_GAME);
				poRole->SetGsLine(roArg.gsline());
				poRole->SetInCrossGs(roRes.iscrossgs());
			}
		}
	}
	if(roRes.mapidlist_size() != roRes.sceneidlist_size())
	{
		LogError("Crash Recover from gs_%u error, invalid mapid/sceneid list size", roArg.gsline());
	}
	else
	{
		for(INT32 i = 0; i < roRes.sceneidlist_size(); ++i)
		{
			UINT32 dwMapID = roRes.mapidlist(i);
			UINT32 dwSceneID = roRes.sceneidlist(i);
			UINT64 ownerid = roRes.owneridlist(i);
			CSceneManager::Instance()->CreateScene(dwMapID, dwSceneID, roArg.gsline());

			CSceneManager::Instance()->GetOwnerScenes().RecoverScene(ownerid, dwMapID, dwSceneID);
		}
	}

	if(roRes.rolelist_size() > 0 || roRes.sceneidlist_size() > 0)
	{
		LogInfo("Ms crash recover role num [%d], scene num [%d] from gs_%u", roRes.rolelist_size(), roRes.sceneidlist_size(), roArg.gsline());
	}

	TeamMgr::Instance()->OnMsCrashRecover(roArg.gsline(), roRes);
	MatchMgr::Instance()->OnMsCrashRecover(roArg.gsline(), roRes);
}

void RpcM2G_CrashRecoverM2G::OnTimeout(const CrashRecoverM2GArg &roArg, const CUserData &roUserData)
{
	LogError("Crash Recover from gs_%u timeout", roArg.gsline());
}
