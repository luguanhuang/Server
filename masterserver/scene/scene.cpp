#include "pch.h"
#include "scene.h"
#include "commondef.h"


CScene::CScene()
:m_dwID(INVALID_SCENE_ID)
,m_dwMap(INVALID_MAP_ID)
,m_dwLineID(INVALID_LINE_ID)
,m_dwRoleNum(0)
,m_dwObserverNum(0)
,m_poConf(NULL)
{
}

CScene::~CScene()
{
}

void CScene::OnCreate()
{

}

void CScene::OnDestroy()
{

}

UINT32 CScene::GetSceneType()
{
	if (m_poConf)
	{
		return m_poConf->type;
	}
	return 0;
}

void CScene::BroadCast(CProtocol& roPtc)
{


}
