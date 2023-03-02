#include "pch.h"
#include "bigmelee/rpcg2k_reportbmroleinfo.h"
#include "scene/bigmeleefight.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/6/15 22:12:05

RPC_CLIENT_IMPLEMETION(RpcG2K_ReportBMRoleInfo, BMSceneFightInfo, BMSceneRankRes)

void RpcG2K_ReportBMRoleInfo::OnReply(const BMSceneFightInfo &roArg, const BMSceneRankRes &roRes, const CUserData &roUserData)
{
	if(roArg.has_roles())
	{
		Role *pRole =	RoleManager::Instance()->FindByRoleID(roArg.roles().roleid());
		if (pRole!=NULL)
		{
			Scene *pScene = pRole->GetCurrScene();
			if (pScene&&pScene->GetSceneType()==KKSG::SCENE_BIGMELEE_FIGHT)
			{
				if (roRes.error()==KKSG::ERR_SUCCESS)
				{
					pScene->GetBMFightHandler()->SetRoleRankPoint(pRole->GetID(), roRes);
				}
			}
		}
	}

}

void RpcG2K_ReportBMRoleInfo::OnTimeout(const BMSceneFightInfo &roArg, const CUserData &roUserData)
{
}
