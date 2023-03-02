#include "pch.h"
#include "reswar/rpcc2g_reswarbuff.h"
#include "scene/scenereswarpve.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"


// generate by ProtoGen at date: 2016/11/26 17:31:00

RPC_SERVER_IMPLEMETION(RpcC2G_ResWarBuff, ResWarBuffArg, ResWarBuffRes)

void RpcC2G_ResWarBuff::OnCall(const ResWarBuffArg &roArg, ResWarBuffRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene * pScene = pRole->GetCurrScene();
	if (pScene)
	{
		SceneResWarPVE *pPVEScene = pScene->GetSceneResWarPVE();
		if (pPVEScene)
		{
			UINT32 nBuff = pPVEScene->GetRandBuffID();
			roRes.set_buffid(nBuff);
		}
	}
	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcC2G_ResWarBuff::OnDelayReplyRpc(const ResWarBuffArg &roArg, ResWarBuffRes &roRes, const CUserData &roUserData)
{
}
