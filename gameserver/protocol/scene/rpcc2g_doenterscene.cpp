#include "pch.h"
#include "scene/rpcc2g_doenterscene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "entity/XRole.h"
#include "math/XVector3.h"
#include "scene/scene.h"
#include "scene/scenepreparelist.h"
#include "unit/pet.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/13 19:09:40

RPC_SERVER_IMPLEMETION(RpcC2G_DoEnterScene, DoEnterSceneArg, DoEnterSceneRes)

static void FillResponse(Scene* scene, Role* role, DoEnterSceneRes &roRes)
{
	if (NULL == role || NULL == scene)
	{
		SSError<<"role:"<<role<<" scene:"<<scene<<END;
		return;
	}
	*roRes.mutable_state() = role->GetState();
	roRes.set_fightgroup(role->GetFightGroup());

	roRes.set_face(role->GetXRole()->GetFaceDegree());
	KKSG::Vec3* pos = roRes.mutable_pos();
	pos->set_x(role->GetXRole()->GetPosition_p().x);
	pos->set_y(role->GetXRole()->GetPosition_p().y);
	pos->set_z(role->GetXRole()->GetPosition_p().z);

	if(!scene->IsSyncScene())
	{
		roRes.set_battlestamp(role->SetBattleStamp());
		LogInfo("roleid [%llu], sceneid [%u], battleStamp:%s", role->GetID(), scene->GetSceneTemplateID(), role->GetBattleStamp().c_str());
	}
	roRes.set_iswatchend(scene->IsWatchEnd());

	if (!role->IsWatcher())
	{
		if (scene->IsSceneReady())
		{
			///> 后进的人解除不能动状态
			if (SceneConfig::Instance()->IsHasReadyScene(scene->GetSceneType()))
			{
				///> 特殊场景不解除
				if (scene->GetSceneType() != KKSG::SCENE_HEROBATTLE && 
					scene->GetSceneType() != KKSG::SCENE_MOBA)
				{
					role->MakePuppet(false);
				}
			}
			///> 场景ready
			role->OnSceneReady();
		}
	}

	roRes.set_specialstate(role->GetSpecialState());

	roRes.mutable_scenestate()->set_isready(scene->IsSceneReady());
	roRes.mutable_scenestate()->set_runstate(scene->GetSceneState());
	roRes.set_is_cross(GSConfig::Instance()->IsCrossGS());
	roRes.set_initface(role->GetPositionKeeper().GetInitFace());

	if (role->GetFlag(RSF_ISLoginReconnect))
	{
		KKSG::LoginReconnectEnterSceneData* lrdata = roRes.mutable_lrdata();
		role->GetAppearance(*lrdata->mutable_selfappearance());
		role->PackDeathNotfiy(lrdata->mutable_deathinfo());
		lrdata->set_isautofight(role->IsClientAutoFight());

		role->SetFlag(RSF_ISLoginReconnect, false);
		role->SetFlag(RSF_ISLoginReconnectFinally, true);
	}
}

void RpcC2G_DoEnterScene::OnCall(const DoEnterSceneArg &roArg, DoEnterSceneRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	if (pRole->GetLoadingScene())
	{
		// client loaded notify
		Scene* loadscene = pRole->GetLoadingScene();
		UINT32 sceneid = roArg.sceneid();
		if (0 == sceneid || loadscene->GetSceneTemplateID() == sceneid)
		{
			//clear loading state first
			pRole->ClearEnterNtf();
			pRole->SetFlag(RSF_ISLoadingScene, false);

			UINT32 delayid = DelayRpc();
			loadscene->AddRoleAfterLoadedScene(pRole, delayid);
			return;
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_DOENTERSCENE_FAILED);
			SSWarn<<"do enter scene failed, sceneid:"<<sceneid<<", loading sceneid:"<<loadscene->GetSceneTemplateID()<<END;
		}
	}
	else
	{
		// client not received, send again
		Scene* scene = pRole->GetCurrScene();
		if (NULL != scene)
		{
			ClientLoadingInfo* info = scene->GetLoadingInfo(pRole->GetID());
			if (NULL != info && info->eState == EnterState)
			{
				//clear loading state first
				pRole->ClearEnterNtf();
				pRole->SetFlag(RSF_ISLoadingScene, false);

				FillResponse(scene, pRole, roRes);
				///> 周围人信息
				std::vector<Role*> roles;
				scene->GetRolesCanSeeSorted(pRole, roles);
				for (UINT32 i = 0; i < roles.size(); ++i)
				{
					if (roles[i] != pRole)
					{
						roles[i]->GetAppearance(*roRes.add_otherunits());
					}
				}
				std::vector<Unit*> units;
				scene->GetUnitsCanSee(pRole, units);
				for (UINT32 i = 0; i < units.size(); ++i)
				{
					units[i]->GetAppearance(*roRes.add_otherunits());
				}

				//pRole->OnOthersAppear(roles);
	
				return;
			}
			else
			{
				SSWarn<<"do enter scene error:"<<pRole->GetID()<<END;
			}
		}
	}
	roRes.set_fightgroup(KKSG::FightRole);
	roRes.set_face(0);
}

void RpcC2G_DoEnterScene::OnDelayReplyRpc(const DoEnterSceneArg &roArg, DoEnterSceneRes &roRes, const CUserData &roUserData)
{
	ClientLoadedInfo* loaded = (ClientLoadedInfo*)roUserData.m_pUserPtr;
	if (NULL != loaded)
	{
		FillResponse(loaded->scene, loaded->role, roRes);
	}
	else
	{
		SSError<<"loaded is NULL"<<END;
	}
	return;
}
