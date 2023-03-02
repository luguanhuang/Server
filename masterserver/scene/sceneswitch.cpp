#include "pch.h"
#include "sceneswitch.h"
#include "role/role.h"
#include "network/gslink.h"
#include "scene/scenemanager.h"
#include "scene/rpcm2n_changescenefromms.h"
#include "friend/friendop.h"
#include "network/rslink.h"
#include "network/controllink.h"
#include "logutil.h"


bool CSceneSwitch::EnterScene(CRole* poRole, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, UINT32 gsLine, bool isCrossGs, UINT32 mapID)
{
	if(poRole == NULL)
	{
		return false;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogWarn("Role[%llu] not in state game, can't enter scene", poRole->GetID());
		return false;
	}

	CScene* poScene = NULL;
	// 本服创建的场景SceneManager会管理，跨服区的场景不会
	if (isCrossGs == false)
	{
		poScene = CSceneManager::Instance()->GetScene(dwSceneInstanceID);
		if(poScene == NULL)
		{
			LogWarn("Can't find scene %u, enter scene failed", dwSceneInstanceID);
			return false;
		}
		else
		{
			mapID = poScene->GetMapID();//非跨服取值	
		}
	}
	else
	{
		if (0 == mapID)//跨服必须传递mapID
		{
			SSError << " enter cross scene mapid = 0 " << END;
		}
		if (roData.crossdata_size() > 0)
		{
			poRole->FillCrossData(*(const_cast<KKSG::SceneSwitchData&>(roData)).mutable_crossdata(0));
		}
		else
		{
			poRole->FillCrossData(*(const_cast<KKSG::SceneSwitchData&>(roData)).add_crossdata());
		}
	}

	RpcM2N_ChangeSceneFromMs* rpc = RpcM2N_ChangeSceneFromMs::CreateRpc();
	rpc->m_oArg.set_destsceneid(dwSceneInstanceID);
	rpc->m_oArg.set_mapid(mapID);
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.mutable_data()->CopyFrom(roData);
	rpc->m_oArg.set_gsline(gsLine);
	rpc->m_oArg.set_istocrossgs(isCrossGs);
	ControlLink::Instance()->SendTo(*rpc);

	return true;
}

bool CSceneSwitch::EnterMainHall(CRole* poRole)
{
	if(poRole == NULL)
	{
		return false;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		return false;
	}

	//if(poRole->GetGsLine() == MAIN_HALL_GS_LINE)
	//{
	//	return false;
	//}

	RpcM2N_ChangeSceneFromMs* rpc = RpcM2N_ChangeSceneFromMs::CreateRpc();
	rpc->m_oArg.set_destsceneid(INVALID_SCENE_ID);
	rpc->m_oArg.set_mapid(HALL_MAP_ID);
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.set_istocrossgs(false);
	ControlLink::Instance()->SendTo(*rpc);

	return true;
}