#include "pch.h"
#include "horse/rpcc2g_horsereconnect.h"
#include "scene/scenehorserace.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenehorserace.h"
#include "buff/buffrecord.h"

// generate by ProtoGen at date: 2016/12/14 10:45:40

RPC_SERVER_IMPLEMETION(RpcC2G_HorseReConnect, HorseReConnectArg, HorseReConnectRes)

void RpcC2G_HorseReConnect::OnCall(const HorseReConnectArg &roArg, HorseReConnectRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	Scene *pScene = pRole->GetCurrScene();
	if (pScene == NULL)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	SceneHorseRace* pRace = pScene->GetHorseRaceHandler();
	if (pRace == NULL)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	pRace->FillAllInfo(pRole, roRes);
	pRole->Get<BuffRecord>()->GetReHorseInfo(&roRes);
}

void RpcC2G_HorseReConnect::OnDelayReplyRpc(const HorseReConnectArg &roArg, HorseReConnectRes &roRes, const CUserData &roUserData)
{
}
