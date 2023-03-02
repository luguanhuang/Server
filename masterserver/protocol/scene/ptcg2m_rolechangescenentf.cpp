#include "pch.h"
#include "scene/ptcg2m_rolechangescenentf.h"
#include "role/rolemanager.h"
#include "role/rpcm2g_fetchroleinfoonms.h"
#include "network/gslink.h"
#include "network/rslink.h"
#include "scene/scenemanager.h"
#include "other/matchhandler.h"
#include "worldboss/worldbossmgr.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/11/4 16:15:51

void PtcG2M_RoleChangeSceneNtf::Process(UINT32 dwConnID)
{
	CRole* poRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if(poRole == NULL)
	{
		LogWarn("Can't find role %llu, ms maybe crashed earlier, fetch role info from gs_%u", m_Data.roleid(), m_Data.gsline());

		RpcM2G_FetchRoleInfoOnMs* rpc = RpcM2G_FetchRoleInfoOnMs::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_gsline(m_Data.gsline());

		if(m_Data.iscrossgs())
		{
			RSLink::Instance()->SendTo(*rpc);
		}
		else
		{
			GSLink::Instance()->SendToLine(m_Data.gsline(), *rpc);
		}
		return;
	}

	HMatchHandler handler;
	handler.OnChangeScene(poRole);


	///> 统计离开世界boss的人数
	if (poRole->GetMapID() == WORLDBOSS_MAP_ID && m_Data.mapid() != WORLDBOSS_MAP_ID)
	{
		WorldBossMgr::Instance()->DesRole(poRole->GetID());
		SSInfo<<"role leave world boss scene, to:"<<m_Data.mapid()<<" roleid:"<<poRole->GetID()<<END;
	}

	poRole->SetGsLine(m_Data.gsline());
	poRole->SetMapID(m_Data.mapid());
	poRole->SetSceneID(m_Data.sceneid());
	poRole->SetState(ROLE_STATE_IN_GAME);
	poRole->SetInCrossGs(m_Data.iscrossgs());

	poRole->OnEnterScene();

	OneOwnerScene* onescene = CSceneManager::Instance()->FindOwnerSceneBySceneId(m_Data.mapid(), m_Data.sceneid());
	if (NULL != onescene)
	{
		onescene->ResetState();
	}

	SSDebug << "role: " << poRole->GetID() << ", changle line: " << m_Data.gsline() << ", cross: " << m_Data.iscrossgs() << END;
}
