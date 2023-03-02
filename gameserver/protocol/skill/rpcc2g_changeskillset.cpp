#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/skilllevel.h"
#include "scene/scene.h"
#include "scene/sceneconfig.h"
#include "gamelogic/rolesummarymgr.h"
#include "skill/rpcc2g_changeskillset.h"

// generate by ProtoGen at date: 2017/2/3 21:24:45

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeSkillSet, ChangeSkillSetArg, ChangeSkillSetRes)

void RpcC2G_ChangeSkillSet::OnCall(const ChangeSkillSetArg &roArg, ChangeSkillSetRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	// level
	if (role->GetLevel() < (int)GetGlobalConfig().SkillPageNewOpen)
	{
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	// scene
	Scene* scene = role->GetCurrScene();
	if (NULL == scene || !SceneConfig::Instance()->IsBattleSceneExcept(scene->GetSceneType()))
	{
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	// change index
	if (!role->Get<RoleSkillSet>()->ChangeSkillSet(roArg.index()))
	{
		SSWarn<<"roleid:"<<role->GetID()<<" index:"<<roArg.index()<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	role->Get<RoleSkillSet>()->FillRecord(roRes.mutable_record());	

	// role summary
	RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_SKILL);	
	// powerpoint
	role->RecalcuateAttributeAndNotify(RecalAttr_ChangeSkillSet);

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_ChangeSkillSet::OnDelayReplyRpc(const ChangeSkillSetArg &roArg, ChangeSkillSetRes &roRes, const CUserData &roUserData)
{
}
