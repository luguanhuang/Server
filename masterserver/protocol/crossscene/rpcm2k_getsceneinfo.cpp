#include "pch.h"
#include "crossscene/rpcm2k_getsceneinfo.h"
#include "scene/sceneswitch.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/10/29 10:49:14

RPC_CLIENT_IMPLEMETION(RpcM2K_GetSceneInfo, GetSceneInfoArg, GetSceneInfoRes)

void RpcM2K_GetSceneInfo::OnReply(const GetSceneInfoArg &roArg, const GetSceneInfoRes &roRes, const CUserData &roUserData)
{
	SSDebug << "cross scene size: " << roRes.scenes_size() << END;
	if (roRes.scenes_size() > 0)
	{
		const KKSG::CrossSceneInfo& info = roRes.scenes(0);
		CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
		if (role)
		{
			SSDebug << "Begin Enter Cross Scene" << END;
			KKSG::SceneSwitchData switchData;
			CSceneSwitch::EnterScene(role, info.scene_id(), switchData, info.gsline(), true);
		}
	}
}

void RpcM2K_GetSceneInfo::OnTimeout(const GetSceneInfoArg &roArg, const CUserData &roUserData)
{
}
