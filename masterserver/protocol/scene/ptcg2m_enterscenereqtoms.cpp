#include "pch.h"
#include "scene/ptcg2m_enterscenereqtoms.h"
#include "scene/scenemanager.h"
#include "role/rolemanager.h"
#include "logutil.h"
#include "scene/sceneswitch.h"

// generate by ProtoGen at date: 2016/11/4 19:57:17

void PtcG2M_EnterSceneReqToMs::Process(UINT32 dwConnID)
{
	if(m_Data.rolelist_size() == 0)
	{
		return;
	}
	if(m_Data.rolelist_size() > 1)
	{
		return;
	}

	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < m_Data.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = m_Data.rolelist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if(poRole == NULL)
		{
			LogError("Can't find role %llu when g2m enter scene %u verify", qwRoleID, m_Data.mapid());
			return;
		}

		oRoleList.push_back(poRole);
	}

	LineDest dest;
	dest.mapid = m_Data.mapid();
	const SceneConf* conf = CSceneConfig::Instance()->GetSceneConf(dest.mapid);
	if (NULL != conf)
	{
		dest.type = conf->type;
	}
	const KKSG::SceneSwitchData* data = m_Data.has_data() ? &m_Data.data() : NULL;

	UINT32 errorcode = CSceneManager::Instance()->FillLineDest(oRoleList, INVALID_SCENE_ID, m_Data.mapid(), data, dest);
	if(errorcode != KKSG::ERR_SUCCESS)
	{
		LogWarn("Role [%s] try to enter scene tid [%u] failed, error:%d", Uint64List2Str(m_Data.rolelist()).c_str(), m_Data.mapid(), errorcode);
		return;
	}

	if(dest.isdelay)
	{
		return;
	}

	for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
	{
		CRole* poRole = oRoleList[i];
		KKSG::SceneSwitchData data;
		CSceneSwitch::EnterScene(poRole, dest.destid, data);
		LogInfo("Role [%llu] begin to change scene, scene tid [%u], sid [%u]", poRole->GetID(), dest.mapid, dest.destid);
	}
}
