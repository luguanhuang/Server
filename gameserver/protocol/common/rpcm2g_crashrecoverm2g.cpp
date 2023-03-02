#include "pch.h"
#include "common/rpcm2g_crashrecoverm2g.h"
#include "config.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "network/mslink.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "event/eventmgr.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/11/22 21:09:23
bool OnRecoverGSInfo2MS()
{
	EventMgr::Instance()->OnConnectMS();
	LiveManager::Instance()->SyncAllInfoToMs();
	return true;
}

RPC_SERVER_IMPLEMETION(RpcM2G_CrashRecoverM2G, CrashRecoverM2GArg, CrashRecoverM2GRes)

void RpcM2G_CrashRecoverM2G::OnCall(const CrashRecoverM2GArg &roArg, CrashRecoverM2GRes &roRes)
{
	if(MSLink::Instance()->GetLinkTimes() < 1)
	{
		return;
	}

	roRes.set_iscrossgs(GSConfig::Instance()->IsCrossGS());
	std::list<Role*>::iterator it = RoleManager::Instance()->Begin();
	std::list<Role*>::iterator itEnd = RoleManager::Instance()->End();
	for(; it != itEnd; ++it)
	{
		Role* poRole = *it;

		roRes.add_sessionlist(poRole->GetSessionID());
		KKSG::RoleInfoOnMs* poInfo = roRes.add_rolelist();
		poInfo->set_accountid(poRole->GetAccount());
		poInfo->set_name(poRole->GetName());
		poInfo->set_roleid(poRole->GetID());
		poInfo->set_rolecreatedtime((UINT32)poRole->GetRoleAllInfo().brief().inittime());
		poInfo->mutable_idipdata()->CopyFrom(poRole->GetRoleAllInfo().idiprecord());			//是否需要重新打包一次?
		poInfo->set_clientip(poRole->GetClientConf().ip());
		poInfo->set_lastlogintime(poRole->GetLoginTime());
		poInfo->set_token(poRole->GetClientConf().token());
		poInfo->set_logintype(poRole->GetClientConf().logintype());
		poInfo->set_loginplatid(poRole->GetClientConf().platid());

		Scene* poScene = poRole->GetCurrScene();
		if(poScene != NULL)
		{
			poInfo->set_mapid(poScene->GetSceneTemplateID());
			poInfo->set_sceneid(poScene->GetSceneID());
		}
		else
		{
			poInfo->set_mapid(INVALID_MAP_ID);
			poInfo->set_sceneid(INVALID_SCENE_ID);
		}
	}

	/*if(GSConfig::Instance()->GetLine() == MAIN_HALL_GS_LINE)
	{
		return;
	}*/

	SceneManager::scene_iter itScene = SceneManager::Instance()->Begin();
	SceneManager::scene_iter itSceneEnd = SceneManager::Instance()->End();
	for(; itScene != itSceneEnd; ++itScene)
	{
		Scene* poScene = itScene->second;
		if(poScene->GetSceneInfo()->m_pConf->IsStaticScene) continue;

		roRes.add_mapidlist(poScene->GetSceneTemplateID());
		roRes.add_sceneidlist(poScene->GetSceneID());
		roRes.add_owneridlist(poScene->GetOwnerID());
	}

	
	time_t time = GameTime::GetTime() + 2;
	TimeSpecific spec(time);
	CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&OnRecoverGSInfo2MS));
}

void RpcM2G_CrashRecoverM2G::OnDelayReplyRpc(const CrashRecoverM2GArg &roArg, CrashRecoverM2GRes &roRes, const CUserData &roUserData)
{
}
