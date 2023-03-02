#include "pch.h"
#include "battlewatch/rpcc2g_enterwatchbattle.h"
#include "battlewatch/rpcg2m_enterwatchbattlems.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/sceneswitch.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/7/11 16:18:09

RPC_SERVER_IMPLEMETION(RpcC2G_EnterWatchBattle, EnterWatchBattleArg, EnterWatchBattleRes)

void RpcC2G_EnterWatchBattle::OnCall(const EnterWatchBattleArg &roArg, EnterWatchBattleRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (!GSConfig::Instance()->IsCrossGS())
	{
		if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
		{
			roRes.set_error(KKSG::ERR_FAILED);
			return;
		}
	}

	RpcG2M_EnterWatchBattleMs* rpc = RpcG2M_EnterWatchBattleMs::CreateRpc();

	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_liveid(roArg.liveid());
	rpc->m_oArg.set_livetype(roArg.type());
	rpc->m_oArg.set_rpcid(DelayRpc());

	role->SendToMS(*rpc);
}

void RpcC2G_EnterWatchBattle::OnDelayReplyRpc(const EnterWatchBattleArg &roArg, EnterWatchBattleRes &roRes, const CUserData &roUserData)
{
	roRes.set_error(KKSG::ErrorCode(roUserData.m_dwUserData));
	if(roRes.error() == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr != NULL)
	{
		EnterWatchBattleMsRes* res = (EnterWatchBattleMsRes*)roUserData.m_pUserPtr;

		Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
		if (NULL == role)
		{
			return;
		}

		KKSG::SceneSwitchData oData;
		oData.add_watchers(role->GetID());
		UINT32 crossLine = 0;
		if(res->istocross())
		{
			crossLine = res->crossgslineid();
		}
		CSceneSwitch::EnterScene(role, res->scenetemplateid(), res->sceneid(), oData, res->istocross(), crossLine);
	}
}
