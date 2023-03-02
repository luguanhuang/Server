#include "pch.h"
#include "scene/rpcc2g_scenemobaop.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenemobabattle.h"
#include "gamelogic/skilllevel.h"

// generate by ProtoGen at date: 2017/6/7 17:02:30

RPC_SERVER_IMPLEMETION(RpcC2G_SceneMobaOp, SceneMobaOpArg, SceneMobaOpRes)

void RpcC2G_SceneMobaOp::OnCall(const SceneMobaOpArg &roArg, SceneMobaOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SceneMobaBattle* battle = scene->GetMobaBattleHandler();
	if (NULL == battle)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	MobaRole* roleinfo = battle->GetMobaRole(role->GetID());
	if (NULL == roleinfo)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	switch (roArg.op())
	{
	case KKSG::MobaOp_LevelSkill:
		{
			///> 技能升级
			KKSG::ErrorCode errorcode = role->Get<RoleSkillSet>()->LevelTransformSkill(2, roleinfo->GetLevel(), roleinfo->GetLevel(), roArg.param());
			roRes.set_errorcode(errorcode);
			break;
		}
	case KKSG::MobaOp_Upgrade:
		{
			UINT32 nowParam = 0;
			KKSG::ErrorCode errorcode = battle->Upgrade(role->GetID(), roArg.param(), nowParam);
			if (errorcode == KKSG::ERR_SUCCESS)
			{
				roRes.set_nowparam(nowParam);
			}
			roRes.set_errorcode(errorcode);
			break;
		}
	default:
		{
			LogWarn("role [%llu], MobaOp [%d] is unknown", role->GetID(), roArg.op());
			roRes.set_errorcode(KKSG::ERR_FAILED);
			break;
		}
	}
}

void RpcC2G_SceneMobaOp::OnDelayReplyRpc(const SceneMobaOpArg &roArg, SceneMobaOpRes &roRes, const CUserData &roUserData)
{
}
