#include "pch.h"
#include "common/ptcr2g_mscrashrecoverr2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "config.h"
#include "gamelogic/rolesummarymgr.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "common/ptcg2m_mscrashrecoverg2m.h"
#include "live/livemanager.h"
#include "event/eventmgr.h"

// generate by ProtoGen at date: 2016/11/23 20:40:18

void PtcR2G_mscrashrecoverr2g::Process(UINT32 dwConnID)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return;
	}

	PtcG2M_mscrashrecoverg2m ntf;
	ntf.m_Data.set_iscrossgs(GSConfig::Instance()->IsCrossGS());
	ntf.m_Data.set_gsline(GSConfig::Instance()->GetLine());

	Role* tempRole = NULL;
	std::list<Role*>::iterator it = RoleManager::Instance()->Begin();
	std::list<Role*>::iterator itEnd = RoleManager::Instance()->End();
	for(; it != itEnd; ++it)
	{
		Role* poRole = *it;
		if (poRole->GetFromServerId() != m_Data.serverid())
		{
			continue;
		}
		tempRole = poRole;

		ntf.m_Data.add_sessionlist(poRole->GetSessionID());
		KKSG::RoleInfoOnMs* poInfo = ntf.m_Data.add_rolelist();
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

	if (tempRole)
	{
		tempRole->SendToMS(ntf);
	}
	EventMgr::Instance()->OnConnectMS(m_Data.serverid());
	LiveManager::Instance()->SyncAllInfoToMs(m_Data.serverid());
}
