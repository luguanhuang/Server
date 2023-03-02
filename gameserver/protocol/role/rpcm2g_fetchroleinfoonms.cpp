#include "pch.h"
#include "role/rpcm2g_fetchroleinfoonms.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "config.h"

// generate by ProtoGen at date: 2016/11/23 15:33:01

RPC_SERVER_IMPLEMETION(RpcM2G_FetchRoleInfoOnMs, FetchRoleInfoOnMsArg, FetchRoleInfoOnMsRes)

void RpcM2G_FetchRoleInfoOnMs::OnCall(const FetchRoleInfoOnMsArg &roArg, FetchRoleInfoOnMsRes &roRes)
{
	Role* poRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(poRole == NULL)
	{
		LogError("Can't find role %llu, fetch role info failed", roArg.roleid());
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_iscrossgs(GSConfig::Instance()->IsCrossGS());
	roRes.set_sessionid(poRole->GetSessionID());

	KKSG::RoleInfoOnMs* poInfo = roRes.mutable_roleinfo();
	poInfo->set_accountid(poRole->GetAccount());
	poInfo->set_name(poRole->GetName());
	poInfo->set_roleid(poRole->GetID());
	poInfo->set_rolecreatedtime((UINT32)poRole->GetRoleAllInfo().brief().inittime());
	poInfo->mutable_idipdata()->CopyFrom(poRole->GetRoleAllInfo().idiprecord());			//是否需要重新打包一次?
	poInfo->set_clientip(poRole->GetClientConf().ip());
	poInfo->set_lastlogintime(poRole->GetLoginTime());
	poInfo->set_token(poRole->GetClientConf().token());
	poInfo->set_logintype(poRole->GetClientConf().logintype());

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

void RpcM2G_FetchRoleInfoOnMs::OnDelayReplyRpc(const FetchRoleInfoOnMsArg &roArg, FetchRoleInfoOnMsRes &roRes, const CUserData &roUserData)
{
}
