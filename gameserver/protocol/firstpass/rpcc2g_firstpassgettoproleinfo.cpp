#include "pch.h"
#include "firstpass/rpcc2g_firstpassgettoproleinfo.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "gamelogic/firstpassrecord.h"
#include "gamelogic/rolesummarymgr.h"
#include "network/mslink.h"
#include "gamelogic/firstpassconfig.h"
#include "table/expeditionconfigmgr.h"
#include "define/teamdef.h"
#include "table/FirstPassTable.h"
#include "scene/sceneconfig.h"

// generate by ProtoGen at date: 2016/8/24 20:58:25

RPC_SERVER_IMPLEMETION(RpcC2G_FirstPassGetTopRoleInfo, FirstPassGetTopRoleInfoArg, FirstPassGetTopRoleInfoRes)

void RpcC2G_FirstPassGetTopRoleInfo::OnCall(const FirstPassGetTopRoleInfoArg &roArg, FirstPassGetTopRoleInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	RpcG2M_MSCommendFirstPass* rpc = RpcG2M_MSCommendFirstPass::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_fistpassid(roArg.firstpassid());
	rpc->m_oArg.set_isgettopinfo(true);
	rpc->m_oArg.set_serverid(pRole->GetServerId());

	pRole->SendToMS(*rpc);
}

void RpcC2G_FirstPassGetTopRoleInfo::OnDelayReplyRpc(const FirstPassGetTopRoleInfoArg &roArg, FirstPassGetTopRoleInfoRes &roRes, const CUserData &roUserData)
{
	roRes.set_error((KKSG::ErrorCode)roUserData.m_dwUserData);
	if (!roUserData.m_pUserPtr)
	{
		return;
	}

	MSCommendFirstPassRes* poRes = (MSCommendFirstPassRes*)roUserData.m_pUserPtr;
	if (!poRes->roleids_size())
	{
		return;
	}

	for (int i = 0; i < poRes->roleids_size(); ++i)
	{
		KKSG::UnitAppearance* info = roRes.add_infos();
		*info = poRes->infos(i);
	}
	roRes.set_commendnum(poRes->commendnum());
	roRes.set_time(poRes->passtime());
	roRes.set_starlevel(poRes->starlevel());

	const FirstPassTable::RowData* table = FirstPassConfig::Instance()->GetFirstPassDataByID(roArg.firstpassid());
	if (table && table->SceneID.size())
	{
		auto data = SceneConfig::Instance()->FindSceneInfo(table->SceneID[0]);
		if (data && data->m_pConf && data->m_pConf->type == KKSG::SCENE_DRAGON)
		{
			roRes.set_starlevel(-2);
		}
	}
}
