#include "pch.h"
#include "sceneswitch.h"
#include "unit/rolemanager.h"
#include "sceneconfig.h"
#include "scenemanager.h"
#include "common/ptcg2c_gserrornotify.h"
#include "scene/rpcg2n_changesceneverify.h"
#include "scene/ptcg2m_enterscenereqtoms.h"
#include "config.h"
#include "network/mslink.h"
#include "network/controllink.h"
#include "unit/role.h"



bool CheckEnterSceneCondition(CSceneSwitch::RoleList& roRoleList, UINT32 dwSceneTemplateID, const KKSG::SceneSwitchData& roData, INT32& rnSceneType)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneTemplateID);
	if(pSceneInfo == NULL)
	{
		LogError("Invalid scene template id [%u]", dwSceneTemplateID);
		return false;
	}

	if(roData.has_teamdata())
	{
		;//组队不判断
	}
	else
	{
		for(CSceneSwitch::RoleList::size_type i = 0; i < roRoleList.size(); ++i)
		{
			Role* poRole = roRoleList[i];
			UINT32 arg = 0;
			int ret = SceneManager::Instance()->CheckEnterSceneCondition(poRole, pSceneInfo, &arg, roData.sceneowner());
			if (ret != KKSG::ERR_SUCCESS)
			{
				LogInfo("check enter scene condition error roleid=%llu error=%d sceneid=%d",poRole->GetID(),ret,pSceneInfo->m_pConf->id);
				if (ret != KKSG::ERR_FAILED)
				{
					PtcG2C_GSErrorNotify oErrorPtc;
					oErrorPtc.m_Data.set_errorno(ret);
					if (0 != arg)
					{
						oErrorPtc.m_Data.add_param(arg);
						oErrorPtc.m_Data.add_param(dwSceneTemplateID);
					}
					poRole->Send(oErrorPtc);
				}

				return false;
			}
		}
	}

	rnSceneType = pSceneInfo->m_pConf->type;
	return true;
}

// 不支持跨服多人一起切
bool CSceneSwitch::ChangeScene(RoleList& roRoleList, UINT32 dwSceneTemplateID, const KKSG::SceneSwitchData& roData)
{
	if(roRoleList.empty()) return false;

	for(RoleList::size_type i = 0; i < roRoleList.size(); ++i)
	{
		Role* poRole = roRoleList[i];
		if(poRole->GetFlag(RSF_ISSceneSwitch_Verifying))
		{
			LogError("Role %llu is in state: SceneSwitch_Verifying, can't change scene", poRole->GetID());
			return false;
		}
	}

	INT32 nSceneType = KKSG::SCENE_HALL;
	if(!CheckEnterSceneCondition(roRoleList, dwSceneTemplateID, roData, nSceneType))
	{
		return false;
	}

	//特殊处理
	if (SceneConfig::Instance()->IsMsCreateScene(nSceneType))
	{
		PtcG2M_EnterSceneReqToMs oPtc;
		oPtc.m_Data.set_mapid(dwSceneTemplateID);
		oPtc.m_Data.mutable_data()->CopyFrom(roData);
		for(RoleList::size_type i = 0; i < roRoleList.size(); ++i)
		{
			Role* poRole = roRoleList[i];
			oPtc.m_Data.add_rolelist(poRole->GetID());
		}

		//MSLink::Instance()->SendTo(oPtc);
		roRoleList[0]->SendToMS(oPtc);
		return true;
	}

	RpcG2N_ChangeSceneVerify* rpc = RpcG2N_ChangeSceneVerify::CreateRpc();
	rpc->m_oArg.set_mapid(dwSceneTemplateID);
	rpc->m_oArg.set_destsceneid(INVALID_SCENE_ID);
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	rpc->m_oArg.mutable_data()->CopyFrom(roData);
	for(RoleList::size_type i = 0; i < roRoleList.size(); ++i)
	{
		Role* poRole = roRoleList[i];
		rpc->m_oArg.add_rolelist(poRole->GetID());
		poRole->SetFlag(RSF_ISSceneSwitch_Verifying, true);
	}
	roRoleList[0]->SendToCtrl(*rpc);
	//ControlLink::Instance()->SendTo(*rpc);
	return true;
}


bool CSceneSwitch::EnterScene(Role* poRole, UINT32 dwSceneTemplateID, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, bool isToCross, UINT32 gsLine)
{
	if(poRole == NULL) return false;

	if(poRole->GetFlag(RSF_ISSceneSwitch_Verifying))
	{
		LogError("Role %llu is in state: SceneSwitch_Verifying, can't enter scene %u", poRole->GetID(), dwSceneInstanceID);
		return false;
	}

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneTemplateID);
	if(pSceneInfo == NULL)
	{
		LogError("Invalid scene template id [%u]", dwSceneTemplateID);
		return false;
	}

	UINT32 arg = 0;

	bool isWatcher = false;
	for(size_t i = 0; i < roData.watchers_size(); ++i)
	{
		if (poRole->GetID() == roData.watchers(i))
		{
			isWatcher = true;
			break;
		}
	}

	int ret = KKSG::ERR_FAILED;
	if (roData.has_teamdata())
	{
		ret = KKSG::ERR_SUCCESS;//组队判断独立于场景
	}
	else
	{
		if (!isWatcher)
		{
			ret = SceneManager::Instance()->CheckEnterSceneCondition(poRole, pSceneInfo, &arg);
		}
		else
		{
			ret = SceneManager::Instance()->CheckWatcherEnterSceneCondition(poRole, pSceneInfo);
		}
	}
	if (ret != KKSG::ERR_SUCCESS)
	{
		if (ret != KKSG::ERR_FAILED)
		{
			PtcG2C_GSErrorNotify oErrorPtc;
			oErrorPtc.m_Data.set_errorno(ret);
			if (0 != arg)
			{
				oErrorPtc.m_Data.add_param(arg);
				oErrorPtc.m_Data.add_param(dwSceneTemplateID);
			}
			poRole->Send(oErrorPtc);
		}
		return false;
	}

	if (isToCross)
	{
		if (roData.crossdata_size() > 0)
		{
			poRole->FillCrossData(*(const_cast<KKSG::SceneSwitchData&>(roData)).mutable_crossdata(0));
		}
		else
		{
			poRole->FillCrossData(*(const_cast<KKSG::SceneSwitchData&>(roData)).add_crossdata());
		}
	}

	poRole->SetFlag(RSF_ISSceneSwitch_Verifying, true);
	RpcG2N_ChangeSceneVerify* rpc = RpcG2N_ChangeSceneVerify::CreateRpc();
	rpc->m_oArg.set_mapid(dwSceneTemplateID);
	rpc->m_oArg.set_destsceneid(dwSceneInstanceID);
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.mutable_data()->CopyFrom(roData);
	rpc->m_oArg.set_isdestcross(isToCross);
	rpc->m_oArg.set_destline(gsLine);
	poRole->SendToCtrl(*rpc);
	//ControlLink::Instance()->SendTo(*rpc);
	return true;
}