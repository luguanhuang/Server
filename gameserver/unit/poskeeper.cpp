#include "pch.h"
#include "poskeeper.h"
#include "scene/sceneconfig.h"
#include "table/globalconfig.h"
#include "scene/scene.h"
#include "pb/project.pb.h"
#include "scene/scenepvp.h"
#include "role.h"
#include "scene/scenegmf.h"

PositionKeeper::PositionKeeper()
{
	m_lastHallTemplateID = 0;
	m_initFace = 0.0f;
}

PositionKeeper::~PositionKeeper()
{

}

void PositionKeeper::ResetHall()
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(GetGlobalConfig().InitialReturnSceneID);
	m_lastHallTemplateID =  GetGlobalConfig().InitialReturnSceneID;
	m_lastHallPosition.x = pSceneInfo->m_pConf->StartPos[0][0];
	m_lastHallPosition.y = pSceneInfo->m_pConf->StartPos[0][1];
	m_lastHallPosition.z = pSceneInfo->m_pConf->StartPos[0][2];
	m_lastHallFace = pSceneInfo->m_pConf->StartRot[1];
}

void PositionKeeper::ResetPos(UINT32 sceneid, Vector3& pos, float& face)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneid);
	pos.x = pSceneInfo->m_pConf->StartPos[0][0];
	pos.y = pSceneInfo->m_pConf->StartPos[0][1];
	pos.z = pSceneInfo->m_pConf->StartPos[0][2];
	face = pSceneInfo->m_pConf->StartRot[1];
}

EnterPosition PositionKeeper::GetEnterScenePosition(Scene *pScene, Role* pRole)
{
	EnterPosition pos;
	if (pScene->GetSceneType() == KKSG::SCENE_HALL)
	{
		if (!m_lastHallPosition.IsZero())
		{
			if (!pScene->IsPosInvalid(m_lastHallPosition))
			{
				ResetPos(pScene->GetSceneTemplateID(), m_lastHallPosition, m_lastHallFace);
			}
			pos.position = m_lastHallPosition;
			pos.face = m_lastHallFace;
			return pos;
		}
	}
	else if (pScene->GetSceneType() == KKSG::SCENE_GUILD_HALL)
	{
		if (!m_lastguildpos.IsZero())
		{
			if (!pScene->IsPosInvalid(m_lastguildpos))
			{
				ResetPos(pScene->GetSceneTemplateID(), m_lastguildpos, m_lastguildface);
			}
			pos.position = m_lastguildpos;
			pos.face = m_lastguildface;
			return pos;
		}
	}
	if(pScene->GetVsHandler())
	{
		pos = pScene->GetVsHandler()->GetInitPos(pRole->GetRoleID());
	}
	else
	{
		pos.position = pScene->GetSceneInfo()->GetStartPos(pScene->GetRoleCount());
		pos.face = pScene->GetSceneInfo()->GetStartFace(0);
	}
	return pos;
}

void PositionKeeper::OnLeaveScene(Scene *pScene, const Vector3 &position, float face)
{
	if (pScene->GetSceneType() == KKSG::SCENE_HALL)
	{
		m_lastHallTemplateID = pScene->GetSceneTemplateID();
		m_lastHallPosition = position;
		m_lastHallFace = face;
	}
	else if (pScene->GetSceneType() == KKSG::SCENE_GUILD_HALL)
	{
		m_lastHallTemplateID = pScene->GetSceneTemplateID();
		m_lastguildface = face;
		m_lastguildpos = position;
	}
}

void PositionKeeper::InitFromBrief(const KKSG::RoleBrief &brief)
{
	m_lastHallTemplateID = brief.sceneid();
	m_lastHallPosition = brief.position();
	m_lastHallFace = brief.face();
}

void PositionKeeper::SaveToBrief(KKSG::RoleBrief *pBrief)
{
	pBrief->set_sceneid(m_lastHallTemplateID);
	pBrief->mutable_position()->set_x(m_lastHallPosition.x);
	pBrief->mutable_position()->set_y(m_lastHallPosition.y);
	pBrief->mutable_position()->set_z(m_lastHallPosition.z);
	pBrief->set_face(m_lastHallFace);
}

